#include "CSelectMenu.h"
#include "CFont.h"
#include "CImage.h"
#include "CExpandButton.h"
#include "CScrollUIBase.h"
#include "Easing.h"

// ���j���[�̃A�j���[�V������̑҂�����
#define OPENED_WAIT_TIME 0.5f

// �ΐ�J�n�̃{�^���̃T�C�Y
#define BATTLE_START_BUTTON_SIZE CVector2(200.0f, 100.0f)
// �ΐ�J�n�̃{�^���̍��W
#define BATTLE_START_BUTTON_POS CVector2(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.7f)
// �߂�{�^���̃T�C�Y
#define RETURN_BUTTON_SIZE CVector2(150.0f, 75.0f)
// �߂�{�^���̍��W
#define RETURN_BUTTON_POS CVector2(WINDOW_WIDTH * 0.075f, WINDOW_HEIGHT * 0.925f)
// �����I���{�^���̃T�C�Y
#define SELECT_ELEMENT_BUTTON_SIZE CVector2(200.0f, 100.0f)
// �����I���{�^���̍��W
#define SELECT_ELEMENT_BUTTON_POS CVector2(WINDOW_WIDTH * 0.25f, WINDOW_HEIGHT * 0.25)

// �{�^���̃e�L�X�g�T�C�Y
#define BUTTON_TEXT_SIZE 32

// �摜�̃p�X
#define PATH "UI/white.png"

// �A�C�R���̃T�C�Y
#define ICON_SIZE CVector2(200.0f,200.0f)
// �v���C���[�A�C�R���̍��W
#define PLAYER_ICON_POS CVector2(WINDOW_WIDTH * 0.25f, WINDOW_HEIGHT * 0.5f)
// �G�A�C�R���̍��W
#define ENEMY_ICON_POS CVector2(WINDOW_WIDTH * 0.75f, WINDOW_HEIGHT * 0.5f)
// �Z�k�r�����I���̎��̃A�C�R���̃I�t�Z�b�g���W
#define ICON_QUICK_OFFSETPOS CVector2(0.0f, WINDOW_HEIGHT * 0.25f)

// �R���X�g���N�^
CSelectMenu::CSelectMenu()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// �t�H���g�f�[�^�𐶐�
	mpFont = new CFont("res\\Font\\toroman.ttf");
	mpFont->SetFontSize(BUTTON_TEXT_SIZE);
	mpFont->SetAlignment(FTGL::TextAlignment::ALIGN_CENTER);
	mpFont->SetLineLength(WINDOW_WIDTH);

	// �w�i�̃C���[�W�𐶐�
	mpBg = new CImage
	(
		"UI/title_bg.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpBg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// [BattleStart]�{�^���𐶐�
	CExpandButton* btn1 = new CExpandButton
	(
		BATTLE_START_BUTTON_POS,
		BATTLE_START_BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �摜��ǂݍ���
	btn1->LoadButtonImage(PATH, PATH);
	// �����̓ǂݍ���
	btn1->LoadButtonText(nullptr, BUTTON_TEXT_SIZE, "BattleStart");
	// �N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn1->SetOnClickFunc(std::bind(&CSelectMenu::OnClickBattleStart, this));
	// �ŏ��͖���
	btn1->SetEnable(false);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn1);

	// [Return]�{�^���𐶐�
	CExpandButton* btn2 = new CExpandButton
	(
		RETURN_BUTTON_POS,
		RETURN_BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �摜��ǂݍ���
	btn2->LoadButtonImage(PATH, PATH);
	// �����̓ǂݍ���
	btn2->LoadButtonText(nullptr, BUTTON_TEXT_SIZE, "Return");
	// �N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn2->SetOnClickFunc(std::bind(&CSelectMenu::OnClickReturn, this));
	// �ŏ��͖���
	btn2->SetEnable(false);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn2);

	// [SelectElement]�{�^���𐶐�
	CExpandButton* btn3 = new CExpandButton
	(
		SELECT_ELEMENT_BUTTON_POS,
		SELECT_ELEMENT_BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �摜��ǂݍ���
	btn3->LoadButtonImage(PATH, PATH);
	// �����̓ǂݍ���
	btn3->LoadButtonText(nullptr, BUTTON_TEXT_SIZE, "Select\nElement");
	// �N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn3->SetOnClickFunc(std::bind(&CSelectMenu::OnClickSelectElement, this));
	// �ŏ��͖���
	btn3->SetEnable(false);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn3);

	// �v���C���[�̃A�C�R���̉摜�𐶐�
	mpPlayerImg = new CImage
	(
		PATH,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,false
	);
	// �A�C�R���̃T�C�Y��ݒ�
	mpPlayerImg->SetSize(ICON_SIZE);
	// �A�C�R���̍��W��ݒ�
	mpPlayerImg->SetPos(PLAYER_ICON_POS - mpPlayerImg->GetSize() * 0.5f);

	// �X�N���[���ł���UI
	mpScrollUI = new CScrollUIBase(
		CVector2(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.3f),
		CVector2(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.3f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
}

// �f�X�g���N�^
CSelectMenu::~CSelectMenu()
{
	SAFE_DELETE(mpFont);
	SAFE_DELETE(mpBg);
	SAFE_DELETE(mpScrollUI);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// �Z���N�g��ʂ��I����
bool CSelectMenu::IsEnd() const
{
	return mIsEnd;
}

// �ΐ���J�n���邩
bool CSelectMenu::IsBattleStart() const
{
	// �I�����ڂ�1�ڂȂ�΁A�ΐ�J�n
	return mSelectIndex == 0;
}

// ��O�ɖ߂邩
bool CSelectMenu::IsReturn() const
{
	// �I�����ڂ�2�ڂȂ�΁A�߂�
	return mSelectIndex == 1;
}

// [�ΐ�J�n]�N���b�N���̃R�[���o�b�N�֐�
void CSelectMenu::OnClickBattleStart()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

// [�߂�]�N���b�N���̃R�[���o�b�N�֐�
void CSelectMenu::OnClickReturn()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}

// [SelectElement]�N���b�N���̃R�[���o�b�N�֐�
void CSelectMenu::OnClickSelectElement()
{
}

// �X�V
void CSelectMenu::Update()
{
	switch (mState)
	{
	case CSelectMenu::EState::eOpen:	UpdateOpen();		break;
	case CSelectMenu::EState::eSelect:	UpdateSelect();		break;
	case CSelectMenu::EState::eFadeOut:	UpdateFadeOut();	break;
	}

	mpBg->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
	mpScrollUI->Update();
}

// �`��
void CSelectMenu::Render()
{
	// �w�i�`��
	mpBg->Render();

	// �{�^���`��
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}

	// �A�C�R���`��
	mpPlayerImg->Render();

	// �X�N���[���ł���UI
	mpScrollUI->Render();
}

// ���j���[���J��
void CSelectMenu::UpdateOpen()
{
	// ���j���[�����̑҂�
	if (mElapsedTime < OPENED_WAIT_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// ��莞�ԑ҂�����A�{�^�����I���ɂ��ă^�b�`�ł���悤�ɂ���
		// �i��^�b�`��h�����߂̑҂����ԁj
		for (CExpandButton* btn : mButtons)
		{
			btn->SetEnable(true);
		}
		ChangeState(EState::eSelect);
	}
}

// ���j���[��I��
void CSelectMenu::UpdateSelect()
{
}

// �t�F�[�h�A�E�g
void CSelectMenu::UpdateFadeOut()
{
}

// ��Ԑ؂�ւ�
void CSelectMenu::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}
