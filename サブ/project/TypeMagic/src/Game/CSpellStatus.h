#pragma once
#include "SpellType.h"

// �����̃X�e�[�^�X
struct SpellStatus
{
	ESpellElementalType elemental;	// ����
	ESpellShapeType shape;			// �`
	int power;				// �U����
	float speed;			// ���x
	int hp;					// �ϋv
};

// �����̃X�e�[�^�X
class CSpellStatus
{
public:
	// �R���X�g���N�^
	CSpellStatus(ESpellElementalType elemental, ESpellShapeType shape);
	// �f�X�g���N�^
	~CSpellStatus();

	// �����̃X�e�[�^�X���擾
	SpellStatus GetSpellStatus() const;

	// ���x��ݒ�
	void SetSpeed(float speed);

protected:
	// �����̃X�e�[�^�X
	SpellStatus mSpellStatus;

private:
	// �X�e�[�^�X�ݒ�
	void SetStatus(ESpellElementalType elemental, ESpellShapeType shape);
};