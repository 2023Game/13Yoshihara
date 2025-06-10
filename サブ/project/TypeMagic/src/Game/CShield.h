#pragma once
#include "CSpellBase.h"

// �V�[���h����
// �����x�[�X�N���X���p��
class CShield : public CSpellBase
{
public:
	// �R���X�g���N�^
	CShield(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target);
	// �f�X�g���N�^
	~CShield();

	// �X�V
	void Update() override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// ���˒��̍X�V
	void UpdateShooting() override;

	// ���݂̊p�x
	float mCurrentAngle;
};