#pragma once
#include "CGameMenuBase.h"

class CGameMenu : public CGameMenuBase
{
public:
	// �R���X�g���N�^
	CGameMenu();
	// �f�X�g���N�^
	~CGameMenu();

	// �X�V
	void Update();

private:
	// [TITLE}�N���b�N���̃R�[���o�b�N�֐�
	void OnClickTitle();
	// [SETTING}�N���b�N���̃R�[���o�b�N�֐�
	void OnClickSetting();
};