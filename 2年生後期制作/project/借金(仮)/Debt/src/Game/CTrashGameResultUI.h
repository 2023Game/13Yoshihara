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
	// ���l�̃e�L�X�g1
	CTextUI2D* mpNumText1;
	// ���l�̃e�L�X�g2
	CTextUI2D* mpNumText2;
	// �v�Z���ʂ̃e�L�X�g
	CTextUI2D* mpTotalText;
	// �P�ʂ̃e�L�X�g
	CTextUI2D* mpUnitText;
	// �L���̃e�L�X�g
	CTextUI2D* mpSymbolText;
	// ��ԉ��̌v�Z�̐����e�L�X�g
	CTextUI2D* mpDescriptionText;
	// �{�[�i�X�̐����e�L�X�g
	CTextUI2D* mpBonusDescriptionText;

	// �S�~�܂̉摜
	CImage* mpTrashBagImg;
	// �S�[���h�̃S�~�܂̉摜
	CImage* mpGoldTrashBagImg;
};