#include "CGameClearUI.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// メニューのアニメーション時間
#define OPEN_ANIM_TIME 0.25f
// メニューのアニメーション後の待ち時間
#define OPENED_WAIT_TIME 0.5f

// ボタンのオフセット座標
#define BUTTON_OFFSET_POS1 CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.5f)
// ボタンのサイズ
#define BUTTON_SIZE CVector2(340.0f, 96.0f)

// 背景画像のパス
#define BG_PATH "UI/game_clear_bg.png"

// コンストラクタ
CGameClearUI::CGameClearUI()
	: CEndUIBase(BG_PATH)
{
	// テキストを設定
	mpText->SetStr("ゲームクリア");

	// [終了]ボタンを生成
	CExpandButton* btn = new CExpandButton
	(
		BUTTON_OFFSET_POS1,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn->LoadButtonImage("UI/title_quit0.png", "UI/title_quit1.png");
	btn->SetOnClickFunc(std::bind(&CGameClearUI::OnClickQuit, this));
	btn->SetEnable(false);
	btn->SetScale(0.0f);
	mButtons.push_back(btn);
}

// デストラクタ
CGameClearUI::~CGameClearUI()
{
}

// ゲームを終了するか
bool CGameClearUI::IsExitGame() const
{
	// 選択項目が1つ目ならば、ゲーム終了
	return mSelectIndex == 0;
}

// 更新
void CGameClearUI::Update()
{
	CEndUIBase::Update();
}

// 描画
void CGameClearUI::Render()
{
	CEndUIBase::Render();
}

// [終了]クリック時のコールバック関数
void CGameClearUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}
