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

	// 更新
	void Update();
	
private:
	// [TRASH]クリック時のコールバック関数
	void OnClickTrash();
	// [DELIVERY]クリック時のコールバック関数
	void OnClickDelivery();
};