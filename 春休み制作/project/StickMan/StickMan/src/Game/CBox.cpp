#include "CBox.h"
#include "CColliderMesh.h"

#define TARGET_POS_1 CVector( 0.0f,10.0f, 0.0f)
#define TARGET_POS_2 CVector( 0.0f, 0.0f, 0.0f)
#define TARGET_POS_3 CVector( 5.0f, 5.0f, 0.0f)
#define TARGET_POS_4 CVector(-5.0f, 5.0f, 0.0f)
#define TARGET_POS_5 CVector( 0.0f, 5.0f, 5.0f)
#define TARGET_POS_6 CVector( 0.0f, 5.0f,-5.0f)

// コンストラクタ
CBox::CBox()
	: CConnectObject()
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
}

// 更新
void CBox::Update()
{
}

// 引っ張られた時の処理
void CBox::Pull()
{
}

// コライダーを生成
void CBox::CreateCol()
{
	mpCol = new CColliderMesh
	(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("Box_Col"),
		true
	);
	// プレイヤー、フィールド、敵、オブジェクト、コネクトオブジェクトの探知用と衝突判定
	mpCol->SetCollisionLayers({ ELayer::ePlayer,
		ELayer::eGround,ELayer::eWall,ELayer::eObject,ELayer::eConnectSearch });
}
