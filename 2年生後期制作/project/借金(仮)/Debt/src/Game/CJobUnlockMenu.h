#pragma once
#include "CGameMenuBase.h"

class CJobUnlockMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="prevMenu">一つ前のメニュー</param>
	CJobUnlockMenu(CGameMenuBase* prevMenu);
	~CJobUnlockMenu();

	// 決定処理
	void Decide(int select) override;

	// 更新
	void Update();
};