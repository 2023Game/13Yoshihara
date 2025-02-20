#include "CGameOverUI.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"
#include "CMoneyManager.h"
#include "CJobStatusManager.h"

// メニューのアニメーション時間
#define OPEN_ANIM_TIME 0.25f
// メニューのアニメーション後の待ち時間
#define OPENED_WAIT_TIME 0.5f

// ボタンのオフセット座標
#define BUTTON_OFFSET_POS1 CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.5f)
#define BUTTON_OFFSET_POS2 CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.75f)
// ボタンのサイズ
#define BUTTON_SIZE CVector2(340.0f, 96.0f)

// 背景画像のパス
#define BG_PATH "UI/game_over_bg.png"

// コンストラクタ
CGameOverUI::CGameOverUI()
	: CEndUIBase(BG_PATH)
{
	// テキストを設定
	mpText->SetStr("ゲームオーバー");

	// [前日へ]ボタンを生成
	CExpandButton* btn1 = new CExpandButton
	(
		BUTTON_OFFSET_POS1,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	btn1->LoadButtonImage("UI/previous_day0.png", "UI/previous_day1.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn1->SetOnClickFunc(std::bind(&CGameOverUI::OnClickPreDay, this));
	// ボタンは最初は無効化して、スケール値を0にしておく
	btn1->SetEnable(false);
	btn1->SetScale(0.0f);
	// ボタンリストに追加
	mButtons.push_back(btn1);

	// [終了]ボタンを生成
	CExpandButton* btn2 = new CExpandButton
	(
		BUTTON_OFFSET_POS2,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn2->LoadButtonImage("UI/title_quit0.png", "UI/title_quit1.png");
	btn2->SetOnClickFunc(std::bind(&CGameOverUI::OnClickQuit, this));
	btn2->SetEnable(false);
	btn2->SetScale(0.0f);
	mButtons.push_back(btn2);
}

// デストラクタ
CGameOverUI::~CGameOverUI()
{
}

// 前日へ戻るか
bool CGameOverUI::IsPreDay() const
{
	// 選択項目が1つ目ならば、前日へ
	return mSelectIndex == 0;
}

// ゲームを終了するか
bool CGameOverUI::IsExitGame() const
{
	// 選択項目が2つ目ならば、ゲーム終了
	return mSelectIndex == 1;
}

// 更新
void CGameOverUI::Update()
{
	CEndUIBase::Update();
}

// 描画
void CGameOverUI::Render()
{
	CEndUIBase::Render();
}

// [前日へ]クリック時のコールバック関数
void CGameOverUI::OnClickPreDay()
{
	if (mIsEnd) return;

	// お金の管理クラスを取得
	auto* moneyMgr = CMoneyManager::Instance();
	// ロールバックする
	moneyMgr->Rollback();

	// 仕事のステータス管理クラスを取得
	auto* jobStatusMgr = CJobStatusManager::Instance();
	// ロールバックする
	jobStatusMgr->Rollback();

	mSelectIndex = 0;
	mIsEnd = true;
}

// [終了]クリック時のコールバック関数
void CGameOverUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}
