#pragma once
#include "CTask.h"

class CImage;
class CTextUI2D;
class CSound;

// ゴミ拾いゲームの警告表示用UI
class CTrashWarningUI : public CTask
{
public:
	// コンストラクタ
	CTrashWarningUI();
	// デストラクタ
	~CTrashWarningUI();

	// 更新
	void Update();
	// 描画
	void Render();

	// 再生している音の停止
	void StopSE();

private:
	// 警告表示用
	CImage* mpWarningImg;

	int mStateStep;
	float mElapsedTime; // 経過時間

	CSound* mpAlarmSE;	// 警告音
	float mIntervalTime;	// 再生間隔の時間
	bool mIsPlay;	// 再生するか
};