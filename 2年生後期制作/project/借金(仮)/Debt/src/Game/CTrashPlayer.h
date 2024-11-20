#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CSound.h"

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CTrashPlayer : public CXCharacter
{
public:
	// �C���X�^���X�̃|�C���^�̎擾
	static CTrashPlayer* Instance();

	// �R���X�g���N�^
	CTrashPlayer();
	// �f�X�g���N�^
	~CTrashPlayer();

	// �X�V
	void Update();
	// �`��
	void Render();

protected:
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
		eIdleClose,			// �ҋ@					�i�Ɓj
		eIdleOpen,			// �ҋ@					�i�J�j
		eMoveClose,			// �ړ�					�i�Ɓj
		eMoveOpen,			// �ړ�					�i�J�j
		eDamageStartClose,	// ��e�J�n				�i�Ɓj
		eDamageStartOpen,	// ��e�J�n				�i�J�j
		eDamageClose,		// ��e�m�b�N�o�b�N		�i�Ɓj
		eDamageOpen,		// ��e�m�b�N�o�b�N		�i�J�j
		eDamageEndClose,	// ��e�I��				�i�Ɓj
		eDamageEndOpen,		// ��e�I��				�i�J�j
		eJumpStartClose,	// �W�����v�J�n			�i�Ɓj
		eJumpStartOpen,		// �W�����v�J�n			�i�J�j
		eJumpClose,			// �W�����v				�i�Ɓj
		eJumpOpen,			// �W�����v				�i�J�j
		eJumpEndClose,		// �W�����v�I��			�i�Ɓj
		eJumpEndOpen,		// �W�����v�I��			�i�J�j
		eAttackStartClose,	// �U���J�n				�i�Ɓj
		eAttackStartOpen,	// �U���J�n				�i�J�j
		eAttack,			// �U��					�i�J�j
		eAttackEnd,			// �U���I��				�i�J�j
		eCriticalStartClose,// �N���e�B�J���U���J�n	�i�Ɓj
		eCriticalStartOpen,	// �N���e�B�J���U���J�n	�i�J�j
		eCritical,			// �N���e�B�J���U��		�i�J�j
		eCriticalEnd,		// �N���e�B�J���U���I��	�i�J�j


		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �v���C���[�̃C���X�^���X
	static CTrashPlayer* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

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
	};
	EState mState;

	// �W���J���Ă��邩
	bool IsOpen;

	int mTest;
};
