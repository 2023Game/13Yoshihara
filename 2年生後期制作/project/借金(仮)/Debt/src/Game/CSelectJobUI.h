#pragma once

class CTextUI2D;

// �I�𒆂̎d����\������UI
class CSelectJobUI
{
public:
	// �R���X�g���N�^
	CSelectJobUI();
	// �f�X�g���N�^
	~CSelectJobUI();

	// �X�V
	void Update();

private:
	// �e�L�X�gUI
	CTextUI2D* mpSelectJobUI;
};