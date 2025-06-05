#pragma once
#include "CSpellBase.h"
#include "CSpellStatus.h"

// �u���X����
// �����x�[�X�N���X���p��
class CBreath : public CSpellBase
{
public:
	// �R���X�g���N�^
	CBreath(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target);
	// �f�X�g���N�^
	~CBreath();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// ���˒��̍X�V
	void UpdateShooting() override;
};