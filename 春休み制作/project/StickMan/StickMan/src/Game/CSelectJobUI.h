#pragma once
#include "CHomeUIBase.h"

class CTextUI2D;

// �I�𒆂̎d����\������UI
class CSelectJobUI : CHomeUIBase
{
public:
	// �R���X�g���N�^
	CSelectJobUI();
	// �f�X�g���N�^
	~CSelectJobUI();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
};