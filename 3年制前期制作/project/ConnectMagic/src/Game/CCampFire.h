#pragma once
#include "CFire.h"
#include "CItemDrop.h"

class CCampFire : public CFire , public CItemDrop
{
public:
	// �R���X�g���N�^
	CCampFire();
	// �f�X�g���N�^
	~CCampFire();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
	// �R�����Ƃ��̏���
	void Burning() override;
};