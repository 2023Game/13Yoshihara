#pragma once
#include "CSpellBase.h"

// ���t���N�^�[����
// �����x�[�X�N���X���p��
class CReflector : public CSpellBase
{
public:
	// �R���X�g���N�^
	CReflector(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target);
	// �f�X�g���N�^
	~CReflector();

	// �X�V
	void Update() override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// ���˒��̍X�V
	void UpdateShooting() override;
};