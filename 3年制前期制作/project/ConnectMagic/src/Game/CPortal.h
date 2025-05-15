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

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
	// �Փ˔���
	CCollider* mpCol;
};