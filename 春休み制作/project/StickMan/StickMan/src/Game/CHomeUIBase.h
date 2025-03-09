#pragma once
#include "CTask.h"

class CTextUI2D;
class CImage;

// �������\��UI
class CHomeUIBase : public CTask
{
public:
	// �R���X�g���N�^
	CHomeUIBase();
	// �f�X�g���N�^
	~CHomeUIBase();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	// �������̗ʂɂ���Ĕw�i��ݒ�
	void SetBgPath();

protected:
	// �e�L�X�gUI
	CTextUI2D* mpTextUI;
	// �e�L�X�g�̔w�i
	CImage* mpBackground;
	// �w�i�̃p�X
	std::string mBgPath;
};