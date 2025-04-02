#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

// �����ǂ��ǃN���X
class CBlockWall : public CObjectBase
{
public:
	// �R���X�g���N�^
	CBlockWall();
	// �f�X�g���N�^
	~CBlockWall();

	// �`��
	void Render() override;

private:
	// �R���C�_�\�𐶐�
	void CreateCol();

	// �R���C�_�\
	CCollider* mpMeshCol;
	// ���f��
	CModel* mpModel;
};