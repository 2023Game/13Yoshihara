#pragma once
#include "CResultUIBase.h"

class CDeliveryGameResultUI : public CResultUIBase
{
public:
	// �R���X�g���N�^
	CDeliveryGameResultUI();
	// �f�X�g���N�^
	~CDeliveryGameResultUI();

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

	// �z�B���̉摜
	CImage* mpDeliveryItemImg;
};