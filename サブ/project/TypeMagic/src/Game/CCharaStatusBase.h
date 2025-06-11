#pragma once

class CCharaStatusBase
{
public:
	CCharaStatusBase(int maxHp, float baseMoveSpeed, 
		float jumpSpeed, int attackPower);
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


	// ��b�ړ����x���擾
	virtual float GetBaseMoveSpeed() const;
	// ��b�ړ����x��ݒ�
	void SetBaseMoveSpeed(float baseMoveSpeed);

	// �W�����v���x���擾
	float GetJumpSpeed() const;
	// �W�����v���x��ݒ�
	void SetJumpSpeed(float jumpSpeed);

	// �U���͂��擾
	int GetAttackPower() const;
	// �U���͂�ݒ�
	void SetAttackPower(int attackPower);

private:
	int mMaxHp;				// �ő�HP
	int mHp;				// ����HP
	float mBaseMoveSpeed;	// ��b�ړ����x
	float mJumpSpeed;		// �W�����v���x
	int mAttackPower;		// �U����
};