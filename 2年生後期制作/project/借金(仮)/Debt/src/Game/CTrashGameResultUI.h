#pragma once
#include "CResultUIBase.h"

class CTextUI2D;

class CTrashGameResultUI : public CResultUIBase
{
public:
	// �R���X�g���N�^
	CTrashGameResultUI();
	// �f�X�g���N�^
	~CTrashGameResultUI();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
private:
	// ���_�̃e�L�X�g
	CTextUI2D* mpScorePointText;
	// �S�~�܂̐��̃e�L�X�g
	CTextUI2D* mpNumText;
	// �S�~�܂̒l�i�̃e�L�X�g
	CTextUI2D* mpPriceText;
	// �P�ʂ̃e�L�X�g
	CTextUI2D* mpUnitText;
	// �~�̃e�L�X�g
	CTextUI2D* mpXText;

	// �S�~�܂̉摜
	CImage* mpTrashBagImg;
	// �S�[���h�̃S�~�܂̉摜
	CImage* mpGoldTrashBagImg;
};