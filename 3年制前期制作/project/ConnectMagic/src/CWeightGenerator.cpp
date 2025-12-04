#include "CWeightGenerator.h"
#include "CWeight.h"

CWeightGenerator::CWeightGenerator(const CVector& pos)
	: CSwitchObject(ETaskPriority::eBackground)
{
	mpModel = CResourceManager::Get<CModel>("Generator");
	Position(pos);

	// 最初重りはオフ
	mpWeight = new CWeight(pos);
	mpWeight->SetEnable(false);
}

CWeightGenerator::~CWeightGenerator()
{
}

// 作用していないときの処理
void CWeightGenerator::UpdateOff()
{
}

// 作用しているときの処理
void CWeightGenerator::UpdateOn()
{
	mpWeight->SetEnable(true);
}
