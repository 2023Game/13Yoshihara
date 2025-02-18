#include "CToDoUI.h"
#include "CTextUI2D.h"
#include "CImage.h"

// 背景のサイズ
#define BG_SIZE CVector2(WINDOW_WIDTH, WINDOW_HEIGHT * 0.17f)

// テキストのオフセット座標
#define TEXT_OFFSET_POS CVector2(50.0f, 50.0f)

// コンストラクタ
CToDoUI::CToDoUI()
	: CHomeUIBase()
	, mTexts({})
	, mNowTextNum(0)
{
	// テキストの座標を設定
	mpTextUI->Position(TEXT_OFFSET_POS);

	// 背景のサイズを設定
	mpBackground->SetSize(BG_SIZE);
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
	mpTextUI->SetStr("やること：" + mTexts[mNowTextNum]);

	CHomeUIBase::Update();
}

// 描画
void CToDoUI::Render()
{
	CHomeUIBase::Render();
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