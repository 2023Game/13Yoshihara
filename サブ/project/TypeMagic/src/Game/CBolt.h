#pragma once
#include "CSpellBase.h"

// �{���g����
// �����x�[�X�N���X���p��
class CBolt : public CSpellBase
{
public:
	// �R���X�g���N�^
	CBolt(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target);
	// �f�X�g���N�^
	~CBolt();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// ���˒��̍X�V
	void UpdateShooting() override;
};