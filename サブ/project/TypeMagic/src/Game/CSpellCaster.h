#pragma once
#include "SpellType.h"

class CSpellBase;

// ���ۂɎ����𔭓���������r���N���X
class CSpellCaster
{
public:
	// �ΐ푊���ݒ�
	void SetOpponent(CObjectBase* opponent);
	// �ΐ푊����擾
	CObjectBase* GetOpponent() const;

	// �r���������擾
	bool IsCasting() const;

	// �r���J�n
	bool CastStart(ESpellElementalType elemental,
		ESpellShapeType shape);
	// �r���I��
	void CastEnd();

	// �V�[���h��\���Ă��邩��ݒ�
	void SetIsShield(bool enable);

protected:
	// �R���X�g���N�^
	CSpellCaster(CObjectBase* owner);
	// �f�X�g���N�^
	~CSpellCaster();
	
	// �X�V
	void Update();

	// �ΐ푊��
	CObjectBase* mpOpponent;

	// �V�[���h��\���Ă��邩
	bool mIsShield;

	// �r�����̎����̌`
	ESpellShapeType mSpellShape;

private:
	// �{�[�������̉r��
	void CastBall();
	// �{���g�����̉r��
	void CastBolt();
	// �u���X�����̉r��
	void CastBreath();
	// �e���|�[�g�����̉r��
	void CastTeleport();
	// �V�[���h�����̉r��
	void CastShield();
	// ���t���N�^�[�����̉r��
	void CastReflector();

	// �������x�Ƒҋ@���ԂƏ���MP�̐ݒ�
	void SetTimeAndMp(ESpellElementalType elemental, ESpellShapeType shape);

	// �����ҋ@
	bool WaitGenerate();
	// ���ˑҋ@
	bool WaitShoot();
	// ����
	bool Shoot();

	// �r�����̎����̑���
	ESpellElementalType mSpellElemental;

	// �r������
	bool mIsSpellCasting;
	// �X�e�b�v
	int mStep;
	// �o�ߎ���
	float mElapsedTime;

	// �����Ԋu
	float mGenerateInterval;
	// �ҋ@����
	float mIdleTime;
	// ����MP
	int mUseMp;

	// �r����������
	std::vector<CSpellBase*> mSpells;

	// ������
	int mGenerateNum;

	// ���̃N���X�̎�����
	CObjectBase* mpOwner;
};