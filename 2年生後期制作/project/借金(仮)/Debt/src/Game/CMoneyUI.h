#pragma once
#include "CHomeUIBase.h"

class CTextUI2D;
class CImage;

// お金表示UI
class CMoneyUI : CHomeUIBase
{
public:
	// コンストラクタ
	CMoneyUI();
	// デストラクタ
	~CMoneyUI();

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
	// 次の返済額
	int mNextDebtMoney;
};