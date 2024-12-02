#pragma once

class CTrashStatusBase
{
public:
	// �R���X�g���N�^
	CTrashStatusBase();
	~CTrashStatusBase();

	// �_���[�W��1�󂯂�
	void TakeDamage();
	// �m�b�N�o�b�N���󂯂�
	void SetTakeKnockBack(float knockback);
protected:
	// �X�e�[�^�X
	struct StatusBase
	{
		int mHp;			// ����HP
		float mMoveSpeed;	// �ړ����x
		float mJumpSpeed;	// �W�����v���x
		float mKnockback;	// �m�b�N�o�b�N���x
		int mPower;			// �S�~�𗎂Ƃ���
	};
	// �X�e�[�^�X
	StatusBase mStatus;

	// �󂯂�m�b�N�o�b�N���x
	float mTakeKnockback;
};