#include "CMoneyUI.h"
#include "CTextUI2D.h"
#include "CMoneyManager.h"

// 所持金UIのオフセット座標
#define MONEY_UI_OFFSET_POS CVector2(WINDOW_WIDTH * 0.35f, WINDOW_HEIGHT * 0.9f)

// コンストラクタ
CMoneyUI::CMoneyUI()
{
	// 所持金UIを生成
	mpMoneyUI = new CTextUI2D(ETaskPauseType::eGame, true, nullptr);
	// 文字の揃いの基準を設定
	mpMoneyUI->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// 座標を設定
	mpMoneyUI->Position(MONEY_UI_OFFSET_POS);
}

// デストラクタ
CMoneyUI::~CMoneyUI()
{
}

// 更新
void CMoneyUI::Update()
{
	// お金の管理クラスを取得
	CMoneyManager* moneyMgr = CMoneyManager::Instance();
	// 描画する文字列を設定
	mpMoneyUI->ChangeToStr("所持金：%d円\n", moneyMgr->GetMoney());
}
