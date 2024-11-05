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

	void Decide(int select) override;

	void Update();
};