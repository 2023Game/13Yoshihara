#pragma once
#include "CGameMenuBase.h"

class CJobUpgradeMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="prevMenu">��O�̃��j���[</param>
	CJobUpgradeMenu(CGameMenuBase* prevMenu);
	// �f�X�g���N�^
	~CJobUpgradeMenu();

	// ���菈��
	void Decide(int select) override;

	// �X�V
	void Update();
};