#pragma once
#include "CGameMenuBase.h"
#include "CManualMenu.h"

class CGameMenu : public CGameMenuBase
{
public:
	// コンストラクタ
	CGameMenu();
	// デストラクタ
	~CGameMenu();

	// 更新
	void Update();

	// 操作説明を設定
	void SetManual(CManualMenu* menu);

private:
	// [タイトルへ}クリック時のコールバック関数
	void OnClickTitle();
	// [操作説明}クリック時のコールバック関数
	void OnClickManual();

	// 操作説明
	CManualMenu* mpManual;
};