#pragma once
#include "CGameMenuBase.h"

class CGameMenu : public CGameMenuBase
{
public:
	// �R���X�g���N�^
	CGameMenu();
	// �f�X�g���N�^
	~CGameMenu();

	// ���菈��
	void Decide(int select) override;

	// �X�V
	void Update();
};