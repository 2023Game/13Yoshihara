#pragma once
#include "CTask.h"

class CImage;
class CTextUI2D;

// �S�~�E���Q�[���̌x���\���pUI
class CTrashWarningUI : public CTask
{
public:
	// �R���X�g���N�^
	CTrashWarningUI();
	// �f�X�g���N�^
	~CTrashWarningUI();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	// �x���\���p
	CImage* mpWarningImg;
	// �x���e�L�X�g
	CTextUI2D* mpWarningText;
};