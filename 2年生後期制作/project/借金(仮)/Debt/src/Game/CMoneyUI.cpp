#include "CMoneyUI.h"
#include "CTextUI2D.h"
#include "CMoneyManager.h"
#include "CImage.h"

// �w�i�̃p�X
#define MONEY_PATH	"UI/text_money_bg.png"

// �w�i�̃T�C�Y
#define BG_SIZE CVector2(WINDOW_WIDTH * 0.4f, WINDOW_HEIGHT * 0.34f)

// �w�i�̃I�t�Z�b�g���W
#define BG_OFFSET_POS CVector2(WINDOW_WIDTH * 0.6f, WINDOW_HEIGHT * 0.65f)

// ����UI�̃I�t�Z�b�g���W
#define MONEY_UI_OFFSET_POS CVector2(WINDOW_WIDTH * 0.65f, WINDOW_HEIGHT * 0.75f)

// �R���X�g���N�^
CMoneyUI::CMoneyUI()
	: CHomeUIBase()
{
	// �e�L�X�g�̍��W��ݒ�
	mpTextUI->Position(MONEY_UI_OFFSET_POS);

	// �w�i�摜��ݒ�
	mpBackground->Load(MONEY_PATH);
	// �w�i�̃T�C�Y��ݒ�
	mpBackground->SetSize(BG_SIZE);
	// �w�i�̍��W��ݒ�
	mpBackground->SetPos(BG_OFFSET_POS);

	// �����̊Ǘ��N���X���擾
	CMoneyManager* moneyMgr = CMoneyManager::Instance();
	// ���̕ԍϊz��ݒ�
	mNextDebtMoney = moneyMgr->GetNextDebtMoney();
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
	mpTextUI->ChangeToStr("�ԍϊz�F%d�~\n�@\n�������F%d�~\n",
		mNextDebtMoney,moneyMgr->GetMoney());
	mpTextUI->Update();
	mpBackground->Update();
}

// �`��
void CMoneyUI::Render()
{
	CHomeUIBase::Render();
}