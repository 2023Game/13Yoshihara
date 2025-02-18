#include "CMoneyManager.h"

// �����̊Ǘ��N���X�̃C���X�^���X
CMoneyManager* CMoneyManager::spInstance = nullptr;

// �����̏�����
#define DEFAULT_MONEY 3000000
// �����̕ԍϊz
#define DEFAULT_DEBT_MONEY 1000

// �C���X�^���X���擾
CMoneyManager* CMoneyManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CMoneyManager();
	}
	return spInstance;
}

// �C���X�^���X��j��
void CMoneyManager::ClearInstance()
{
	SAFE_DELETE(spInstance);
}

// �R���X�g���N�^
CMoneyManager::CMoneyManager()
	: mMoney(DEFAULT_MONEY)
	, mDebtMoney(DEFAULT_DEBT_MONEY)
	, mNextDebtMoney(DEFAULT_DEBT_MONEY)
	, mDay(0)
	, mDid(false)
{
	// ���̕ԍϊz��1���ڂɐݒ�
	SetNextDebtMoney(1);
}

// �f�X�g���N�^
CMoneyManager::~CMoneyManager()
{
	// �C���X�^���X���폜
	spInstance = nullptr;
}

// ���������擾����
int CMoneyManager::GetMoney() const
{
	return mMoney;
}

// ��������ݒ肷��
void CMoneyManager::SetMoney(int money)
{
	mMoney = money;
}

// �ԍϊz���擾����
int CMoneyManager::GetDebtMoney() const
{
	return mDebtMoney;
}

// �w�肵�������̕ԍϊz��ݒ肷��
void CMoneyManager::SetDebtMoney(int day)
{
	mDebtMoney += mDebtMoney * (day + 1);
	mNextDebtMoney = mDebtMoney;
	SetNextDebtMoney(day + 1);
}

// ���̕ԍϊz���擾����
int CMoneyManager::GetNextDebtMoney() const
{
	return mNextDebtMoney;
}

// �w�肵�������̗����̕ԍϊz�����̕ԍϊz�ɐݒ肷��
void CMoneyManager::SetNextDebtMoney(int day)
{
	mNextDebtMoney += mNextDebtMoney * (day + 1);
}

// �������擾����
int CMoneyManager::GetDay() const
{
	return mDay;
}

// ������ݒ肷��
void CMoneyManager::SetDay(int day)
{
	mDay = day;
}

// �������o�߂�����
void CMoneyManager::DayPass()
{
	mDay++;
}

// �ԍς�����
bool CMoneyManager::GetDid() const
{
	return mDid;
}

// �ԍς�������ݒ肷��
void CMoneyManager::SetDid(bool isDid)
{
	mDid = isDid;
}
