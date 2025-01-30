#include "CTimeUI.h"
#include "CTextUI2D.h"

// 時間UIのオフセット座標
#define TIME_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.38f,10.0f,0.0f)

// コンストラクタ
CTimeUI::CTimeUI(int maxTime)
	: mTime(maxTime)
	, mElapsedTime(0.0f)
{
	mpTimeUI = new CTextUI2D();
}

// デストラクタ
CTimeUI::~CTimeUI()
{
}

// 更新
void CTimeUI::Update()
{
	// 描画する文字列を設定
	mpTimeUI->ChangeStr("のこりじかん：%d\n", GetTime());
	// 座標を設定
	mpTimeUI->Position(TIME_UI_OFFSET_POS);
	// 時間を数える
	Countdown();
}

// 時間を数える
void CTimeUI::Countdown()
{
	// 経過時間を加算
	mElapsedTime += Times::DeltaTime();
	// 1秒以上なら
	if (mElapsedTime >= 1.0f)
	{
		// 1秒マイナス
		mElapsedTime -= 1.0f;
		mTime--;
	}
}

// 時間が経過したか
bool CTimeUI::GetElapsedTime()
{
	return mTime <= 0;
}

// 時間を取得する
int CTimeUI::GetTime() const
{
	return mTime;
}
