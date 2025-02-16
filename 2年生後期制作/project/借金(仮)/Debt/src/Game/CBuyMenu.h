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

	// 更新
	void Update();

private:
	// [UNLOCK]クリック時のコールバック関数
	void OnClickUnlock();
	// [UPGRADE]クリック時のコールバック関数
	void OnClickUpgrade();
	// [SELECT]クリック時のコールバック関数
	void OnClickSelect();

	CJobUnlockMenu* mpJobUnlockMenu;		// 仕事アンロックメニュー
	CJobUpgradeMenu* mpJobUpgradeMenu;		// 仕事強化メニュー
	CStageSelectMenu* mpStageSelectMenu;	// 仕事選択メニュー
};