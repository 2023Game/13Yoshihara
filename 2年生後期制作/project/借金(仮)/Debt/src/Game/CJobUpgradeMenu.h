#pragma once
#include "CGameMenuBase.h"

class CJobUpgradeMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="prevMenu">一つ前のメニュー</param>
	CJobUpgradeMenu(CGameMenuBase* prevMenu);
	// デストラクタ
	~CJobUpgradeMenu();

	// 決定処理
	void Decide(int select) override;

	// 更新
	void Update();
};