#pragma once

class CTrashStatusBase
{
private:
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

public:
	// �R���X�g���N�^
	CTrashStatusBase();
	~CTrashStatusBase();

	// �_���[�W��1�󂯂�
	void TakeDamage();

	// �m�b�N�o�b�N�̒l���擾
	float GetTakeKnockback();
	// �m�b�N�o�b�N���󂯂�
	void SetTakeKnockback(float knockback);

	// �x�[�X�X�e�[�^�X���擾����
	StatusBase GetStatusBase();
	// �x�[�X�X�e�[�^�X��ݒ肷��
	void SetStatusBase(int hp, float moveSpeed, float jumpSpeed, float knockback, int power);
};