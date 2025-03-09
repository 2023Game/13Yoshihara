#pragma once
#include "CGameMenuBase.h"

class CStageSelectMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="prevMenu">一つ前のメニュー</param>
	CStageSelectMenu(CGameMenuBase* prevMenu);
	// デストラクタ
	~CStageSelectMenu();

	// 更新
	void Update();

private:
	// [TRASH]クリック時のコールバック関数
	void OnClickTrash();
	// [DELIVERY]クリック時のコールバック関数
	void OnClickDelivery();
};