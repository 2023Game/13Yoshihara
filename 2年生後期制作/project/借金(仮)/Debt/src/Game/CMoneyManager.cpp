#include "CMoneyManager.h"

// �����̊Ǘ��N���X�̃C���X�^���X
CMoneyManager* CMoneyManager::spInstance = nullptr;

// �����̏�����
#define DEFAULT_MONEY 3000

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
{
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

// �����������Z����
void CMoneyManager::SetMoney(int money)
{
	mMoney += money;
	// 0��菬�����Ȃ�����0�ɂ���
	if (mMoney < 0)
	{
		mMoney = 0;
	}
}
