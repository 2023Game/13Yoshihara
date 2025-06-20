#pragma once

class CCharaStatusBase
{
public:
	CCharaStatusBase(int maxHp, int maxMp,
		float baseMoveSpeed, float jumpSpeed);
	~CCharaStatusBase();


	// �_���[�W���󂯂�
	virtual void TakeDamage(int damage, CObjectBase* attacker);

	// ���S
	virtual void Death();
	// ����ł��邩�ǂ���
	virtual bool IsDeath() const;

	// �ő�HP���擾
	int GetMaxHp() const;
	// �ő�HP��ݒ�
	void SetMaxHp(int maxHp);
	// ����HP���擾
	int GetHp() const;
	// ����HP�������Z����
	void SetHp(int num);
	// ����HP�����Z�b�g
	void ResetHp();

	// �ő�MP���擾
	int GetMaxMp() const;
	// �ő�MP��ݒ�
	void SetMaxMp(int maxMp);
	// ����MP���擾
	int GetMp() const;
	// ����MP�������Z����
	void SetMp(int num);
	// ����MP�����Z�b�g
	void ResetMp();

	// ��b�ړ����x���擾
	virtual float GetBaseMoveSpeed() const;
	// ��b�ړ����x��ݒ�
	void SetBaseMoveSpeed(float baseMoveSpeed);

	// �W�����v���x���擾
	float GetJumpSpeed() const;
	// �W�����v���x��ݒ�
	void SetJumpSpeed(float jumpSpeed);

private:
	int mMaxHp;				// �ő�HP
	int mHp;				// ����HP
	int mMaxMp;				// �ő�MP
	int mMp;				// ����MP
	float mBaseMoveSpeed;	// ��b�ړ����x
	float mJumpSpeed;		// �W�����v���x
};