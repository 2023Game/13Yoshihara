#pragma once
#include "CConnectObject.h"

// �󒆂ɕ�����ł���ڑ��I�u�W�F�N�g
class CAirConnectObj : public CConnectObject
{
public:
	// �R���X�g���N�^
	CAirConnectObj();
	// �f�X�g���N�^
	~CAirConnectObj();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
};