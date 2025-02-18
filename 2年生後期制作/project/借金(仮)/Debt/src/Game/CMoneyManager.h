#pragma once

// �����̕n�x�̏������̗�
#define POOR_MONEY 5000		// �n�R
#define RICH_MONEY 50000	// �T��

/*
�����̊Ǘ��N���X
*/
class CMoneyManager
{
public:
	// �C���X�^���X���擾
	static CMoneyManager* Instance();
	// �C���X�^���X��j��
	static void ClearInstance();

	// �R���X�g���N�^
	CMoneyManager();
	// �f�X�g���N�^
	~CMoneyManager();

	// ���������擾����
	int GetMoney() const;
	// ��������ݒ肷��
	void SetMoney(int money);

	// �ԍϊz���擾����
	int GetDebtMoney() const;
	// �w�肵�������̕ԍϊz��ݒ肷��
	void SetDebtMoney(int day);

	// ���̕ԍϊz���擾����
	int GetNextDebtMoney() const;
	// �w�肵�������̗����̕ԍϊz�����̕ԍϊz�ɐݒ肷��
	void SetNextDebtMoney(int day);

	// �������擾����
	int GetDay() const;
	// ������ݒ肷��
	void SetDay(int day);
	// �������o�߂�����
	void DayPass();

	// �ԍς�����
	bool GetDid() const;
	// �ԍς�������ݒ肷��
	void SetDid(bool isDid);

private:
	// �����̊Ǘ��N���X�̃C���X�^���X
	static CMoneyManager* spInstance;

	// ������
	int mMoney;
	// �ԍϊz
	int mDebtMoney;
	// ���̕ԍϊz
	int mNextDebtMoney;
	// ����
	int mDay;
	// �ԍς�������
	bool mDid;
};