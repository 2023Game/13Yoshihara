#include "CMoneyUI.h"
#include "CTextUI2D.h"
#include "CMoneyManager.h"
#include "CImage.h"

// 背景のパス
#define MONEY_PATH	"UI/text_money_bg.png"

// 背景のサイズ
#define BG_SIZE CVector2(WINDOW_WIDTH * 0.4f, WINDOW_HEIGHT * 0.34f)

// 背景のオフセット座標
#define BG_OFFSET_POS CVector2(WINDOW_WIDTH * 0.6f, WINDOW_HEIGHT * 0.65f)

// お金UIのオフセット座標
#define MONEY_UI_OFFSET_POS CVector2(WINDOW_WIDTH * 0.65f, WINDOW_HEIGHT * 0.75f)

// コンストラクタ
CMoneyUI::CMoneyUI()
	: CHomeUIBase()
{
	// テキストの座標を設定
	mpTextUI->Position(MONEY_UI_OFFSET_POS);

	// 背景画像を設定
	mpBackground->Load(MONEY_PATH);
	// 背景のサイズを設定
	mpBackground->SetSize(BG_SIZE);
	// 背景の座標を設定
	mpBackground->SetPos(BG_OFFSET_POS);

	// お金の管理クラスを取得
	CMoneyManager* moneyMgr = CMoneyManager::Instance();
	// 次の返済額を設定
	mNextDebtMoney = moneyMgr->GetNextDebtMoney();
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
	mpTextUI->ChangeToStr("返済額：%d円\n　\n所持金：%d円\n",
		mNextDebtMoney,moneyMgr->GetMoney());
	mpTextUI->Update();
	mpBackground->Update();
}

// 描画
void CMoneyUI::Render()
{
	CHomeUIBase::Render();
}