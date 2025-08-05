#pragma once
#include "CObjectBase.h"

class CImage3D;

// �������̕����ɍs���Ȃ��悤�ɂ���V�[���h
class CShield : public CObjectBase
{
public:
	// �R���X�g���N�^
	CShield();
	// �f�X�g���N�^
	~CShield();

	// �X�V
	void Update() override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
	// �R���C�_�[
	CCollider* mpCol;

	// �摜�𐶐�
	void CreateImg();
	// �V�[���h�̉摜
	CImage3D* mpImg;
};