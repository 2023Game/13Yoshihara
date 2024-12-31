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

	// 決定処理
	void Decide(int select) override;

	// 更新
	void Update();
};