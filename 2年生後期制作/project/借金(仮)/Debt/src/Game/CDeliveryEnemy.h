#pragma once
#include "CEnemyBase.h"

/*
�z�B�Q�[���̓G�N���X
�G���N���X���p��
*/
class CDeliveryEnemy : public CEnemyBase
{
public:
	// �R���X�g���N�^
	CDeliveryEnemy();
	// �f�X�g���N�^
	~CDeliveryEnemy();

	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render();

private:
	// �R���C�_�\�𐶐�
	void CreateCol();

	// 3d���f��
	CModel* mpModel;
};