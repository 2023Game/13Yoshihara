#pragma once
#include "CTextUI3D.h"

// �_���[�W�\�L��UI
class CDamageUI3D : public CTextUI3D
{
public:
	// �R���X�g���N�^
	CDamageUI3D(CVector pos);
	// �f�X�g���N�^
	~CDamageUI3D();

	// �X�V
	void Update() override;

private:
	// ���W���Ԃ炵�ĕԂ�
	CVector RandPos(const CVector& pos);

	// �����ŏ�����܂ł̎���
	float mAutoDeleteTime;
};