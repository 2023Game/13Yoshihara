#include "CResultUIBase.h"
#include "CFont.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// ���j���[�̃A�j���[�V��������
#define OPEN_ANIM_TIME 0.25f
// ���j���[�̃A�j���[�V������̑҂�����
#define OPENED_WAIT_TIME 0.5f

// �{�^���̃I�t�Z�b�g���W
#define BUTTON_OFFSET_POS CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.25f)
// �{�^���̃T�C�Y
#define BUTTON_SIZE CVector2(340.0f, 96.0f)

// �X�R�A�Ƃ����e�L�X�g�̃I�t�Z�b�g���W
#define SCORE_TEXT_OFFSET_POS			CVector2(0.0f,25.0f)
// ���_�̃e�L�X�g�̃I�t�Z�b�g���W
#define SCORE_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.0f,   WINDOW_HEIGHT * 0.2f)
// �S�~�܂̐��̃e�L�X�g�̃I�t�Z�b�g���W
#define NUM_OFFSET_POS					CVector2(WINDOW_WIDTH * 0.3f,   WINDOW_HEIGHT * 0.4f)
// �S�~�܂̒l�i�̃e�L�X�g�̃I�t�Z�b�g���W
#define PRICE_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.475f, WINDOW_HEIGHT * 0.4f)
// �S�~�܂̒l�i�̃e�L�X�g�̃I�t�Z�b�g���W
#define TOTAL_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.7f,   WINDOW_HEIGHT * 0.4f)
// �P�ʂ̃e�L�X�g�̃I�t�Z�b�g���W
#define UNIT_OFFSET_POS					CVector2(WINDOW_WIDTH * 0.35f,  WINDOW_HEIGHT * 0.4f)
// �L���̃e�L�X�g�̃I�t�Z�b�g���W
#define SYMBOL_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.42f,  WINDOW_HEIGHT * 0.4f)
// ��ԉ��̌v�Z�̐����e�L�X�g�̃I�t�Z�b�g���W
#define DESCRIPTION_OFFSET_POS			CVector2(WINDOW_WIDTH * 0.15f, WINDOW_HEIGHT * 0.76f)
// �{�[�i�X�̐����e�L�X�g�̃I�t�Z�b�g���W
#define BONUS_DESCRIPTION_OFFSET_POS	CVector2(WINDOW_WIDTH * 0.475f, WINDOW_HEIGHT * 0.83f)


