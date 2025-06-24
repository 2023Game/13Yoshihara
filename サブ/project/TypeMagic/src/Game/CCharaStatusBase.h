#pragma once

class CCharaStatusBase
{
public:
	CCharaStatusBase(int maxHp, int maxMp, int regeneMp,
		float baseMoveSpeed, float jumpSpeed);
	~CCharaStatusBase();


	// �_���[�W���󂯂�
	virtual void TakeDamage(int damage, CObjectBase* attacker);
	// MP������

	/// <summary>
	/// MP���g�p
	/// </summary>
	/// <param name="useMp">�g�p����MP</param>
	/// <returns>�g�p������������</returns>
	bool UseMp(int useMp);

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
	float GetMaxMp() const;
	// �ő�MP��ݒ�
	void SetMaxMp(float maxMp);
	// ����MP���擾
	float GetMp() const;
	// ����MP�������Z����
	void SetMp(float num);
	// ����MP�����Z�b�g
	void ResetMp();
	// MP�̍Đ�
	void RegeneMp();

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
	float mMaxMp;			// �ő�MP
	float mMp;				// ����MP
	float mRegeneMp;			// MP��1�b������̍Đ����l
	float mBaseMoveSpeed;	// ��b�ړ����x
	float mJumpSpeed;		// �W�����v���x
};