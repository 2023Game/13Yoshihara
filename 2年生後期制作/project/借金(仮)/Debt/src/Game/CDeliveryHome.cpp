#include "CDeliveryHome.h"
#include "CDeliveryField.h"
#include "CColliderMesh.h"

// コンストラクタ
CDeliveryHome::CDeliveryHome()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
{
	// モデル取得
	mpModel = CResourceManager::Get<CModel>("DeliveryHome");
	// 壁のコライダ―を設定
	mpWallCol = new CColliderMesh
	(
		this, ELayer::eWall,
		CResourceManager::Get<CModel>("DeliveryHome_Wall_Collision"),
		true
	);
	// ゴール判定のコライダ―設定
	mpGoalCol = new CColliderMesh
	(
		this, ELayer::eGoal,
		CResourceManager::Get<CModel>("DeliveryHome_Goal_Collision"),
		true
	);
}

// デストラクタ
CDeliveryHome::~CDeliveryHome()
{
	SAFE_DELETE(mpWallCol);
	SAFE_DELETE(mpGoalCol);
}

// 更新
void CDeliveryHome::Update()
{
	// 強制移動の速度分、手前へ移動
	float moveSpeed = FORCE_MOVE_SPEED * Times::DeltaTime();
	Position(Position() + CVector::forward * moveSpeed);
}

// 描画
void CDeliveryHome::Render()
{
	mpModel->Render(Matrix());
}
