#include "CDeliveryField.h"

// コンストラクタ
CDeliveryField::CDeliveryField()
	: CFieldBase()
{
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

}

// デストラクタ
CDeliveryField::~CDeliveryField()
{
}

// 更新
void CDeliveryField::Update()
{
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
