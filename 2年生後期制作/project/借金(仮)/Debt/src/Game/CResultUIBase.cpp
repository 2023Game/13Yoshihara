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
#define BUTTON_POS CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.8f)
// �{�^���̃T�C�Y
#define BUTTON_SIZE CVector2(340.0f, 96.0f)

// �e�L�X�g�̃I�t�Z�b�g���W
#define TEXT_POS CVector(0.0f,25.0f,0.0f)

// �R���X�g���N�^
CResultUIBase::CResultUIBase(std::string backGroundPath)
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
	, mSelectIndex(-1)
{
	// �X�R�A�̃e�L�X�g�𐶐�
	mpScoreText = new CTextUI2D(ETaskPauseType::eGame, false);
	mpScoreText->SetStr("Score");
	mpScoreText->SetFontSize(116);
	mpScoreText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	mpScoreText->Position(TEXT_POS);

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
		BUTTON_POS,
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
	mpScoreText->Update();
	mpResultBg->Update();
	mpReturnButton->Update();
}

// �`��
void CResultUIBase::Render()
{
	// �`�揇
	// �w�i���X�R�A�����_�֖߂�{�^��
	
	// �w�i�`��
	mpResultBg->Render();
	// �X�R�A�̕����`��
	mpScoreText->Render();
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