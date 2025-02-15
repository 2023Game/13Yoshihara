#include "CResultUIBase.h"
#include "CFont.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// �^�C�g�����j���[�̃A�j���[�V��������
#define OPEN_ANIM_TIME 0.25f
// �^�C�g�����j���[�̃A�j���[�V������̑҂�����
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
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
	, mSelectIndex(-1)
{
	// �X�R�A�Ƃ����e�L�X�g�𐶐�
	mpScoreText = new CTextUI2D(ETaskPauseType::eGame, false);
	mpScoreText->SetStr("Score");
	mpScoreText->SetFontSize(116);
	mpScoreText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	mpScoreText->Position(SCORE_TEXT_OFFSET_POS);

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


	// ���U���g��ʂ̔w�i�C���[�W�𐶐�
	mpResultBg = new CImage
	(
		backGroundPath.c_str(),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	// [���_��]�{�^���𐶐�
	mpReturnButton = new CExpandButton
	(
		BUTTON_OFFSET_POS,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �{�^���̉摜��ǂݍ���
	mpReturnButton->LoadButtonImage("UI/result_return0.png", "UI/result_return1.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	mpReturnButton->SetOnClickFunc(std::bind(&CResultUIBase::OnClickReturn, this));
	// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
	mpReturnButton->SetEnable(false);
	mpReturnButton->SetScale(0.0f);
}

// �f�X�g���N�^
CResultUIBase::~CResultUIBase()
{
	SAFE_DELETE(mpScoreText);
	SAFE_DELETE(mpScorePointText);
	SAFE_DELETE(mpNumText1);
	SAFE_DELETE(mpNumText2);
	SAFE_DELETE(mpUnitText);
	SAFE_DELETE(mpSymbolText);
	SAFE_DELETE(mpDescriptionText);
	SAFE_DELETE(mpBonusDescriptionText);
	SAFE_DELETE(mpResultBg);
}

// ���ʉ�ʏI����
bool CResultUIBase::IsEnd() const
{
	return mIsEnd;
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
	switch (mState)
	{
		// ���j���[���J��
	case EState::eOpen:
		UpdateOpen();
		break;
		// ���j���[�I��
	case EState::eSelect:
		UpdateSelect();
		break;
		// �t�F�[�h�A�E�g
	case EState::eFadeOut:
		UpdateFadeOut();
		break;
	}

	// �e�L�X�g
	mpScoreText->Update();
	mpScorePointText->Update();
	mpNumText1->Update();
	mpNumText2->Update();
	mpTotalText->Update();
	mpUnitText->Update();
	mpSymbolText->Update();
	mpDescriptionText->Update();
	mpBonusDescriptionText->Update();
	// �w�i�摜
	mpResultBg->Update();
	// �{�^��
	mpReturnButton->Update();
}

// �`��
void CResultUIBase::Render()
{
	// �`�揇
	// �w�i���e�L�X�g�����_�֖߂�{�^��
	
	// �w�i�`��
	mpResultBg->Render();
	// �e�L�X�g�`��
	mpScoreText->Render();
	mpScorePointText->Render();
	mpNumText1->Render();
	mpNumText2->Render();
	mpTotalText->Render();
	mpUnitText->Render();
	mpSymbolText->Render();
	mpDescriptionText->Render();
	mpBonusDescriptionText->Render();
	// �{�^���`��
	mpReturnButton->Render();
}

// ���j���[���J��
void CResultUIBase::UpdateOpen()
{
	switch (mStateStep)
	{
		// �X�e�b�v0�F���j���[�̓���A�j���[�V����
	case 0:
		if (mElapsedTime < OPEN_ANIM_TIME)
		{
			// �X�P�[���l����U1.0���傫�����āA1.0�֖߂�C�[�W���O�A�j���[�V����
			float scale = Easing::BackOut(mElapsedTime, OPEN_ANIM_TIME, 0.0f, 1.0f, 2.0f);
			mpReturnButton->SetScale(scale);
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			mpReturnButton->SetScale(1.0f);
			mStateStep++;
			mElapsedTime = 0.0f;
		}
		break;
		// �X�e�b�v1�F���j���[�����̑҂�
	case 1:
		if (mElapsedTime < OPENED_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// ��莞�ԑ҂�����A�{�^�����I���ɂ��ă^�b�`�ł���悤�ɂ���
			// �i��^�b�`��h�����߂̑҂����ԁj
			mpReturnButton->SetEnable(true);
			ChangeState(EState::eSelect);
		}
		break;
	}
}

// ���j���[�I��
void CResultUIBase::UpdateSelect()
{
}

// �t�F�[�h�A�E�g
void CResultUIBase::UpdateFadeOut()
{
}

// ��Ԑ؂�ւ�
void CResultUIBase::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}