#pragma once
#include "CGameMenuBase.h"

class CStageSelectMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="prevMenu">��O�̃��j���[</param>
	CStageSelectMenu(CGameMenuBase* prevMenu);
	// �f�X�g���N�^
	~CStageSelectMenu();

	// ���菈��
	void Decide(int select) override;

	// �X�V
	void Update();
};