// �R���X�g���N�^
CResultUIBase::CResultUIBase(std::string backGroundPath)
	: CEndUIBase(backGroundPath)
{
	// �e�L�X�g��ݒ�
	mpText->SetStr("Score");
	mpText->SetFontSize(116);
	mpText->Position(SCORE_TEXT_OFFSET_POS);

	// ���_�̃e�L�X�g�𐶐�
	mpScorePointText = new CTextUI2D(ETaskPauseType::eGame, false);
	// ���W��ݒ�
	mpScorePointText->Position(SCORE_OFFSET_POS);
	// �t�H���g�T�C�Y��ݒ�
	mpScorePointText->SetFontSize(128);
	// �����̑����̊��ݒ�
	mpScorePointText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);

	// ���l�̃e�L�X�g1�𐶐�
	mpNumText1 = new CTextUI2D(ETaskPauseType::eGame, false);
	// ���W��ݒ�
	mpNumText1->Position(NUM_OFFSET_POS);

	// ���l�̃e�L�X�g2�𐶐�
	mpNumText2 = new CTextUI2D(ETaskPauseType::eGame, false);
	// ���W��ݒ�
	mpNumText2->Position(PRICE_OFFSET_POS);

	// �v�Z���ʂ̃e�L�X�g�𐶐�
	mpTotalText = new CTextUI2D(ETaskPauseType::eGame, false);
	// ���W��ݒ�
	mpTotalText->Position(TOTAL_OFFSET_POS);

	// �P�ʂ̃e�L�X�g�𐶐�
	mpUnitText = new CTextUI2D(ETaskPauseType::eGame, false);
	// �P�ʂ̃e�L�X�g��ݒ�
	mpUnitText->SetStr("�R�@�@�@�@�@�@�~\n�@\n�@\n�R�@�@�@�@�@�@�~\n");
	// ���W��ݒ�
	mpUnitText->Position(UNIT_OFFSET_POS);

	// �L���̃e�L�X�g�𐶐�
	mpSymbolText = new CTextUI2D(ETaskPauseType::eGame, false);
	// �L���̃e�L�X�g��ݒ�
	mpSymbolText->SetStr("�~�@�@�@�@�@�@��\n�@\n�@\n�~�@�@�@�@�@�@��\n�@\n�@\n�~�@�@�@�@�@�@��\n");
	// ���W��ݒ�
	mpSymbolText->Position(SYMBOL_OFFSET_POS);

	// ��ԉ��̌v�Z�̐����e�L�X�g
	mpDescriptionText = new CTextUI2D(ETaskPauseType::eGame, false);
	//  ��ԉ��̌v�Z�̐����e�L�X�g��ݒ�
	mpDescriptionText->SetStr("�{�[�i�X�I�I");
	// ���W��ݒ�
	mpDescriptionText->Position(DESCRIPTION_OFFSET_POS);
	// �����̐F��ݒ�
	mpDescriptionText->SetFontColor(CColor(0.9f, 0.0f, 0.0f));

	// �{�[�i�X�̐����e�L�X�g
	mpBonusDescriptionText = new CTextUI2D(ETaskPauseType::eGame, false, nullptr);
	// �t�H���g�T�C�Y��ݒ�
	mpBonusDescriptionText->SetFontSize(32);
	// ���W��ݒ�
	mpBonusDescriptionText->Position(BONUS_DESCRIPTION_OFFSET_POS);

	// [���_��]�{�^���𐶐�
	CExpandButton* btn = new CExpandButton
	(
		BUTTON_OFFSET_POS,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �{�^���̉摜��ǂݍ���
	btn->LoadButtonImage("UI/result_return0.png", "UI/result_return1.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn->SetOnClickFunc(std::bind(&CResultUIBase::OnClickReturn, this));
	// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
	btn->SetEnable(false);
	btn->SetScale(0.0f);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn);
}

// �f�X�g���N�^
CResultUIBase::~CResultUIBase()
{
	SAFE_DELETE(mpScorePointText);
	SAFE_DELETE(mpNumText1);
	SAFE_DELETE(mpNumText2);
	SAFE_DELETE(mpUnitText);
	SAFE_DELETE(mpSymbolText);
	SAFE_DELETE(mpDescriptionText);
	SAFE_DELETE(mpBonusDescriptionText);
}

// ���_�֖߂邩
bool CResultUIBase::IsReturnHome() const
{
	// �I�����ڂ�1�ڂȂ�΁A���_�֖߂�
	return mSelectIndex == 0;
}

// [RETURN]�N���b�N���̃R�[���o�b�N�֐�
void CResultUIBase::OnClickReturn()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}


// �X�V
void CResultUIBase::Update()
{
	CEndUIBase::Update();

	// �e�L�X�g
	mpScorePointText->Update();
	mpNumText1->Update();
	mpNumText2->Update();
	mpTotalText->Update();
	mpUnitText->Update();
	mpSymbolText->Update();
	mpDescriptionText->Update();
	mpBonusDescriptionText->Update();
}

// �`��
void CResultUIBase::Render()
{
	CEndUIBase::Render();

	// �e�L�X�g�`��
	mpScorePointText->Render();
	mpNumText1->Render();
	mpNumText2->Render();
	mpTotalText->Render();
	mpUnitText->Render();
	mpSymbolText->Render();
	mpDescriptionText->Render();
	mpBonusDescriptionText->Render();
}

// �X�R�A���擾
int CResultUIBase::GetScore() const
{
	return mScore;
}

// �X�R�A��ݒ�
void CResultUIBase::SetScore(int score)
{
	mScore = score;
}