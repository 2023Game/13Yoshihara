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
	CJobUnlockMenu* mpJobUnlockMenu;// �d��������j���[
	CJobUpgradeMenu* mpJobUpgradeMenu;		// �d���������j���[
};