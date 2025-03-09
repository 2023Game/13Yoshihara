#include "CDeliveryField.h"

// コンストラクタ
CDeliveryField::CDeliveryField()
	: CFieldBase()
{
	// スケールを設定
	Scale(Scale() * SCALE_RATIO);
	mpModel = CResourceManager::Get<CModel>("DeliveryStage");
	mpGroundColliderMesh = new CColliderMesh
	(
		this, ELayer::eGround,
		CResourceManager::Get<CModel>
		(
			"DeliveryStage_Ground_Collision"
		),
		true
	);
	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CDeliveryField::~CDeliveryField()
{
	SAFE_DELETE(mpGroundColliderMesh);
}

// 更新
void CDeliveryField::Update()
{
	// 強制移動の速度分、手前へ移動
	float moveSpeed = FORCE_MOVE_SPEED * Times::DeltaTime();
	Position(Position() + CVector::forward * moveSpeed);
}

// 描画
void CDeliveryField::Render()
{
	CFieldBase::Render();
}

// フィールドのオブジェクト生成
void CDeliveryField::CreateFieldObjects()
{

}