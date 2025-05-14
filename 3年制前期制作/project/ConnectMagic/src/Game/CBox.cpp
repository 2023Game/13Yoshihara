#include "CBox.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"
#include "CConnectPointManager.h"

// 接続ターゲットの座標
#define TARGET_POS_1 CVector( 0.0f,10.0f, 0.0f)
#define TARGET_POS_2 CVector( 0.0f, 0.0f, 0.0f)
#define TARGET_POS_3 CVector( 5.0f, 5.0f, 0.0f)
#define TARGET_POS_4 CVector(-5.0f, 5.0f, 0.0f)
#define TARGET_POS_5 CVector( 0.0f, 5.0f, 5.0f)
#define TARGET_POS_6 CVector( 0.0f, 5.0f,-5.0f)

// 箱の重さ
#define WEIGHT 0.1f

// コライダーの半径
#define RADIUS 2.4f

// コンストラクタ
CBox::CBox()
	: CConnectObject(WEIGHT)
{
	mpModel = CResourceManager::Get<CModel>("Box");

	Scale(Scale() * 2.0f);

	// コライダーを生成
	CreateCol();

	// 接続ターゲットを生成
	CreateTarget(TARGET_POS_1);
	CreateTarget(TARGET_POS_2);
	CreateTarget(TARGET_POS_3);
	CreateTarget(TARGET_POS_4);
	CreateTarget(TARGET_POS_5);
	CreateTarget(TARGET_POS_6);
}

// デストラクタ
CBox::~CBox()
{
	SAFE_DELETE(mpCharaCol);
}

// コライダーを生成
void CBox::CreateCol()
{
	// フィールドやオブジェクトと衝突判定をするコライダー
	mpCol = new CColliderSphere
	(
		this, ELayer::eObject,
		RADIUS
	);
	// 座標を調整
	mpCol->Position(Position() + CVector(0.0f, RADIUS / 2, 0.0f));
	// フィールド、オブジェクト、コネクトオブジェクトの探知用、スイッチと衝突判定
	mpCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall,
		ELayer::eObject, ELayer::eConnectSearch, ELayer::eSwitch });

	// キャラと衝突判定をするコライダー
	mpCharaCol = new CColliderMesh
	(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("Box_Col")
	);
	// プレイヤー、敵、オブジェクトと衝突判定
	mpCharaCol->SetCollisionLayers({ ELayer::ePlayer, ELayer::eEnemy, ELayer::eObject });

	// 接続部の管理クラスの衝突判定するコライダーに追加
	CConnectPointManager::Instance()->AddCollider(mpCharaCol);

	// カメラの衝突判定するコライダーに追加
	CCamera::CurrentCamera()->AddCollider(mpCharaCol);
}
