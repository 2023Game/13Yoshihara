#pragma once
#include "CBillBoardImage.h"
#include "CColliderSphere.h"

// �|�[�^���̃G�t�F�N�g
class CPortal : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CPortal();
	// �f�X�g���N�^
	~CPortal();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
	// �Փ˔���
	CCollider* mpCol;
};