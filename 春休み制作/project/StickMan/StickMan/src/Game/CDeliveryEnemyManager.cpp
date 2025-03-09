#include "CDeliveryEnemyManager.h"
#include "CDeliveryEnemy.h"
#include "CDeliveryField.h"
#include "Maths.h"

// 敵の生成位置
#define ENEMY_POP_POSZ 300.0f

// 敵の生成間隔
#define ENEMY_POP_TIME 20.0f

CDeliveryEnemyManager* CDeliveryEnemyManager::spInstance = nullptr;

// インスタンスを取得
CDeliveryEnemyManager* CDeliveryEnemyManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CDeliveryEnemyManager::CDeliveryEnemyManager()
	: mElapsedTime(0.0f)
{
	spInstance = this;
	// 敵の生成
	mpEnemy = new CDeliveryEnemy();
	// 最初は無効
	mpEnemy->SetOnOff(false);
}

// デストラクタ
CDeliveryEnemyManager::~CDeliveryEnemyManager()
{
	SAFE_DELETE(spInstance);
}

// 更新
void CDeliveryEnemyManager::Update()
{
	// 敵が無効なら
	if (!mpEnemy->IsEnable())
	{
		// 時間の経過
		mElapsedTime += Times::DeltaTime();
		// 敵の生成までの時間が経過したら
		if (mElapsedTime >= ENEMY_POP_TIME)
		{
			mElapsedTime = 0.0f;
			// ランダムな道に敵を生成
			PopEnemyRandomRoad();
		}
	}
}

// 敵が出現しているか
bool CDeliveryEnemyManager::GetEnamyEnable()
{
	return mpEnemy->IsEnable();
}

// ランダムな道に敵を生成
void CDeliveryEnemyManager::PopEnemyRandomRoad()
{
	// 1から4の乱数
	int random = Math::Rand(1, 4);
	switch (random)
	{
		// 左1の道
	case 1:
		mpEnemy->SetRoadType(ERoadType::eLeft1);
		mpEnemy->Position(ROAD_LEFT1_POSX, 0.0f, ENEMY_POP_POSZ);
		break;
		// 左2の道
	case 2:
		mpEnemy->SetRoadType(ERoadType::eLeft2);
		mpEnemy->Position(ROAD_LEFT2_POSX, 0.0f, ENEMY_POP_POSZ);
		break;
		// 右1の道
	case 3:
		mpEnemy->SetRoadType(ERoadType::eRight1);
		mpEnemy->Position(ROAD_RIGHT1_POSX, 0.0f, ENEMY_POP_POSZ);
		break;
		// 右2の道
	case 4:
		mpEnemy->SetRoadType(ERoadType::eRight2);
		mpEnemy->Position(ROAD_RIGHT2_POSX, 0.0f, ENEMY_POP_POSZ);
		break;
	}
	// 有効
	mpEnemy->SetOnOff(true);
}
