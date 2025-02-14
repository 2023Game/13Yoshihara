#include "CDeliveryFieldManager.h"
#include "CDeliveryField.h"
#include "CDeliveryFieldItem.h"
#include "CDeliveryHome.h"
#include "CDeliveryObstruction.h"
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
#define HOME_OFFSET_POSZ  -450.0f
// 家の方向
#define HOME_ROT_L CVector(0.0f,-90.0f,0.0f)
#define HOME_ROT_R CVector(0.0f, 90.0f,0.0f)
// 最後に家の生成判定をした座標が
// この値を超えたら次の家の生成判定をする
#define HOME_JUDGE_POSZ -100.0f
// 家の生成率
#define HOME_POP_RATE 25.0f

// 障害物のオフセット座標
#define OBSTRUCTION_OFFSET_POSZ -450.0f
// 最後に障害物の生成判定をした座標が
// この値を超えたら次の障害物の生成判定をする
#define OBSTRUCTION_JUDGE_POSZ -100.0f
// 障害物の生成率
#define OBSTRUCTION_POP_RATE 50.0f

// アイテムのオフセット座標
#define ITEM_OFFSET_POSZ -550.0f
// 最後にアイテムの生成判定をした座標が
// この値を超えたら次のアイテムの生成判定をする
#define ITEM_JUDGE_POSZ -200.0f
// アイテムの生成率
#define ITEM_POP_RATE 50.0f

