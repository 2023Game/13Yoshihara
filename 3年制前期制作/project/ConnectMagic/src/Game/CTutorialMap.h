#pragma once
#include "CFieldBase.h"

// �`���[�g���A���}�b�v
class CTutorialMap : public CFieldBase
{
public:
	// �R���X�g���N�^
	CTutorialMap();
	// �f�X�g���N�^
	~CTutorialMap();

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
};