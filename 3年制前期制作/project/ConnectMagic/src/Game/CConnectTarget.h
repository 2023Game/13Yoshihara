#pragma once
#include "CObjectBase.h"

// �ڑ��ł���^�[�Q�b�g����
class CConnectTarget : public CObjectBase
{
public:
	// �R���X�g���N�^
	CConnectTarget();
	// �f�X�g���N�^
	~CConnectTarget();

	// �`��
	void Render() override;
};