#pragma once
//�v���C���[���N���X�̃C���N���[�h
#include "CPlayerBase.h"
// �v���C���[�̃X�e�[�^�X�N���X�̃C���N���[�h
#include "CTrashPlayerStatus.h"
#include "CCollector.h"

/*
�S�~�E���Q�[���̃v���C���[�N���X
�v���C���[���N���X��
�v���C���[�̃X�e�[�^�X�N���X���p��
*/
class CTrashPlayer : public CPlayerBase , public CTrashPlayerStatus
{
public:
	// �R���X�g���N�^
	CTrashPlayer();
	// �f�X�g���N�^
	~CTrashPlayer();

	// �U������
	bool IsAttacking() const override;
	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;

	// �_���[�W���󂯂�
	void TakeDamage(int damage, CObjectBase* causer) override;
	// �N���e�B�J���_���[�W���󂯂�
	void TakeCritical(int damage, CObjectBase* causer);

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

	// ����������Ă��邩���擾
	bool GetStickCollector() const;
	// ����������Ă��邩��ݒ�
	void SetStickCollector(bool stickCollector);
	// ���Ă��������̃|�C���^���擾
	CCollector* GetStickCollectorPointer() const;
	// ���Ă��������̃|�C���^��ݒ�
	void SetStickCollectorPointer(CCollector* collector);

	// �J���Ă��邩���擾
	bool GetOpen() const;

private:
	/*
	�A�N�V�����̃L�[����
	����������Ă��Ȃ��Ƃ��̂ݓ��͉\
	*/
	void ActionInput();

	/*
	�A�j���[�V�����̎��
	(��)���W�����Ă����Ԃ���̃A�j���[�V����
	(�J)���W���J���Ă����Ԃ���̃A�j���[�V����
	(�J��)�������̏�Ԍ��p�̃A�j���[�V����
	*/
	enum class EAnimType
	{
		None = -1,

		eTPose,				// T�|�[�Y
		eIdle_Close,		// �ҋ@					�i�Ɓj
		eIdle_Open,			// �ҋ@					�i�J�j
		eMove_Close,		// �ړ�					�i�Ɓj
		eMove_Open,			// �ړ�					�i�J�j
		eDamage_Close_Start,// ��e�J�n				�i�Ɓj
		eDamage_Open_Start,	// ��e�J�n				�i�J�j
		eDamage,			// ��e�m�b�N�o�b�N		�i�J�j
		eDamage_End,		// ��e�I��				�i�J�j
		eJump_Close_Start,	// �W�����v�J�n			�i�Ɓj
		eJump_Close,		// �W�����v��			�i�Ɓj
		eJump_Close_End,	// �W�����v�I��			�i�Ɓj
		eJump_Open_Start,	// �W�����v�J�n			�i�J�j
		eJump_Open,			// �W�����v��			�i�J�j
		eJump_Open_End,		// �W�����v�I��			�i�J�j
		eAttack_Start,		// �U���J�n				�i�J�j
		eAttack,			// �U����				�i�J�j
		eAttack_End,		// �U���I��				�i�J�j
		eCritical_Start,	// �N���e�B�J���U���J�n	�i�J�j
		eCritical,			// �N���e�B�J���U����	�i�J�j
		eCritical_End,		// �N���e�B�J���U���I��	�i�J�j
		eOpen,				// �W���J��				�i�Ɓj
		eClose,				// �W�����			�i�J�j
		eDeath,				// ���S					�i�J�j


		Num
	};

	// �v���C���[�̏��
	enum class EState
	{
		eIdle,			// �ҋ@
		eMove,			// �ړ�
		eDamageStart,	// ��e�J�n
		eDamage,		// ��e�m�b�N�o�b�N
		eDamageEnd,		// ��e�I��
		eJumpStart,		// �W�����v�J�n
		eJump,			// �W�����v
		eJumpEnd,		// �W�����v�I��
		eAttackStart,	// �U���J�n
		eAttack,		// �U��
		eAttackEnd,		// �U���I��
		eCriticalStart,	// �N���e�B�J���U���J�n
		eCritical,		// �N���e�B�J���U��
		eCriticalEnd,	// �N���e�B�J���U���I��
		eOpenClose,		// �W���J����
		eDeath,			// ���S
	};

	// �ҋ@���
	void UpdateIdle();
	// �ړ�����
	void UpdateMove();
	// ��e�J�n
	void UpdateDamageStart();
	// ��e�m�b�N�o�b�N
	void UpdateDamage();
	// ��e�I��
	void UpdateDamageEnd();
	// �W�����v�J�n
	void UpdateJumpStart();
	// �W�����v��
	void UpdateJump();
	// �W�����v�I��
	void UpdateJumpEnd();
	// �U���J�n
	void UpdateAttackStart();
	// �U����
	void UpdateAttack();
	// �U���I��
	void UpdateAttackEnd();
	// �N���e�B�J���J�n
	void UpdateCriticalStart();
	// �N���e�B�J����
	void UpdateCritical();
	// �N���e�B�J���I��
	void UpdateCriticalEnd();
	// �W���J����
	void UpdateOpenClose();
	// ���S�̍X�V����
	void UpdateDeath();
	// ���S
	void Death() override;

	// ��Ԑ؂�ւ�
	void ChangeState(EState state);
	EState mState;	// �v���C���[�̏��
	int mStateStep;				// ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime;			// �o�ߎ��Ԍv���p

#if _DEBUG
	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
#endif

	// �N���e�B�J���U���R���C�_�\
	CCollider* mpCriticalCol;

	// �W���J���Ă��邩
	bool mIsOpen;
	// �W�����v���Ă��邩
	bool mIsJump;
	// ����������Ă��邩�ǂ���
	bool mIsStickCollector;
	// ���Ă��������̃|�C���^
	CCollector* mpStickCollector;
};
