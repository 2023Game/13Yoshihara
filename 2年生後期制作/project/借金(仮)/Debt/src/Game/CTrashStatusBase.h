#pragma once

class CTrashStatusBase
{
public:
	// �R���X�g���N�^
	CTrashStatusBase();
	~CTrashStatusBase();

	// �_���[�W��1�󂯂�
	void TakeDamage();

	// ����HP���擾
	int GetHp() const;
	// HP��ݒ�
	void SetHp(int hp);

	// �ړ����x���擾
	float GetBaseMoveSpeed() const;
	// �ړ����x��ݒ�
	void SetBaseMoveSpeed(float baseMoveSpeed);

	// �W�����v���x���擾
	float GetJumpSpeed() const;
	// �W�����v���x��ݒ�
	void SetJumpSpeed(float jumpSpeed);

	// �^����m�b�N�o�b�N���x���擾
	float GetKnockback() const;
	// �^����m�b�N�o�b�N���x��ݒ�
	void SetKnockback(float knockback);

	// �S�~�𗎂Ƃ��͂��擾
	int GetPower() const;
	// �S�~�𗎂Ƃ��͂�ݒ�
	void SetPower(int power);

	// �󂯂�m�b�N�o�b�N���x���擾
	float GetTakeKnockback() const;
	// �󂯂�m�b�N�o�b�N���x��ݒ�
	void SetTakeKnockback(float knockback);
private:
	int mHp;				// ����HP
	float mBaseMoveSpeed;	// ��b�ړ����x
	float mJumpSpeed;		// �W�����v���x
	float mKnockback;		// �m�b�N�o�b�N���x
	int mPower;				// �S�~�𗎂Ƃ���
	float mTakeKnockback;	// �󂯂�m�b�N�o�b�N���x
};