#pragma once
#include "CSwitchObject.h"

class CWater;

// スイッチで水位が下げられる水クラス
class CSwitchWater : public CSwitchObject
{
public:
	CSwitchWater(const CVector& pos, const CVector& downPos, const CVector& scale);
	~CSwitchWater();

private:
	// 作用していないときの更新
	void UpdateOff() override;
	// 作用しているときの更新
	void UpdateOn() override;

	void SetEnable(bool enable) override;
	
	void ChangeState(EState state) override;

	// 初期座標
	CVector mDefaultPos;
	// 下がった後の座標
	CVector mDownPos;
	// 経過時間
	float mElapsedTime;

	CWater* mpWater;
};