#include "CTimeUI.h"
#include "CTextUI2D.h"
#include "CTaskManager.h"

// 時間UIのオフセット座標
const CVector TIME_UI_OFFSET_POS = CVector(WINDOW_WIDTH * 0.35f, 10.0f, 0.0f);

// コンストラクタ
CTimeUI::CTimeUI(int maxTime)
	: mTimeLocation(maxTime)
	, mElapsedTime(0.0f)
{
	// 時間UIを生成
	mpTimeUI = new CTextUI2D();
	// 文字の揃いの基準を設定
	mpTimeUI->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// 座標を設定
	mpTimeUI->Position(TIME_UI_OFFSET_POS);
}

// デストラクタ
CTimeUI::~CTimeUI()
{
}

// 更新
void CTimeUI::Update()
{
	// 描画する文字列を設定
	mpTimeUI->ChangeToStr("Time： % d\n", GetTime());
	// 時間を数える
	Countdown();
}

// 時間を数える
void CTimeUI::Countdown()
{
	CTaskManager* taskMgr=CTaskManager::Instance();
	// ポーズされていない場合
	if (!taskMgr->IsPaused())
	{
		// 経過時間を加算
		mElapsedTime += Times::DeltaTime();
	}
	// 1秒以上なら
	if (mElapsedTime >= 1.0f)
	{
		// 1秒マイナス
		mElapsedTime -= 1.0f;
		mTimeLocation--;
	}
}

// 時間が経過したか
bool CTimeUI::GetElapsedTime()
{
	return mTimeLocation <= 0;
}

// 時間を取得する
int CTimeUI::GetTime() const
{
	return mTimeLocation;
}
