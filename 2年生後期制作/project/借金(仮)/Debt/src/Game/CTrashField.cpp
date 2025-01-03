#include "CTrashField.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "CVehicleManager.h"

// コンストラクタ
CTrashField::CTrashField()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TrashStage");

	mpGroundColliderMesh = new CColliderMesh(this, ELayer::eField, CResourceManager::Get<CModel>("TrashStage_Ground_Collision"), true);
	mpWallColliderMesh = new CColliderMesh(this, ELayer::eWall, CResourceManager::Get<CModel>("TrashStage_Wall_Collision"), true);
	mpObjectColliderMesh = new CColliderMesh(this, ELayer::eObject, CResourceManager::Get<CModel>("TrashStage_Object_Collision"), true);

	// フィールドのオブジェクトを生成
	CreateFieldObjects();
	// 経路探索用のノードを生成
	CreateNavNodes();
}

// デストラクタ
CTrashField::~CTrashField()
{
}

// フィールドのオブジェクト生成
void CTrashField::CreateFieldObjects()
{
}

// 経路探索用のノードを生成
void CTrashField::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		new CNavNode(CVector( 10.0f, 0.0f,  40.0f), this);
		new CNavNode(CVector(-10.0f, 0.0f,  40.0f), this);
		new CNavNode(CVector(-10.0f, 0.0f, -40.0f), this);
		new CNavNode(CVector( 10.0f, 0.0f, -40.0f), this);
	}
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
