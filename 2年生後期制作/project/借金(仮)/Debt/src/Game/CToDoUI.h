#pragma once

class CTextUI2D;

// やることを表示するUI
class CToDoUI
{
public:
	// コンストラクタ
	CToDoUI();
	// デストラクタ
	~CToDoUI();

	// 更新
	void Update();

	// やることを表示する文字列をリストに追加
	void AddText(std::string text);
	// やることを表示する文字列をリストから設定する
	void SetText(int num);
private:
	// やることを表示する文字列のリスト
	std::vector<std::string> mTexts;
	// 今表示する文字列
	int mNowTextNum;
	// テキストUI
	CTextUI2D* mpToDoUI;
};