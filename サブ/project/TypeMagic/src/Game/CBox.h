#pragma once
#include "CConnectObject.h"

class CBox : public CConnectObject
{
public:
	// �R���X�g���N�^
	CBox();
	// �f�X�g���N�^
	~CBox();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// �L�����ƏՓ˔��������R���C�_�[
	CCollider* mpCharaCol;
};