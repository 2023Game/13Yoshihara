#include "CAirConnectObj.h"
#include "CColliderSphere.h"

// 接続ターゲットの座標
#define TARGET_POS CVector(2.5f,0.0f,0.0f)

// 空中の接続オブジェクトの重さ
#define WEIGHT 1.0f

// コライダーの半径
#define RADIUS 5.0f

// コンストラクタ
CAirConnectObj::CAirConnectObj()
	: CConnectObject(WEIGHT)
{
	// 空中の接続オブジェクトのタグを設定
	SetConnectObjTag(EConnectObjTag::eAir);

	mpModel = CResourceManager::Get<CModel>("AirConnectObj");

	// コライダーを生成
	CreateCol();

	// 接続ターゲットを生成
	CreateTarget(TARGET_POS);
}

// デストラクタ
CAirConnectObj::~CAirConnectObj()
{
}

// コライダーを生成
void CAirConnectObj::CreateCol()
{
	mpCol = new CColliderSphere
	(
		this, ELayer::eObject,
		RADIUS, true
	);
	// プレイヤー、敵、オブジェクト、コネクトオブジェクトの探知用と衝突判定をする
	mpCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eObject,ELayer::eConnectSearch });
}
