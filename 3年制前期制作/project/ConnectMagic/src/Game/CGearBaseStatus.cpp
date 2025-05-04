#include "CGearBaseStatus.h"

// コンストラクタ
CGearBaseStatus::CGearBaseStatus(ERare rare, std::string name)
	: mRare(rare)
	, mName(name)
{
}

// デストラクタ
CGearBaseStatus::~CGearBaseStatus()
{
}

// レア度を取得する
CGearBaseStatus::ERare CGearBaseStatus::GetRare()
{
	return mRare;
}

// 装備名を取得する
std::string CGearBaseStatus::GetName()
{
	return mName;
}

// ステータスを取得する
std::map<std::string, int> CGearBaseStatus::GetStatus()
{
	return mStatus;
}
