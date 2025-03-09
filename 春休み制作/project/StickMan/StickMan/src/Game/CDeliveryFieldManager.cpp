#include "CDeliveryFieldManager.h"
#include "CDeliveryField.h"
#include "CDeliveryFieldItem.h"
#include "CDeliveryHome.h"
#include "CDeliveryObstruction.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemyManager.h"
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
#define HOME_POP_RATE 50.0f

// 障害物のオフセット座標
#define OBSTRUCTION_OFFSET_POSZ -450.0f
// 最後に障害物の生成判定をした座標が
// この値を超えたら次の障害物の生成判定をする
#define OBSTRUCTION_JUDGE_POSZ -200.0f
// 障害物の生成率
#define OBSTRUCTION_POP_RATE 100.0f

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
	, mLastObstructionRoad(ERoadType::None)
	, mLastItemRoad(ERoadType::None)
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
	// 最後に障害物の生成判定をしたZ座標を手前へ移動していく
	mLastObstructionPopZ = mLastObstructionPopZ + moveSpeed;
	// 最後にフィールドアイテムの生成判定をしたZ座標を手前へ移動していく
	mLastFieldItemPopZ = mLastFieldItemPopZ + moveSpeed;
	// 最後に生成したZ座標が判定を超えたら
	if (mLastHomePopZ > HOME_JUDGE_POSZ)
	{
		// 家を生成するか判定する
		CreateHome();
	}

	// 最後に生成したZ座標が判定を超えたら
	if (mLastObstructionPopZ > OBSTRUCTION_JUDGE_POSZ)
	{
		// 障害物を生成するか判定する
		CreateObstruction();
	}
	// 最後に生成したZ座標が判定を超えたら
	else if (mLastFieldItemPopZ > ITEM_JUDGE_POSZ)
	{
		// フィールドアイテムを生成するか判定する
		CreateFieldItem();
		// 敵が有効なら
		if (CDeliveryEnemyManager::Instance()->GetEnamyEnable())
		{
			// もう一つ生成
			CreateFieldItem();
		}
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

	// 全てのフィールドアイテムとプレイヤーの距離を計算する
	for (CDeliveryFieldItem* fieldItem : mFieldItems)
	{
		CVector fieldItemPos = fieldItem->Position();
		// プレイヤーより奥にあるなら次へ
		if (fieldItemPos.Z() < playerPos.Z()) continue;

		// プレイヤーとの距離
		float dist = fieldItemPos.Z() - playerPos.Z();
		// プレイヤーとの距離が最高値以上なら
		if (dist > PLAYER_DIST)
		{
			// 無効
			fieldItem->SetEnable(false);
			fieldItem->SetShow(false);
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
	bool isPop = false;

	// 1から100までの100個の数字からランダムで取得
	float pop = Math::Rand(1, 100);
	// 生成率以下なら生成
	if (pop <= OBSTRUCTION_POP_RATE) isPop = true;

	// 生成できたか
	bool isPopSuccess = false;

	for (CDeliveryObstruction* obstruction : mObstructions)
	{
		// 生成出来たらループ終了
		if (isPopSuccess) break;
		// 既に有効なら次へ
		if (obstruction->IsEnable()) continue;

		// 障害物の座標をランダムで設定
		ObstructionRandomPos(obstruction);
		// 生成した
		isPopSuccess = true;
		// 有効
		obstruction->SetEnable(true);
		obstruction->SetShow(true);
	}

	// まだ生成していないなら
	while (!isPopSuccess &&
		isPop)
	{
		// 新しい障害物を生成
		CDeliveryObstruction* obstruction = new CDeliveryObstruction();
		// 障害物の座標をランダムで設定
		ObstructionRandomPos(obstruction);
		// リストに追加
		mObstructions.push_back(obstruction);
		// 生成した
		isPopSuccess = true;
	}

	// 最後に判定したZ座標を障害物の生成場所に設定
	mLastObstructionPopZ = OBSTRUCTION_OFFSET_POSZ;
}

// 障害物の座標をランダムで設定
void CDeliveryFieldManager::ObstructionRandomPos(CDeliveryObstruction* obstruction)
{
	// 全ての道
	std::vector<ERoadType> allRoads =
	{ ERoadType::eLeft1,ERoadType::eLeft2,ERoadType::eRight1,ERoadType::eRight2 };
	// 生成できる道
	std::vector<ERoadType> canPopRoads;

	// 全ての道分繰り返す
	for (ERoadType road : allRoads)
	{
		// 最後に生成した道でなければ
		if (road != mLastObstructionRoad)
		{
			// 生成できる道に追加
			canPopRoads.push_back(road);
		}
	}

	// 1から道の数の乱数
	int random = Math::Rand(1, canPopRoads.size());
	// 生成する道を決定
	ERoadType popRoad = canPopRoads[random - 1];
	// 生成する道のX座標
	float popPosX = 0.0f;

	// 生成する道のX座標を決定
	switch (popRoad)
	{
	case ERoadType::eLeft1:

		popPosX = ROAD_LEFT1_POSX;

		break;
	case ERoadType::eLeft2:

		popPosX = ROAD_LEFT2_POSX;

		break;
	case ERoadType::eRight1:

		popPosX = ROAD_RIGHT1_POSX;

		break;
	case ERoadType::eRight2:

		popPosX = ROAD_RIGHT2_POSX;

		break;
	}		
	// 座標を設定
	obstruction->Position(
		popPosX,
		0.0f,
		OBSTRUCTION_OFFSET_POSZ
	);
	// 道を設定
	obstruction->SetRoadType(popRoad);
	// 最後に障害物の生成をした道を設定
	mLastObstructionRoad = popRoad;
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

	// 生成できたか
	bool isPopSuccess = false;

	for (CDeliveryFieldItem* fieldItem : mFieldItems)
	{
		// 生成出来たらループ終了
		if (isPopSuccess) break;
		// 既に有効なら次へ
		if (fieldItem->IsEnable()) continue;

		// アイテムの座標をランダムで設定
		ItemRandomPos(fieldItem);
		// 生成した
		isPopSuccess = true;
		// 有効
		fieldItem->SetEnable(true);
		fieldItem->SetShow(true);
	}

	// まだ生成していないなら
	while (!isPopSuccess&&
		isPop)
	{
		// 新しいアイテムを生成
		CDeliveryFieldItem* fieldItem = new CDeliveryFieldItem();

		// 座標をランダムで設定
		ItemRandomPos(fieldItem);

		// リストに追加
		mFieldItems.push_back(fieldItem);
		// 生成した
		isPopSuccess = true;
	}

	// 最後に判定したZ座標をアイテムの生成場所に設定
	mLastFieldItemPopZ = ITEM_OFFSET_POSZ;
}

// アイテムの座標をランダムで設定
void CDeliveryFieldManager::ItemRandomPos(CDeliveryFieldItem* fieldItem)
{
	// 全ての道
	std::vector<ERoadType> allRoads =
	{ ERoadType::eLeft1,ERoadType::eLeft2,ERoadType::eRight1,ERoadType::eRight2 };
	// 生成できる道
	std::vector<ERoadType> canPopRoads;

	// 全ての道分繰り返す
	for (ERoadType road : allRoads)
	{
		// 最後に生成した道でなければ
		if (road != mLastItemRoad)
		{
			// 生成できる道に追加
			canPopRoads.push_back(road);
		}
	}

	// 1から道の数の乱数
	int random = Math::Rand(1, canPopRoads.size());
	// 生成する道を決定
	ERoadType popRoad = canPopRoads[random - 1];
	// 生成する道のX座標
	float popPosX = 0.0f;

	// 生成する道のX座標を決定
	switch (popRoad)
	{
	case ERoadType::eLeft1:

		popPosX = ROAD_LEFT1_POSX;

		break;
	case ERoadType::eLeft2:

		popPosX = ROAD_LEFT2_POSX;

		break;
	case ERoadType::eRight1:

		popPosX = ROAD_RIGHT1_POSX;

		break;
	case ERoadType::eRight2:

		popPosX = ROAD_RIGHT2_POSX;

		break;
	}
	// 座標を設定
	fieldItem->Position(
		popPosX,
		0.0f,
		OBSTRUCTION_OFFSET_POSZ
	);
	// 道を設定
	fieldItem->SetRoadType(popRoad);
	// 最後に障害物の生成をした道を設定
	mLastItemRoad = popRoad;
}
