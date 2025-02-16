#pragma once
#include "CGameMenuBase.h"

class CGameMenu : public CGameMenuBase
{
public:
	// コンストラクタ
	CGameMenu();
	// デストラクタ
	~CGameMenu();

	// 更新
	void Update();

private:
	// [TITLE}クリック時のコールバック関数
	void OnClickTitle();
	// [SETTING}クリック時のコールバック関数
	void OnClickSetting();
};