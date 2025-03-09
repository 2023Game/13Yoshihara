#pragma once
#include "CGameMenuBase.h"
#include "CManualMenu.h"

class CGameMenu : public CGameMenuBase
{
public:
	// �R���X�g���N�^
	CGameMenu();
	// �f�X�g���N�^
	~CGameMenu();

	// �X�V
	void Update();

	// ���������ݒ�
	void SetManual(CManualMenu* menu);

private:
	// [�^�C�g����}�N���b�N���̃R�[���o�b�N�֐�
	void OnClickTitle();
	// [�������}�N���b�N���̃R�[���o�b�N�֐�
	void OnClickManual();

	// �������
	CManualMenu* mpManual;
};