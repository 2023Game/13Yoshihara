#pragma once
#include "CObjectBase.h"

// �I�u�W�F�N�g�����̕����ɍs���Ȃ��悤�ɂ���V�[���h
class CShield : public CObjectBase
{
public:
	// �R���X�g���N�^
	CShield();
	// �f�X�g���N�^
	~CShield();

	// �`��
	void Render() override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
	// �R���C�_�[
	CCollider* mpCol1;
	CCollider* mpCol2;
};