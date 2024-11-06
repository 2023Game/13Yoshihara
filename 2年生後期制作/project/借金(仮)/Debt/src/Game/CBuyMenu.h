#pragma once
#include "CGameMenuBase.h"
class CJobUnlockMenu;
class CJobUpgradeMenu;

class CBuyMenu : public CGameMenuBase
{
public:
	CBuyMenu();
	~CBuyMenu();

	void Decide(int select) override;

	void Update();

private:
	CJobUnlockMenu* mpJobUnlockMenu;// 仕事アンロックメニュー
	CJobUpgradeMenu* mpJobUpgradeMenu;		// 仕事強化メニュー
};