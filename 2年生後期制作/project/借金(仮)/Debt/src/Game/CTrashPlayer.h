#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CPlayerBase.h"
#include "CTrashPlayerStatus.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CSound.h"

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CTrashPlayer : public CPlayerBase, public CTrashPlayerStatus
{
public:
	// �R���X�g���N�^
	CTrashPlayer();
	// �f�X�g���N�^
	~CTrashPlayer();

	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

protected:
	// �A�N�V�����̃L�[����
	void ActionInput();
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
	// �N���e�B�J���U���J�n
	void UpdateCriticalStart();
	// �N���e�B�J���U����
	void UpdateCritical();
	// �N���e�B�J���U���I��
	void UpdateCriticalEnd();
	// �W���J����
	void UpdateOpenClose();

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

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
		eDamage_Close,		// ��e�m�b�N�o�b�N		�i�Ɓj
		eDamage_Close_End,	// ��e�I��				�i�Ɓj
		eDamage_Open_Start,	// ��e�J�n				�i�J�j
		eDamage_Open,		// ��e�m�b�N�o�b�N		�i�J�j
		eDamage_Open_End,	// ��e�I��				�i�J�j
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


		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

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
	};
	// �X�e�[�^�X�؂�ւ�
	void ChangeState(EState state);
	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
	EState mState;	// �v���C���[�̏��



	// �W���J���Ă��邩
	bool mIsOpen;
	// �W�����v���Ă��邩
	bool mIsJump;
};
