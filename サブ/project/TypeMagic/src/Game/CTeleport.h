#pragma once
#include "CSpellBase.h"

// �e���|�[�g����
// �����x�[�X�N���X���p��
class CTeleport : public CSpellBase
{
public:
	// �R���X�g���N�^
	CTeleport(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target);
	// �f�X�g���N�^
	~CTeleport();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// �ҋ@���̍X�V
	void UpdateIdle() override;
	// ���˒��̍X�V
	void UpdateShooting() override;
};