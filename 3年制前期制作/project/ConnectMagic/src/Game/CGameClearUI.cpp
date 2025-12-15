#include "CGameClearUI.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// メニューのアニメーション時間
constexpr float OPEN_ANIM_TIME =		0.25f;
// メニューのアニメーション後の待ち時間
constexpr float  OPENED_WAIT_TIME =		0.5f;

// ボタンのオフセット座標
const CVector2 BUTTON_OFFSET_POS1 =		CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.5f);
const CVector2 BUTTON_OFFSET_POS2 =		CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.75f);
// ボタンのサイズ
const CVector2 BUTTON_SIZE =			CVector2(340.0f, 96.0f);

// 背景画像のパス
const char* const BG_PATH =				"UI/game_clear_bg.png";

// コンストラクタ
CGameClearUI::CGameClearUI()
	: CEndUIBase(BG_PATH)
{
	// テキストを設定
	mpText->SetStr("ゲームクリア");	
	
	// [タイトルへ]ボタンを生成
	CExpandButton* btn = new CExpandButton
	(
		BUTTON_OFFSET_POS1,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn->LoadButtonImage("UI/title0.png", "UI/title1.png");
	btn->SetOnClickFunc(std::bind(&CGameClearUI::OnClickTitle, this));
	btn->SetEnable(false);
	btn->SetScale(0.0f);
	//mButtons.push_back(btn);

	// [終了]ボタンを生成
	btn = new CExpandButton
	(
		BUTTON_OFFSET_POS2,
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

// タイトルへ戻るか
bool CGameClearUI::IsReturnTitle() const
{
	// 選択項目が1つ目ならば、タイトルへ
	return mSelectIndex == 0;
}

// ゲームを終了するか
bool CGameClearUI::IsExitGame() const
{
	// 選択項目が2つ目ならば、ゲーム終了
	return mSelectIndex == 1;
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

// [タイトルへ]クリック時のコールバック関数
void CGameClearUI::OnClickTitle()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

// [終了]クリック時のコールバック関数
void CGameClearUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}
