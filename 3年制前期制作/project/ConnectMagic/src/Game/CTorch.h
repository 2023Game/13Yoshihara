#pragma once
#include "CFire.h"

// �����N���X
class CTorch : public CFire
{
public:
	// �R���X�g���N�^
	CTorch();
	// �f�X�g���N�^
	~CTorch();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
};