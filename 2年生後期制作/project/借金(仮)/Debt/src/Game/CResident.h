#pragma once
#include "CEnemyBase.h"
#include "CResidentStatus.h"

/*
�S�~�E���Q�[���̏Z�l�N���X
�G���N���X��
�Z�l�̃X�e�[�^�X�N���X���p��
*/
class CResident : public CEnemyBase, public CResidentStatus
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model">�Z�l�̃��f��</param>
	/// <param name="patrolPoints">�������̍��W</param>
	/// <param name="patrolPoints">����|�C���g�̃��X�g</param>
	CResident(CModelX* model, CVector startPos,
		std::vector<CNavNode*> patrolPoints);
	// �f�X�g���N�^
	~CResident();

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
private:
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eMove,		// �ړ�
		eThrowBag,	// �S�~�܂𓊂���


		Num
	};

	// �G�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		ePatrol,	// ����
		eThrowBag,	// �S�~�܂𓊂���
	};

	// �ҋ@���
	void UpdateIdle();
	// ���񏈗�
	void UpdatePatrol();
	// �S�~�܂𓊂��鏈��
	void UpdateThrowBag();

	// ��Ԑ؂�ւ�
	void ChangeState(EState state);
	EState mState;	// �G�̏��
	int mStateStep;	// ��ԓ��̃X�e�b�v��
	float mElapsedTime;	// �o�ߎ���

#if _DEBUG
	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
	// ��Ԃ̐F���擾
	CColor GetStateColor(EState state) const;
#endif

	// �ړ��𒆒f�������ǂ���
	bool mIsMovePause;
};