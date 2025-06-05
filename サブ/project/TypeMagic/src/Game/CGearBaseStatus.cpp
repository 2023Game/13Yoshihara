#include "CGearBaseStatus.h"

// �R���X�g���N�^
CGearBaseStatus::CGearBaseStatus(ERare rare, std::string name)
	: mRare(rare)
	, mName(name)
{
}

// �f�X�g���N�^
CGearBaseStatus::~CGearBaseStatus()
{
}

// ���A�x���擾����
CGearBaseStatus::ERare CGearBaseStatus::GetRare()
{
	return mRare;
}

// ���������擾����
std::string CGearBaseStatus::GetName()
{
	return mName;
}

// �X�e�[�^�X���擾����
std::map<std::string, int> CGearBaseStatus::GetStatus()
{
	return mStatus;
}
