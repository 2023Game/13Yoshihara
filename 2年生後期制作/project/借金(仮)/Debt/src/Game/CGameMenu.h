#pragma once
#include "CGameMenuBase.h"

class CGameMenu : public CGameMenuBase
{
public:
	// コンストラクタ
	CGameMenu();
	// デストラクタ
	~CGameMenu();

	// 決定処理
	void Decide(int select) override;

	// 更新
	void Update();
};