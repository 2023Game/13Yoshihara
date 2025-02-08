#pragma once
#include "CObjectBase.h"

class CModel;

class CDeliveryItem : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDeliveryItem();
	// �f�X�g���N�^
	~CDeliveryItem();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	// �ړ���ݒ�
	void SetMoveSpeed(CVector moveSpeed);

private:
	// �R���C�_�\�𐶐�
	void CreateCol();
	// 3d���f��
	CModel* mpModel;
	// �{�̃R���C�_�\
	CCollider* mpBodyCol;

	// �ړ�
	CVector mMoveSpeed;
	// �o�ߎ���
	float mElapsedTime;
};