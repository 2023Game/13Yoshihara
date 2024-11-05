#pragma once
#include "CGameMenuBase.h"

class CStageSelectMenu : public CGameMenuBase
{
public:
	CStageSelectMenu();
	~CStageSelectMenu();

	void Decide(int select) override;

	void Update();
};