#pragma once
#include "CPlayerBase.h"

/*
���_�̃v���C���[�L�����N�^�[
�v���C���[�x�[�X���p��
*/
class CHomePlayer : public CPlayerBase
{
public:
	CHomePlayer();
	~CHomePlayer();

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
	
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eMove,		// �ړ�


		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	enum class EState
	{
		eIdle,			// �ҋ@
	};
	EState mState;

	CColliderLine* mpColliderLine;
}; 
