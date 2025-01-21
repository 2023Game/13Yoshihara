#pragma once
#include "CEnemyBase.h"
#include "CCollectorStatus.h"

/*
�S�~���W�Ԃ��琶�������
������̃N���X
�G���N���X��
*/
class CCollector : public CEnemyBase , public CCollectorStatus
{
public :
	/// <summary>
	///	�R���X�g���N�^
	/// </summary>
	/// <param name="punisher">true�Ȃ�΁A���d�u���p</param>
	/// <param name="owner">������̎�����</param>
	/// <param name="patrolPoints">����|�C���g�̃��X�g</param>
	CCollector(bool punisher, CObjectBase* owner,
		std::vector<CNavNode*> patrolPoints);
	// �f�X�g���N�^
	~CCollector();

	// �U������
	bool IsAttacking() const override;
	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;
	// �U��������������
	bool GetAttackSuccess() const;

	// �_���[�W���󂯂�
	void TakeDamage(int damage, CObjectBase* causer) override;
	// ����ł��邩�ǂ���
	bool IsDead() override;

	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render() override;

	// ������̗L��������؂�ւ���
	void SetOnOff(bool setOnOff);

	// �P����Ԃɕς���
	void ChangeStateReturn();

	// ������̎������ݒ�
	void SetOwner(CObjectBase* owner);
	// ������̎�������擾
	CObjectBase* GetOwner() const;
private:
	/*
�A�j���[�V�����̎��
(��)���W�����Ă����Ԃ���̃A�j���[�V����
(�J)���W���J���Ă����Ԃ���̃A�j���[�V����
(�J��)�������̏�Ԍ��p�̃A�j���[�V����
*/
	enum class EAnimType
	{
		None = -1,

		eTPose,					// T�|�[�Y
		eIdle,					// �ҋ@				�i���j
		eIdle_Bag,				// �ҋ@				�i�L�j
		eMove,					// �ړ�				�i���j
		eMove_Bag,				// �ړ�				�i�L�j
		eGetBag,				// �S�~�܊l��		�i���j
		eGetBag_End,			// �S�~�܊l���I��	�i�L�j
		eDeath,					// ���S				�i�L���j
		eAttack_Start,			// �U���J�n			�i���j
		eAttack_True,			// �U������			�i���j
		eAttack_False,			// �U�����s			�i���j
		eAttack_False_Fall,		// �U�����s������	�i���j
		eAttack_False_StandUp,	// �����オ��		�i���j


		Num
	};

	// �G�̏��
	enum class EState
	{
		eIdle,			// �ҋ@
		ePatrol,		// ����
		eChase,			// �ǐ�
		eLost,			// ������
		eReturn,		// �S�~���W�Ԃɖ߂�
		eAttackStart,	// �U���J�n
		eAttackTrue,	// �U�����i�����j
		eAttackFalse,	// �U�����i���s�j
		eAttackEnd,		// �U���I��
		eDeath,			// ���S
	};

	// �ҋ@���
	void UpdateIdle();
	// ���񏈗�
	void UpdatePatrol();
	// �ǐՏ���
	void UpdateChase();
	// ����������
	void UpdateLost();
	// �S�~���W�Ԃɖ߂鏈��
	void UpdateReturn();
	// �U���J�n
	void UpdateAttackStart();
	// �U�����i�����j
	void UpdateAttackTrue();
	// �U�����i���s�j
	void UpdateAttackFalse();
	// �U���I��
	void UpdateAttackEnd();
	// ���S�̍X�V����
	void UpdateDeath();
	// ���S����
	void Death() override;

	// ��Ԑ؂�ւ�
	void ChangeState(EState state);
	EState mState;	// ������̏��
	int mStateStep;	// ��ԓ��̃X�e�b�v��
	float mElapsedTime;	// �o�ߎ���

#if _DEBUG
	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
	// ��Ԃ̐F���擾
	CColor GetStateColor(EState state) const;
#endif

	bool mIsBag;	// �S�~�܂������Ă��邩�ǂ���
	// �U���������������ǂ���
	bool mIsAttackSuccess;		
	// �U�������ǂ���
	bool mIsAttacking;

	// ������̎�����
	CObjectBase* mpOwner;
};