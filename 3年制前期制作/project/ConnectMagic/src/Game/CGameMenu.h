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
	// [タイトルへ}クリック時のコールバック関数
	void OnClickTitle();
	// [操作説明}クリック時のコールバック関数
	void OnClickManual();
};