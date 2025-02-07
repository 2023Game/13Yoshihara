#pragma once
#include "CPlayerBase.h"

#include "CObjectBase.h"
/*
�z�B�Q�[���̃v���C���[�N���X
*/
class CDeliveryPlayer : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDeliveryPlayer();
	// �f�X�g���N�^
	~CDeliveryPlayer();

	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);// override;

	// �`��
	void Render();

private:
	// �R���C�_�\�𐶐�
	void CreateCol();

	// 3d���f��
	CModel* mpModel;

	CCollider* mpBodyCol;
};