#include "CMoneyManager.h"

// �����̊Ǘ��N���X�̃C���X�^���X
CMoneyManager* CMoneyManager::spInstance = nullptr;

// �����̏�����
#define DEFAULT_MONEY 5000
// �����̕ԍϊz
#define DEFAULT_DEBT_MONEY 3000

// �Q�[���N���A�̓���
#define CLEAR_DAY 2

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
	: mPreMoney(DEFAULT_MONEY)
	, mMoney(DEFAULT_MONEY)
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

// �O�̓��̏�������ݒ肷��
void CMoneyManager::SetPreMoney(int money)
{
	mPreMoney = money;
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
	mDebtMoney = DEFAULT_DEBT_MONEY * (day * day + 1);
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
	mNextDebtMoney = DEFAULT_DEBT_MONEY * (day * day + 1);
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

// 1�������[���o�b�N����
void CMoneyManager::Rollback()
{
	// �O�̓��̏������ɐݒ�
	mMoney = mPreMoney;
	// ������1���߂�
	SetDay(GetDay() - 1);
	// 1���߂��������̎؋��z��ݒ�
	SetDebtMoney(GetDay());
	// �ԍς��Ă��Ȃ�
	SetDid(false);
}

// �Q�[���N���A���ǂ���
bool CMoneyManager::IsClear()
{
	// �N���A�̓����ȍ~�Ȃ�
	if (mDay >= CLEAR_DAY)
	{
		// �N���A
		return true;
	}
	// �����łȂ��ꍇ�N���A���Ă��Ȃ�
	return false;
}