// インスタンスを取得
CDeliveryFieldManager* CDeliveryFieldManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CDeliveryFieldManager::CDeliveryFieldManager()
	: mLastHomePopZ(HOME_OFFSET_POSZ)
	, mLastObstructionPopZ(OBSTRUCTION_OFFSET_POSZ)
	, mLastFieldItemPopZ(ITEM_OFFSET_POSZ)
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
	mLastHomePopZ = mLastHomePopZ + moveSpeed;
	// 最後に生成したZ座標が判定を超えたら
	if (mLastHomePopZ > HOME_JUDGE_POSZ)
	{
		// 家を生成するか判定する
		CreateHome();
	}
	// 最後に障害物の生成判定をしたZ座標を手前へ移動していく
	mLastObstructionPopZ = mLastObstructionPopZ + moveSpeed;
	// 最後に生成したZ座標が判定を超えたら
	if (mLastObstructionPopZ > OBSTRUCTION_JUDGE_POSZ)
	{
		// 障害物を生成するか判定する
		CreateObstruction();
	}
	// 最後に障害物の生成判定をしたZ座標を手前へ移動していく
	mLastFieldItemPopZ = mLastFieldItemPopZ + moveSpeed;
	// 最後に生成したZ座標が判定を超えたら
	if (mLastFieldItemPopZ > ITEM_JUDGE_POSZ)
	{
		// アイテムを生成するか判定する
		CreateFieldItem();
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
	// 全ての障害物とプレイヤーの距離を計算する
	for (CDeliveryObstruction* obstruction : mObstructions)
	{
		CVector obstructionPos = obstruction->Position();
		// プレイヤーより奥にあるなら次へ
		if (obstructionPos.Z() < playerPos.Z()) continue;

		// プレイヤーとの距離
		float dist = obstructionPos.Z() - playerPos.Z();
		// プレイヤーとの距離が最高値以上なら
		if (dist > PLAYER_DIST)
		{
			// 無効
			obstruction->SetEnable(false);
			obstruction->SetShow(false);
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
	mLastHomePopZ = HOME_OFFSET_POSZ;
}

// 障害物を生成するか判定する
void CDeliveryFieldManager::CreateObstruction()
{
	// 生成するか
	bool isPopL1 = false;	// 左1
	bool isPopL2 = false;	// 左2
	bool isPopR1 = false;	// 右1
	bool isPopR2 = false;	// 右2

	// 1から100までの100個の数字からランダムで取得
	float pop = Math::Rand(1, 100);
	// 生成率以下なら左1に生成
	if (pop <= OBSTRUCTION_POP_RATE) isPopL1 = true;
	// もう一度ランダム取得
	pop = Math::Rand(1, 100);
	// 生成率以下なら左1に生成
	if (pop <= OBSTRUCTION_POP_RATE) isPopL2= true;
	// もう一度ランダム取得
	pop = Math::Rand(1, 100);
	// 生成率以下なら右1に生成
	if (pop <= OBSTRUCTION_POP_RATE) isPopR1 = true;
	// もう一度ランダム取得
	pop = Math::Rand(1, 100);
	// 生成率以下なら右1に生成
	if (pop <= OBSTRUCTION_POP_RATE) isPopR2 = true;

	// 生成できたか
	bool isPopSuccessL1 = false;	// 左1
	bool isPopSuccessL2 = false;	// 左2
	bool isPopSuccessR1 = false;	// 右1
	bool isPopSuccessR2 = false;	// 右2
	// 生成しないなら生成できたことにする
	if (!isPopL1) isPopSuccessL1 = true;
	if (!isPopL2) isPopSuccessL2 = true;
	if (!isPopR1) isPopSuccessR1 = true;
	if (!isPopR2) isPopSuccessR2 = true;

	// 通れない状態を作らないため
	// 全て生成するなら一つ生成しない
	if (!isPopSuccessL1 &&
		!isPopSuccessL2 &&
		!isPopSuccessR1 &&
		!isPopSuccessR2)
	{
		int random = Math::Rand(1, 4);
		switch (random)
		{
		case 1:
			// 左1を生成しない
			isPopSuccessL1 = true;
			break;
		case 2:
			// 左2を生成しない
			isPopSuccessL2 = true;
			break;
		case 3:
			// 右1を生成しない
			isPopSuccessR1 = true;
			break;
		case 4:
			// 右2を生成しない
			isPopSuccessR2 = true;
			break;
		}
	}

	for (CDeliveryObstruction* obstruction : mObstructions)
	{
		// 全て生成出来たらループ終了
		if (isPopSuccessL1 &&
			isPopSuccessL2 &&
			isPopSuccessR1 &&
			isPopSuccessR2) break;
		// 既に有効なら次へ
		if (obstruction->IsEnable()) continue;

		// 障害物の座標をランダムで設定
		ObstructionRandomPos(obstruction,
			isPopSuccessL1,isPopSuccessL2,
			isPopSuccessR1, isPopSuccessR2);
		// 有効
		obstruction->SetEnable(true);
		obstruction->SetShow(true);
	}

	while (!isPopSuccessL1 ||
		!isPopSuccessL2 ||
		!isPopSuccessR1 ||
		!isPopSuccessR2)
	{
		// 新しい障害物を生成
		CDeliveryObstruction* obstruction = new CDeliveryObstruction();
		// 障害物の座標をランダムで設定
		ObstructionRandomPos(obstruction,
			isPopSuccessL1, isPopSuccessL2,
			isPopSuccessR1, isPopSuccessR2);
		// リストに追加
		mObstructions.push_back(obstruction);
	}

	// 最後に判定したZ座標を家の生成場所に設定
	mLastObstructionPopZ = OBSTRUCTION_OFFSET_POSZ;
}

// 障害物の座標をランダムで設定
void CDeliveryFieldManager::ObstructionRandomPos(
	CDeliveryObstruction* obstruction,
	bool& isPopSuccessL1, bool& isPopSuccessL2,
	bool& isPopSuccessR1, bool& isPopSuccessR2)
{
	// 左1をまだ生成していないなら
	if (!isPopSuccessL1)
	{
		// 左の座標を設定
		obstruction->Position(
			ROAD_LEFT1_POSX,
			0.0f,
			OBSTRUCTION_OFFSET_POSZ
		);
		// 道を設定
		obstruction->SetRoadType(ERoadType::eLeft1);
		isPopSuccessL1 = true;
	}
	// 左2をまだ生成していないなら
	else if (!isPopSuccessL2)
	{
		// 左の座標を設定
		obstruction->Position(
			ROAD_LEFT2_POSX,
			0.0f,
			OBSTRUCTION_OFFSET_POSZ
		);
		// 道を設定
		obstruction->SetRoadType(ERoadType::eLeft2);
		isPopSuccessL2 = true;
	}
	// 右1をまだ生成していないなら
	else if (!isPopSuccessR1)
	{
		// 右の座標を設定
		obstruction->Position(
			ROAD_RIGHT1_POSX,
			0.0f,
			OBSTRUCTION_OFFSET_POSZ
		);
		// 道を設定
		obstruction->SetRoadType(ERoadType::eRight1);
		isPopSuccessR1 = true;
	}
	// 右2をまだ生成していないなら
	else if (!isPopSuccessR2)
	{
		// 右の座標を設定
		obstruction->Position(
			ROAD_RIGHT2_POSX,
			0.0f,
			OBSTRUCTION_OFFSET_POSZ
		);
		// 道を設定
		obstruction->SetRoadType(ERoadType::eRight2);
		isPopSuccessR2 = true;
	}
}

// アイテムを生成するか判定する
void CDeliveryFieldManager::CreateFieldItem()
{
	// 生成するか
	bool isPop = false;

	// 1から100までの100個の数字からランダムで取得
	float pop = Math::Rand(1, 100);
	// 生成率以下なら生成
	if (pop <= ITEM_POP_RATE) isPop = true;
	// 生成しないなら処理しない
	if (!isPop) return;

	// 生成できたか
	bool isPopSuccess = false;

	for (CDeliveryFieldItem* fieldItem : mFieldItems)
	{
		// 生成出来たらループ終了
		if (isPopSuccess) break;
		// 既に有効なら次へ
		if (fieldItem->IsEnable()) continue;

		// まだ生成していないなら
		if (!isPopSuccess)
		{
			// 座標をランダムで設定
			ItemRandomPos(fieldItem);
		}
		// 生成した
		isPopSuccess = true;
		// 有効
		fieldItem->SetEnable(true);
		fieldItem->SetShow(true);
	}

	// まだ生成していないなら
	if (!isPopSuccess)
	{
		// 新しい障害物を生成
		CDeliveryFieldItem* fieldItem = new CDeliveryFieldItem();
		
		// 座標をランダムで設定
		ItemRandomPos(fieldItem);
		
		// リストに追加
		mFieldItems.push_back(fieldItem);
	}

	// 最後に判定したZ座標を家の生成場所に設定
	mLastFieldItemPopZ = ITEM_OFFSET_POSZ;
}

// アイテムの座標をランダムで設定
void CDeliveryFieldManager::ItemRandomPos(CDeliveryFieldItem* fieldItem)
{
	int random = Math::Rand(1, 4);
	switch (random)
	{
	case 1:
		// 左1の座標を設定
		fieldItem->Position(
			ROAD_LEFT1_POSX,
			0.0f,
			ITEM_OFFSET_POSZ
		);
		// 道を設定
		fieldItem->SetRoadType(ERoadType::eLeft1);
		break;
	case 2:
		// 左2の座標を設定
		fieldItem->Position(
			ROAD_LEFT2_POSX,
			0.0f,
			ITEM_OFFSET_POSZ
		);
		// 道を設定
		fieldItem->SetRoadType(ERoadType::eLeft2);
		break;
	case 3:
		// 右1の座標を設定
		fieldItem->Position(
			ROAD_RIGHT1_POSX,
			0.0f,
			ITEM_OFFSET_POSZ
		);
		// 道を設定
		fieldItem->SetRoadType(ERoadType::eRight1);
		break;
	case 4:
		// 右2の座標を設定
		fieldItem->Position(
			ROAD_RIGHT2_POSX,
			0.0f,
			ITEM_OFFSET_POSZ
		);
		// 道を設定
		fieldItem->SetRoadType(ERoadType::eRight2);
		break;
	}
}
