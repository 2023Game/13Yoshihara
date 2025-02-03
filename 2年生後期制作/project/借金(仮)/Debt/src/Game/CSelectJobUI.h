#pragma once

class CTextUI2D;

// 選択中の仕事を表示するUI
class CSelectJobUI
{
public:
	// コンストラクタ
	CSelectJobUI();
	// デストラクタ
	~CSelectJobUI();

	// 更新
	void Update();

private:
	// テキストUI
	CTextUI2D* mpSelectJobUI;
};