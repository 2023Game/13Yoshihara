#include "CMoneyUI.h"
#include "CTextUI2D.h"
#include "CMoneyManager.h"

// ������UI�̃I�t�Z�b�g���W
#define MONEY_UI_OFFSET_POS CVector2(WINDOW_WIDTH * 0.35f, WINDOW_HEIGHT * 0.9f)

// �R���X�g���N�^
CMoneyUI::CMoneyUI()
{
	// ������UI�𐶐�
	mpMoneyUI = new CTextUI2D(ETaskPauseType::eGame, true, nullptr);
	// �����̑����̊��ݒ�
	mpMoneyUI->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// ���W��ݒ�
	mpMoneyUI->Position(MONEY_UI_OFFSET_POS);
}

// �f�X�g���N�^
CMoneyUI::~CMoneyUI()
{
}

// �X�V
void CMoneyUI::Update()
{
	// �����̊Ǘ��N���X���擾
	CMoneyManager* moneyMgr = CMoneyManager::Instance();
	// �`�悷�镶�����ݒ�
	mpMoneyUI->ChangeToStr("�������F%d�~\n", moneyMgr->GetMoney());
}
