#pragma once
#include "CSwitchObject.h"

class CWeight;

// スイッチによって重りを生成する
class CWeightGenerator : public CSwitchObject
{
public:
	CWeightGenerator(const CVector& pos);
	~CWeightGenerator();

private:
	// 作用していないときの処理
	void UpdateOff() override;
	// 作用しているときの処理
	void UpdateOn() override;

	CWeight* mpWeight;
};