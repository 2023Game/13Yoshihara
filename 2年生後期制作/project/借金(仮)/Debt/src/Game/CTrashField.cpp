#include "CTrashField.h"
 
// コンストラクタ
CTrashField::CTrashField()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TrashStage");

	mpGroundColliderMesh = new CColliderMesh(this, ELayer::eField, CResourceManager::Get<CModel>("TrashStage_Ground_Collision"), true);
	mpWallColliderMesh = new CColliderMesh(this, ELayer::eWall, CResourceManager::Get<CModel>("TrashStage_Wall_Collision"), true);
	mpObjectColliderMesh = new CColliderMesh(this, ELayer::eObject, CResourceManager::Get<CModel>("TrashStage_Object_Collision"), true);

	CreateFieldObjects();
}

// デストラクタ
CTrashField::~CTrashField()
{
}

// フィールドのオブジェクト生成
void CTrashField::CreateFieldObjects()
{
}

// 更新
void CTrashField::Update()
{
}

// 描画
void CTrashField::Render()
{
	mpModel->Render(Matrix());
}
