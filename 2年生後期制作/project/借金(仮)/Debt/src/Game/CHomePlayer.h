#pragma once
#include "CPlayerBase.h"

/*
���_�̃v���C���[�L�����N�^�[
�v���C���[�x�[�X���p��
*/
class CHomePlayer : public CPlayerBase
{
public:
	// �R���X�g���N�^
	CHomePlayer();
	// �f�X�g���N�^
	~CHomePlayer();

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
	// �ҋ@���
	void UpdateIdle();
	// �ړ�����
	void UpdateMove();

	// �A�N�V�����̃L�[����
	void ActionInput();
	
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eMove,		// �ړ�


		Num
	};

	enum class EState
	{
		eIdle,			// �ҋ@
	};
	EState mState;
}; 
