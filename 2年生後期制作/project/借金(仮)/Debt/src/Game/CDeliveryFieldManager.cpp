#include "CDeliveryFieldManager.h"
#include "CDeliveryField.h"
#include "CDeliveryHome.h"
#include "CDeliveryPlayer.h"
#include "CSky.h"
#include "Maths.h"

// 配達のフィールド管理クラスのインスタンス
CDeliveryFieldManager* CDeliveryFieldManager::spInstance = nullptr;

// 生成するフィールドの数
#define FIELD_NUM 5

// フィールド同士の距離
#define FIELD_DIST 400.0f * SCALE_RATIO

// プレイヤーとの距離の最高値、
// これより遠いフィールドはプレイヤーの前方へ
// 家は無効にする
#define PLAYER_DIST (FIELD_DIST * 1.5f)

// 家のオフセット座標
#define HOME_OFFSET_POSX   250.0f
#define HOME_OFFSET_POSY    10.0f
#define HOME_OFFSET_POSZ -450.0f
// 家の方向
#define HOME_ROT_L CVector(0.0f,-90.0f,0.0f)
#define HOME_ROT_R CVector(0.0f, 90.0f,0.0f)

// 最後に家の生成判定をした座標が
// この値を超えたら次の家の生成判定をする
#define HOME_JUDGE_POSZ -100.0f

// 家の生成率
#define HOME_POP_RATE 25.0f

// インスタンスを取得
CDeliveryFieldManager* CDeliveryFieldManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CDeliveryFieldManager::CDeliveryFieldManager()
	: mLastPopZ(HOME_OFFSET_POSZ)
{
	spInstance = this;

	// フィールドを生成
	CreateField();
}

// デストラクタ
CDeliveryFieldManager::~CDeliveryFieldManager()
{
	SAFE_DELETE(spInstance);
}

// 更新
void CDeliveryFieldManager::Update()
{
	// 最後に家の生成判定をしたZ座標を手前へ移動していく
	float moveSpeed = FORCE_MOVE_SPEED * Times::DeltaTime();
	mLastPopZ = mLastPopZ + moveSpeed;
	// 最後に生成したZ座標が判定を超えたら
	if (mLastPopZ > HOME_JUDGE_POSZ)
	{
		// 家を生成するか判定する
		CreateHome();
	}

	// プレイヤーの座標を取得
	CPlayerBase* player = CPlayerBase::Instance();
	CVector playerPos = player->Position();

	// 全てのフィールドとプレイヤーの距離を計算する
	for (CDeliveryField* field : mFields)
	{
		CVector fieldPos = field->Position();
		// プレイヤーより奥にあるなら次へ
		if (fieldPos.Z() < playerPos.Z()) continue;

		// プレイヤーとの距離
		float dist = fieldPos.Z() - playerPos.Z();
		// プレイヤーとの距離が最高値以上なら
		if (dist > PLAYER_DIST)
		{
			// 一番遠くのフィールドのZ座標から
			// フィールド同士の距離分さらに遠くのZ座標
			float newPosZ = mpFarField->Position().Z() - FIELD_DIST;
			// 座標を設定
			field->Position(0.0f, 0.0f, newPosZ);
			// 一番遠いフィールドを保存
			mpFarField = field;
		}
	}
	// 全ての家とプレイヤーの距離を計算する
	for (CDeliveryHome* home: mHomes)
	{
		CVector homePos = home->Position();
		// プレイヤーより奥にあるなら次へ
		if (homePos.Z() < playerPos.Z()) continue;

		// プレイヤーとの距離
		float dist = homePos.Z() - playerPos.Z();
		// プレイヤーとの距離が最高値以上なら
		if (dist > PLAYER_DIST)
		{
			// 無効
			home->SetEnable(false);
			home->SetShow(false);
		}
	}
}

// 指定のフィールドを取得する
CDeliveryField* CDeliveryFieldManager::GetField(int num, bool& end)
{
	if (num == mFields.size() - 1) end = true;

	return mFields[num];
}

// フィールドを生成
void CDeliveryFieldManager::CreateField()
{
	for (int i = 0; i < FIELD_NUM; i++)
	{
		CDeliveryField* field = new CDeliveryField();
		field->Position(0.0f, 0.0f, -FIELD_DIST * (i - 1));
		mFields.push_back(field);
		// 一番最後のフィールドのZ座標を保存
		if (i == FIELD_NUM - 1) mpFarField = field;
	}
}

// 家を生成
void CDeliveryFieldManager::CreateHome()
{
	// 生成するか
	bool isPopL = false;	// 左
	bool isPopR = false;	// 右

	// 1から100までの100個の数字からランダムで取得
	float pop = Math::Rand(1, 100);
	// 生成率以下なら左に生成
	if (pop <= HOME_POP_RATE) isPopL = true;
	// もう一度ランダム取得
	pop = Math::Rand(1, 100);
	// 生成率以下なら右に生成
	if (pop <= HOME_POP_RATE) isPopR = true;

	// 生成できたか
	bool isPopSuccessL = false;	// 左
	bool isPopSuccessR = false;	// 右
	// 生成しないなら生成できたことにする
	if (!isPopL) isPopSuccessL = true;
	if (!isPopR) isPopSuccessR = true;

	for (CDeliveryHome* home : mHomes)
	{
		// 両方生成出来たらループ終了
		if (isPopSuccessL && isPopSuccessR) break;
		// 既に有効なら次へ
		if (home->IsEnable()) continue;

		// 左をまだ生成していないなら
		if (!isPopSuccessL)
		{
			// 左の座標を設定
			home->Position(
				-HOME_OFFSET_POSX,
				 HOME_OFFSET_POSY,
				 HOME_OFFSET_POSZ
			);
			// 方向を設定
			home->Rotation(HOME_ROT_L);
			isPopSuccessL = true;
		}
		// 右をまだ生成していないなら
		else if (!isPopSuccessR)
		{
			// 右の座標を設定
			home->Position(
				HOME_OFFSET_POSX,
				HOME_OFFSET_POSY,
				HOME_OFFSET_POSZ
			);
			// 方向を設定
			home->Rotation(HOME_ROT_R);
			isPopSuccessR = true;
		}
		// 有効
		home->SetEnable(true);
		home->SetShow(true);
	}

	// 左をまだ生成していないなら
	if (!isPopSuccessL)
	{
		// 新しい家を生成
		CDeliveryHome* home = new CDeliveryHome();
		// 左の座標を設定
		home->Position(
			-HOME_OFFSET_POSX,
			 HOME_OFFSET_POSY,
			 HOME_OFFSET_POSZ
		);
		// 方向を設定
		home->Rotation(HOME_ROT_L);
		// リストに追加
		mHomes.push_back(home);
	}
	// 右をまだ生成していないなら
	if (!isPopSuccessR)
	{
		// 新しい家を生成
		CDeliveryHome* home = new CDeliveryHome();
		// 右の座標を設定
		home->Position(
			HOME_OFFSET_POSX,
			HOME_OFFSET_POSY,
			HOME_OFFSET_POSZ
		);
		// 方向を設定
		home->Rotation(HOME_ROT_R);
		// リストに追加
		mHomes.push_back(home);
	}

	// 最後に判定したZ座標を家の生成場所に設定
	mLastPopZ = HOME_OFFSET_POSZ;
}
