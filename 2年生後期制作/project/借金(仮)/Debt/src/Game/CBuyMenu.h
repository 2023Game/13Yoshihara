#pragma once
#include "CGameMenuBase.h"
class CJobUnlockMenu;
class CJobUpgradeMenu;
class CStageSelectMenu;

class CBuyMenu : public CGameMenuBase
{
public:
	// コンストラクタ
	CBuyMenu();
	// デストラクタ
	~CBuyMenu();

	// 決定処理
	void Decide(int select) override;

	// 更新
	void Update();

private:
	CJobUnlockMenu* mpJobUnlockMenu;		// 仕事アンロックメニュー
	CJobUpgradeMenu* mpJobUpgradeMenu;		// 仕事強化メニュー
	CStageSelectMenu* mpStageSelectMenu;	// 仕事選択メニュー
};