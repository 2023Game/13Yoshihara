#include "CResidentManager.h"
#include "CResident.h"
#include "CNavNode.h"


// 住人の生成時の座標
#define START_POS_LEFT  CVector(-100.0f,0.0f,-100.0f)
#define START_POS_RIGHT CVector( 100.0f,0.0f, 100.0f)

// 左の道の巡回ポイント
#define PATROLPOINT_L_1 CVector(-100.0f, 4.0f, 100.0f)
#define PATROLPOINT_L_2 CVector(-100.0f, 4.0f,-100.0f)
// 右の道の巡回ポイント
#define PATROLPOINT_R_1 CVector(100.0f, 4.0f,-100.0f)
#define PATROLPOINT_R_2 CVector(100.0f, 4.0f, 100.0f)
 
// 住人の最大数
#define RESIDENT_MAX_NUM 2

// 住人管理クラスのインスタンス
CResidentManager* CResidentManager::spInstance = nullptr;

// インスタンスのポインタの取得
CResidentManager* CResidentManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CResidentManager::CResidentManager()
{
	spInstance = this;

	// 住人のモデル
	mpResidentModel = CResourceManager::Get<CModelX>("Resident");

	// 経路探索用のノードを作成
	CreateNavNodes();

	// 住人の作成
	CreateResident();
}

// デストラクタ
CResidentManager::~CResidentManager()
{
	// インスタンスと削除している住人管理クラスが同一なら削除
	if (spInstance == this)
	{
		spInstance = nullptr;
	}
}

// 住人を全て生成
void CResidentManager::CreateResident()
{
	for (int i = 0; i < RESIDENT_MAX_NUM; i++)
	{
		// 2で割ったあまりが0なら左の道に生成
		if (i % 2 == 0)
		{
			mpResidents.push_back(new CResident(mpResidentModel,START_POS_LEFT,mpPatrolPointsLeft));
		}
		// あまりが1なら右の道に生成
		else
		{
			mpResidents.push_back(new CResident(mpResidentModel, START_POS_RIGHT, mpPatrolPointsRight));
		}
	}
}

// 経路探索用のノードを作成
void CResidentManager::CreateNavNodes()
{
	// 左の道の巡回ポイントを生成
	mpPatrolPointsLeft.push_back(new CNavNode(PATROLPOINT_L_1, true));
	mpPatrolPointsLeft.push_back(new CNavNode(PATROLPOINT_L_2, true));

	// 右の道の巡回ポイントを生成
	mpPatrolPointsRight.push_back(new CNavNode(PATROLPOINT_R_1, true));
	mpPatrolPointsRight.push_back(new CNavNode(PATROLPOINT_R_2, true));
}
