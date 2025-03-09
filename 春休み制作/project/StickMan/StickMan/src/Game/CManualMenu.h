#pragma once
#include "CGameMenuBase.h"

class CTextUI2D;

class CManualMenu : public CGameMenuBase
{
public:
	// �R���X�g���N�^
	CManualMenu(std::string bgPath);
	// �f�X�g���N�^
	~CManualMenu();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// ���j���[���J��
	void UpdateOpen() override;

	CTextUI2D* mpCloseText;	// �u�N���b�N�ŕ���v�̃e�L�X�g
};