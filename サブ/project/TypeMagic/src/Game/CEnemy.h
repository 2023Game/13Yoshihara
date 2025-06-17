#pragma once
#include "CEnemyBase.h"
#include "CCastSpellStr.h"

// �G�̊��N���X��
// �����r���N���X���p�������G�N���X
class CEnemy : public CEnemyBase, public CCastSpellStr
{
public:
	// �R���X�g���N�^
	CEnemy();
	// �f�X�g���N�^
	~CEnemy();

	// �X�V
	void Update() override;

	// ���C��������ݒ�
	void SetMainElemental(ESpellElementalType elemental);

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;

	// �G�̏��
	enum class EState
	{
		eIdle,	// �ҋ@
		eCast,	// �r����
		eChase,	// �ǂ�������
		eDodge,	// ���
		eRun,	// ������
	};

	// �ҋ@���
	void UpdateIdle();
	// �ړ�����
	void UpdateMove();
	// �r����
	void UpdateCast();
	// �ǂ�������
	void UpdateChase();
	// ���
	void UpdateDodge();
	// ������
	void UpdateRun();

	// ��Ԑ؂�ւ�
	void ChangeState(EState state);
	EState mState;	// �G�̏��
	int mStateStep;	// ��ԓ��̃X�e�b�v�Ǘ��p
	// �o�ߎ���
	float mElapsedTime;

	// ���C������
	ESpellElementalType mMainElement;
};