#pragma once
//�v���C���[���N���X�̃C���N���[�h
#include "CPlayerBase.h"
// �v���C���[�̃X�e�[�^�X�N���X�̃C���N���[�h
#include "CPlayerStatus.h"
#include "CCastSpellStr.h"

class CSound;
class CImage;
class CCastSpellStr;

/*
�Q�[���̃v���C���[�N���X
�v���C���[���N���X��
�v���C���[�̃X�e�[�^�X�N���X��
�����r���N���X���p��
*/
class CPlayer : public CPlayerBase ,  public CPlayerStatus, public CCastSpellStr
{
public:
	// �R���X�g���N�^
	CPlayer();
	// �f�X�g���N�^
	~CPlayer();

	// �X�V
	void Update();

	// �v���C���[���r����Ԃ�
	bool IsCastState() const;

private:
	// �R���C�_�\�𐶐�
	void CreateCol() override;
	/*
	�A�N�V�����̃L�[����
	����������Ă��Ȃ��Ƃ��̂ݓ��͉\
	*/
	void ActionInput();
	// �r���̃L�[����
	void CastInput();
	// ��{�r���̃L�[����
	void BasicCastInput();
	// �Z�k�r���̃L�[����
	void QuickCastInput();

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
		eDamageStart,	// ��e�J�n
		eDamage,		// ��e�m�b�N�o�b�N
		eDamageEnd,		// ��e�I��
		eCast,			// �r����
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

	// MP�Q�[�W
	CGaugeUI2D* mpMpGauge;
};
