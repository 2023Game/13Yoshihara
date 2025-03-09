#pragma once
#include "CEndUIBase.h"

class CTextUI2D;

class CResultUIBase : public CEndUIBase
{
public:
	// �R���X�g���N�^
	CResultUIBase(std::string backGroundPath);
	// �f�X�g���N�^
	~CResultUIBase();

	// ���_�֖߂邩
	bool IsReturnHome() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	// �X�R�A���擾
	int GetScore() const;
	// �X�R�A��ݒ�
	void SetScore(int score);

protected:

	// [���_��]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickReturn();

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

	// ���_
	int mScore;
};