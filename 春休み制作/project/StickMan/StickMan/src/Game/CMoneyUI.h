#pragma once
#include "CHomeUIBase.h"

class CTextUI2D;
class CImage;

// �����\��UI
class CMoneyUI : CHomeUIBase
{
public:
	// �R���X�g���N�^
	CMoneyUI();
	// �f�X�g���N�^
	~CMoneyUI();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
private:
	// ���̕ԍϊz
	int mNextDebtMoney;
};