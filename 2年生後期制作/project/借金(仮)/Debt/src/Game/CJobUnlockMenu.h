#pragma once
#include "CGameMenuBase.h"

class CJobUnlockMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="prevMenu">��O�̃��j���[</param>
	CJobUnlockMenu(CGameMenuBase* prevMenu);
	~CJobUnlockMenu();

	void Decide(int select) override;

	void Update();
};