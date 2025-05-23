#pragma once
#include "CObjectBase.h"

class CSwitch;
class CModel;

// スイッチのフレーム部分
class CSwitchFrame : public CObjectBase
{
public:
	// コンストラクタ
	CSwitchFrame(CVector pos, CSwitch* owner);
	// デストラクタ
	~CSwitchFrame();

	// 描画
	void Render() override;

private:
	CSwitch* mpOwner;	// 持ち主

	CModel* mpModel;
};