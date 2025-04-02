#include "CBlockWall.h"

// コンストラクタ
CBlockWall::CBlockWall()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
{
	mpModel = CResourceManager::Get<CModel>("BlockWall");

	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CBlockWall::~CBlockWall()
{
	SAFE_DELETE(mpMeshCol);
}

// コライダ―を生成
void CBlockWall::CreateCol()
{
	mpMeshCol = new CColliderMesh
	{
		this, ELayer::eWall,
		mpModel,
		true
	};
}

// 描画
void CBlockWall::Render()
{
	mpModel->Render(Matrix());
}
