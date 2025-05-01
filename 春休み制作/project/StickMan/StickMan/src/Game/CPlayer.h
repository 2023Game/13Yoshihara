#pragma once
//�v���C���[���N���X�̃C���N���[�h
#include "CPlayerBase.h"
// �v���C���[�̃X�e�[�^�X�N���X�̃C���N���[�h
#include "CPlayerStatus.h"

class CSound;
class CWand;
class CConnectPoint;
class CConnectObject;

/*
�Q�[���̃v���C���[�N���X
�v���C���[���N���X��
�v���C���[�̃X�e�[�^�X�N���X���p��
*/
class CPlayer : public CPlayerBase ,  public CPlayerStatus
{
public:
	// �R���X�g���N�^
	CPlayer();
	// �f�X�g���N�^
	~CPlayer();

	// �U������
	bool IsAttacking() const override;
	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;

	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W</param>
	/// <param name="causer">�U�����Ă�������</param>
	void TakeDamage(int damage, CObjectBase* causer) override;

	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// ��̃|�C���^���擾
	CWand* GetWand();

private:
	// �R���C�_�\�𐶐�
	void CreateCol() override;
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

		eIdle,		// �ҋ@
		eIdle_Wand,	// �񎝂��ҋ@
		eMove,		// �ړ�
		eMove_Wand,	// �񎝂��ړ�
		eJump,		// �W�����v
		eAttack,	// �U����

		eDeath,		// ���S


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

	// �W�����v���Ă��邩
	bool mIsJump;
	// ��������Ă��邩
	bool mIsWand;
	// �U������
	bool mIsAttacking;

	CWand* mpWand;			// ��
	CConnectPoint* mpPoint;	// �ڑ���

	// �˒����ɂ���R�l�N�g�I�u�W�F�N�g�̃��X�g
	std::list<CConnectObject*> mConnectObjs;
	// �R�l�N�g�I�u�W�F�N�g�̒T�m�p
	CCollider* mpSearchConnectObjCol;

	// ���_�̒��S�Ɉ�ԋ߂��I�u�W�F�N�g�����߂�
	CConnectObject* CenterObject();
};
