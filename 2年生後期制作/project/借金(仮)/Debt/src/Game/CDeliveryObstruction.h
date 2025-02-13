#pragma once
#include "CObjectBase.h"

class CModel;

// �z�B�̏�Q��
class CDeliveryObstruction : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDeliveryObstruction();
	// �f�X�g���N�^
	~CDeliveryObstruction();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;
private:
	// �R���C�_�\�𐶐�
	void CreateCol();
	// 3D���f��
	CModel* mpModel;
	// �{�̃R���C�_�\
	CCollider* mpBodyCol;
};