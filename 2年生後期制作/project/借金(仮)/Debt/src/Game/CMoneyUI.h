#pragma once

class CTextUI2D;

// 所持金表示UI
class CMoneyUI
{
public:
	// コンストラクタ
	CMoneyUI();
	// デストラクタ
	~CMoneyUI();

	// 更新
	void Update();
private:
	// テキストUI
	CTextUI2D* mpMoneyUI;
};