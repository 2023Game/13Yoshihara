#pragma once
#include "CTask.h"

class CImage;
class CTextUI2D;

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

private:
	// 警告表示用
	CImage* mpWarningImg;
	// 警告テキスト
	CTextUI2D* mpWarningText;
};