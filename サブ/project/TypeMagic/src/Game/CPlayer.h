#pragma once
//�v���C���[���N���X�̃C���N���[�h
#include "CPlayerBase.h"
// �v���C���[�̃X�e�[�^�X�N���X�̃C���N���[�h
#include "CPlayerStatus.h"
#include "CSpellCaster.h"

class CSound;
class CImage;

/*
�Q�[���̃v���C���[�N���X
�v���C���[���N���X��
�v���C���[�̃X�e�[�^�X�N���X��
�����r���N���X���p��
*/
class CPlayer : public CPlayerBase ,  public CPlayerStatus, public CSpellCaster
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

	// �U������
	bool mIsAttacking;
};
