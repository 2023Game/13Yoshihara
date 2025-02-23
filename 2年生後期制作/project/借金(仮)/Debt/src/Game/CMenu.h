#pragma once
#include "CGameMenuBase.h"

// ホームシーンでのメニュー
class CMenu : public CGameMenuBase
{
public:
	// コンストラクタ
	CMenu();
	// デストラクタ
	~CMenu();

	// 更新
	void Update();

private:
	// [タイトルへ}クリック時のコールバック関数
	void OnClickTitle();
};