#pragma once
#include "CHomeUIBase.h"

class CTextUI2D;
class CImage;

// やることを表示するUI
class CToDoUI : CHomeUIBase
{
public:
	// コンストラクタ
	CToDoUI();
	// デストラクタ
	~CToDoUI();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// やることを表示する文字列をリストに追加
	void AddText(std::string text);
	// やることを表示する文字列をリストから設定する
	void SetText(int num);

private:
	// やることを表示する文字列のリスト
	std::vector<std::string> mTexts;
	// 今表示する文字列
	int mNowTextNum;
};