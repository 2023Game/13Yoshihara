#include "CSelectMenu.h"
#include "CFont.h"
#include "CImage.h"
#include "CExpandButton.h"
#include "CScrollUIBase.h"
#include "Easing.h"

// メニューのアニメーション後の待ち時間
#define OPENED_WAIT_TIME 0.5f

// 対戦開始のボタンのサイズ
#define BATTLE_START_BUTTON_SIZE CVector2(200.0f, 100.0f)
// 対戦開始のボタンの座標
#define BATTLE_START_BUTTON_POS CVector2(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.7f)
// 戻るボタンのサイズ
#define RETURN_BUTTON_SIZE CVector2(150.0f, 75.0f)
// 戻るボタンの座標
#define RETURN_BUTTON_POS CVector2(WINDOW_WIDTH * 0.075f, WINDOW_HEIGHT * 0.925f)
// 属性選択ボタンのサイズ
#define SELECT_ELEMENT_BUTTON_SIZE CVector2(200.0f, 100.0f)
// 属性選択ボタンの座標
#define SELECT_ELEMENT_BUTTON_POS CVector2(WINDOW_WIDTH * 0.25f, WINDOW_HEIGHT * 0.25)

// ボタンのテキストサイズ
#define BUTTON_TEXT_SIZE 32

// 画像のパス
#define PATH "UI/white.png"

// アイコンのサイズ
#define ICON_SIZE CVector2(200.0f,200.0f)
// プレイヤーアイコンの座標
#define PLAYER_ICON_POS CVector2(WINDOW_WIDTH * 0.25f, WINDOW_HEIGHT * 0.5f)
// 敵アイコンの座標
#define ENEMY_ICON_POS CVector2(WINDOW_WIDTH * 0.75f, WINDOW_HEIGHT * 0.5f)
// 短縮詠唱がオンの時のアイコンのオフセット座標
#define ICON_QUICK_OFFSETPOS CVector2(0.0f, WINDOW_HEIGHT * 0.25f)

// コンストラクタ
CSelectMenu::CSelectMenu()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// フォントデータを生成
	mpFont = new CFont("res\\Font\\toroman.ttf");
	mpFont->SetFontSize(BUTTON_TEXT_SIZE);
	mpFont->SetAlignment(FTGL::TextAlignment::ALIGN_CENTER);
	mpFont->SetLineLength(WINDOW_WIDTH);

	// 背景のイメージを生成
	mpBg = new CImage
	(
		"UI/title_bg.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpBg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// [BattleStart]ボタンを生成
	CExpandButton* btn1 = new CExpandButton
	(
		BATTLE_START_BUTTON_POS,
		BATTLE_START_BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// 画像を読み込み
	btn1->LoadButtonImage(PATH, PATH);
	// 文字の読み込み
	btn1->LoadButtonText(nullptr, BUTTON_TEXT_SIZE, "BattleStart");
	// クリック時に呼び出されるコールバック関数を設定
	btn1->SetOnClickFunc(std::bind(&CSelectMenu::OnClickBattleStart, this));
	// 最初は無効
	btn1->SetEnable(false);
	// ボタンリストに追加
	mButtons.push_back(btn1);

	// [Return]ボタンを生成
	CExpandButton* btn2 = new CExpandButton
	(
		RETURN_BUTTON_POS,
		RETURN_BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// 画像を読み込み
	btn2->LoadButtonImage(PATH, PATH);
	// 文字の読み込み
	btn2->LoadButtonText(nullptr, BUTTON_TEXT_SIZE, "Return");
	// クリック時に呼び出されるコールバック関数を設定
	btn2->SetOnClickFunc(std::bind(&CSelectMenu::OnClickReturn, this));
	// 最初は無効
	btn2->SetEnable(false);
	// ボタンリストに追加
	mButtons.push_back(btn2);

	// [SelectElement]ボタンを生成
	CExpandButton* btn3 = new CExpandButton
	(
		SELECT_ELEMENT_BUTTON_POS,
		SELECT_ELEMENT_BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// 画像を読み込み
	btn3->LoadButtonImage(PATH, PATH);
	// 文字の読み込み
	btn3->LoadButtonText(nullptr, BUTTON_TEXT_SIZE, "Select\nElement");
	// クリック時に呼び出されるコールバック関数を設定
	btn3->SetOnClickFunc(std::bind(&CSelectMenu::OnClickSelectElement, this));
	// 最初は無効
	btn3->SetEnable(false);
	// ボタンリストに追加
	mButtons.push_back(btn3);

	// プレイヤーのアイコンの画像を生成
	mpPlayerImg = new CImage
	(
		PATH,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,false
	);
	// アイコンのサイズを設定
	mpPlayerImg->SetSize(ICON_SIZE);
	// アイコンの座標を設定
	mpPlayerImg->SetPos(PLAYER_ICON_POS - mpPlayerImg->GetSize() * 0.5f);

	// スクロールできるUI
	mpScrollUI = new CScrollUIBase(
		CVector2(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.3f),
		CVector2(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.3f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
}

// デストラクタ
CSelectMenu::~CSelectMenu()
{
	SAFE_DELETE(mpFont);
	SAFE_DELETE(mpBg);
	SAFE_DELETE(mpScrollUI);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// セレクト画面が終了か
bool CSelectMenu::IsEnd() const
{
	return mIsEnd;
}

// 対戦を開始するか
bool CSelectMenu::IsBattleStart() const
{
	// 選択項目が1つ目ならば、対戦開始
	return mSelectIndex == 0;
}

// 一つ前に戻るか
bool CSelectMenu::IsReturn() const
{
	// 選択項目が2つ目ならば、戻る
	return mSelectIndex == 1;
}

// [対戦開始]クリック時のコールバック関数
void CSelectMenu::OnClickBattleStart()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

// [戻る]クリック時のコールバック関数
void CSelectMenu::OnClickReturn()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}

// [SelectElement]クリック時のコールバック関数
void CSelectMenu::OnClickSelectElement()
{
}

// 更新
void CSelectMenu::Update()
{
	switch (mState)
	{
	case CSelectMenu::EState::eOpen:	UpdateOpen();		break;
	case CSelectMenu::EState::eSelect:	UpdateSelect();		break;
	case CSelectMenu::EState::eFadeOut:	UpdateFadeOut();	break;
	}

	mpBg->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
	mpScrollUI->Update();
}

// 描画
void CSelectMenu::Render()
{
	// 背景描画
	mpBg->Render();

	// ボタン描画
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}

	// アイコン描画
	mpPlayerImg->Render();

	// スクロールできるUI
	mpScrollUI->Render();
}

// メニューを開く
void CSelectMenu::UpdateOpen()
{
	// メニュー入場後の待ち
	if (mElapsedTime < OPENED_WAIT_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// 一定時間待ったら、ボタンをオンにしてタッチできるようにする
		// （誤タッチを防ぐための待ち時間）
		for (CExpandButton* btn : mButtons)
		{
			btn->SetEnable(true);
		}
		ChangeState(EState::eSelect);
	}
}

// メニューを選択
void CSelectMenu::UpdateSelect()
{
}

// フェードアウト
void CSelectMenu::UpdateFadeOut()
{
}

// 状態切り替え
void CSelectMenu::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}
