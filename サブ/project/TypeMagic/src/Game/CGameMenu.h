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
	// [�^�C�g����}�N���b�N���̃R�[���o�b�N�֐�
	void OnClickTitle();
	// [�������}�N���b�N���̃R�[���o�b�N�֐�
	void OnClickManual();
};