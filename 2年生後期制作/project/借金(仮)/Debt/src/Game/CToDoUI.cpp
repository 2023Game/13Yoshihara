#include "CToDoUI.h"
#include "CTextUI2D.h"

// コンストラクタ
CToDoUI::CToDoUI()
	: mTexts({})
	, mNowTextNum(0)
{
	// UIを生成
	mpToDoUI = new CTextUI2D(ETaskPauseType::eMenu, true, nullptr);
}

// デストラクタ
CToDoUI::~CToDoUI()
{
	mTexts.clear();
}

// 更新
void CToDoUI::Update()
{
	// 描画する文字列を設定
	mpToDoUI->SetStr("やること：" + mTexts[mNowTextNum]);
}

// やることを表示する文字列をリストに追加
void CToDoUI::AddText(std::string text)
{
	mTexts.push_back(text);
}

// やることを表示する文字列をリストから設定する
void CToDoUI::SetText(int num)
{
	// サイズ以上の数字なら処理しない
	if (mTexts.size() <= num) return;

	mNowTextNum = num;
}
