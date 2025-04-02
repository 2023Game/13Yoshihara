#include "CBlockWall.h"

// �R���X�g���N�^
CBlockWall::CBlockWall()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
{
	mpModel = CResourceManager::Get<CModel>("BlockWall");

	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CBlockWall::~CBlockWall()
{
	SAFE_DELETE(mpMeshCol);
}

// �R���C�_�\�𐶐�
void CBlockWall::CreateCol()
{
	mpMeshCol = new CColliderMesh
	{
		this, ELayer::eWall,
		mpModel,
		true
	};
}

// �`��
void CBlockWall::Render()
{
	mpModel->Render(Matrix());
}
