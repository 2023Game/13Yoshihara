#pragma once

class CTextUI2D;

// 時間表示UI
class CTimeUI
{
public:
	// コンストラクタ
	CTimeUI(int maxTime);
	// デストラクタ
	~CTimeUI();

	// 更新
	void Update();

	// 時間を数える
	void Countdown();
	// 時間が経過したか
	bool GetElapsedTime();

	// 時間を取得する
	int GetTime() const;

private:
	// 時間
	int mTime;
	// 経過時間
	float mElapsedTime;

	// テキストUI
	CTextUI2D* mpTimeUI;
};