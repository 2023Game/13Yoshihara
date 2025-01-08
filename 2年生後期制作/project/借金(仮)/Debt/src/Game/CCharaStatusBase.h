#pragma once

class CCharaStatusBase
{
public:
	CCharaStatusBase(int maxHp, float baseMoveSpeed, 
		float jumpSpeed, int attackPower);
	~CCharaStatusBase();


	// �_���[�W���󂯂�
	virtual void TakeDamage(int damage, CObjectBase* causer);

	// ���S
	virtual void Death();
	// ����ł��邩�ǂ���
	bool IsDeath() const;

	// �ő�HP���擾
	int GetMaxHp() const;
	// ����HP���擾
	int GetHp() const;

	// ��b�ړ����x���擾
	float GetBaseMoveSpeed() const;

	// �W�����v���x���擾
	float GetJumpSpeed() const;

	// �U���͂��擾
	int GetAttackPower() const;

private:
	int mMaxHp;				// �ő�HP
	int mHp;				// ����HP
	float mBaseMoveSpeed;	// ��b�ړ����x
	float mJumpSpeed;		// �W�����v���x
	int mAttackPower;		// �U����
};