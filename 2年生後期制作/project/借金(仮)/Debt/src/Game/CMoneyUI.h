#pragma once

class CTextUI2D;

// �������\��UI
class CMoneyUI
{
public:
	// �R���X�g���N�^
	CMoneyUI();
	// �f�X�g���N�^
	~CMoneyUI();

	// �X�V
	void Update();
private:
	// �e�L�X�gUI
	CTextUI2D* mpMoneyUI;
};