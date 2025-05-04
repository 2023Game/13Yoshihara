#include "CGameMenuBase.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CSoundManager.h"
#include "CTextUI2D.h"
#include "Easing.h"
#include "CSceneManager.h"

#define MENU_ALPHA 0.75f

// ���ʉ��̉���
#define SE_VOLUME 0.5f

// �T�C�Y�̔{��
#define SIZE_RATIO 1.5f
// �{�^���̃T�C�Y
#define BUTTON_SIZE CVector2(340.0f, 96.0f)*1.5f
// �{�^���̃I�t�Z�b�g���W
#define BUTTON_OFFSET_POSX 200.0f

// ���j���[�̃A�j���[�V��������
#define OPEN_ANIM_TIME 0.25f
// ���j���[�̃A�j���[�V������̑҂�����
#define OPENED_WAIT_TIME 0.1f

// �e�L�X�g�̃I�t�Z�b�g���W
#define TEXT_OFFSET_POSY 50.0f

// �R���X�g���N�^
CGameMenuBase::CGameMenuBase(std::vector<std::string> menuItemPathList)
	: CTask(ETaskPriority::eUI, 1, ETaskPauseType::eMenu)
	, mIsOpened(false)
	, mpPreMenu(nullptr)
	, mIsNextMenu(false)
	, mElapsedTime(0.0f)
{
	// �ҋ@���
	ChangeState(EState::eIdle);
	// ���ʉ�
	/*mpSelectSE = CResourceManager::Get<CSound>("SelectSE");
	mpPushSE = CResourceManager::Get<CSound>("PushSE");
	mpBuzzerSE = CResourceManager::Get<CSound>("BuzzerSE");*/

	int menuItemMax = menuItemPathList.size();

	mpBackground = new CImage
	(
		"UI/menu_back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackground->SetSize(mpBackground->GetSize() * SIZE_RATIO);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(1.0f, 1.0f, 1.0f, MENU_ALPHA);


	int menuItemCount = menuItemMax;
	float spaceY = (float)WINDOW_HEIGHT / (menuItemCount + 1);
	for (int i = 0; i < menuItemCount; i++)
	{
		// �{�^���𐶐�
		CExpandButton* btn = new CExpandButton
		(
			CVector2(0.0f, 0.0f),
			BUTTON_SIZE,
			ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
			false, false
		);
		// ���W��ݒ�
		float posX = (1280.0f - 1024.0f + btn->GetSize().X()) * 0.5f + BUTTON_OFFSET_POSX;
		btn->SetPos(posX, spaceY * (i + 1));
		// �{�^���̉摜��ǂݍ���
		btn->LoadButtonImage(menuItemPathList[i], menuItemPathList[i]);
		// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
		btn->SetEnable(false);
		btn->SetScale(0.0f);
		// �Ō�̗v�f�Ȃ�
		if (i == menuItemCount - 1)
		{
			btn->SetOnClickFunc(std::bind(&CGameMenuBase::OnClickClose, this));
		}
		// �{�^�����X�g�ɒǉ�
		mButtons.push_back(btn);
		mMenuOnOff.push_back(true);

		// �e�L�X�g�𐶐�
		CTextUI2D* text = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
		text->Position(WINDOW_WIDTH * 0.5f, spaceY * (i + 1) - TEXT_OFFSET_POSY, 0.0f);
		mMenuTexts.push_back(text);
	}

	SetEnable(false);
	SetShow(false);
}

// �f�X�g���N�^
CGameMenuBase::~CGameMenuBase()
{
}

// ���j���[���J��
void CGameMenuBase::Open()
{
	SetEnable(true);
	SetShow(true);
	mIsOpened = true;
	mElapsedTime = 0.0f;
	if (mpPreMenu == nullptr)
	{
		CBGMManager::Instance()->Play(EBGMType::eMenu, false, 0.25f);
		CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
		// ���j���[�̓J�[�\���\��
		CInput::ShowCursor(true);
	}
	// ���j���[���J�����
	ChangeState(EState::eOpen);
	for (CButton* btn : mButtons)
	{	
		// �{�^���͖��������āA�X�P�[���l��0�ɂ��Ă���
		btn->SetEnable(false);
		btn->SetScale(0.0f);
	}
}

// ���j���[�����
void CGameMenuBase::Close()
{
	SetEnable(false);
	SetShow(false);
	mIsOpened = false;

	if (mpPreMenu == nullptr)
	{
		CBGMManager::Instance()->Play(EBGMType::eGame, false, 0.25f);
		CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
		// �J�[�\����\��
		CInput::ShowCursor(false);
	}	// �ҋ@���
	ChangeState(EState::eIdle);
}

// ���j���[���J���Ă��邩�ǂ���
bool CGameMenuBase::IsOpened() const
{
	return mIsOpened;
}

// �X�V
void CGameMenuBase::Update()
{
	switch (mState)
	{
		// �ҋ@
	case EState::eIdle:
		UpdateIdle();
		break;
		// ���j���[���J��
	case EState::eOpen:
		UpdateOpen();
		break;
		// ���j���[�I��
	case EState::eSelect:
		UpdateSelect();
		break;
	}

	mpBackground->Update();
	// �A�C�e���ƃe�L�X�g�̃T�C�Y�͈ꏏ
	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		mButtons[i]->Update();
		mMenuTexts[i]->Update();
	}
}

// �`��
void CGameMenuBase::Render()
{
	mpBackground->Render();
	// �A�C�e���ƃe�L�X�g�̃T�C�Y�͈ꏏ
	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CExpandButton* btn = mButtons[i];
		CTextUI2D* text = mMenuTexts[i];
		// �I�t�̏ꍇ�Â�����
		if (!mMenuOnOff[i])
		{
			btn->SetColor(0.1f, 0.1f, 0.1f);
		}
		// �I���̏ꍇ���邭����
		else
		{
			btn->SetColor(1.0f, 1.0f, 1.0f);
		}
		btn->Render();
		text->Render();
	}
}

// ���j���[�̗v�f�̗L��������ݒ�
void CGameMenuBase::SetMenuOnOff(int num, bool isOnOff)
{
	mMenuOnOff[num] = isOnOff;
}

// ��O�̃��j���[��ݒ�
void CGameMenuBase::SetPreMenu(CGameMenuBase* preMenu)
{
	mpPreMenu = preMenu;
}

// �L�������̐؂�ւ�
void CGameMenuBase::SetOnOff(bool isOnOff)
{
	SetEnable(isOnOff);
	SetShow(isOnOff);
}

// �ҋ@
void CGameMenuBase::UpdateIdle()
{
}

// ���j���[���J��
void CGameMenuBase::UpdateOpen()
{
	switch (mStateStep)
	{
		// �X�e�b�v0�F���j���[�̓���A�j���[�V����
	case 0:
		if (mElapsedTime < OPEN_ANIM_TIME)
		{
			// �X�P�[���l����U1.0���傫�����āA1.0�֖߂�C�[�W���O�A�j���[�V����
			float scale = Easing::BackOut(mElapsedTime, OPEN_ANIM_TIME, 0.0f, 1.0f, 2.0f);
			for (CExpandButton* btn : mButtons)
			{
				btn->SetScale(scale);
			}
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			for (CExpandButton* btn : mButtons)
			{
				btn->SetScale(1.0f);
			}
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
			for (CExpandButton* btn : mButtons)
			{
				btn->SetEnable(true);
			}
			ChangeState(EState::eSelect);
		}
		break;
	}
}

// ���j���[�I��
void CGameMenuBase::UpdateSelect()
{
}

// ��Ԑ؂�ւ�
void CGameMenuBase::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// [CLOSE]�N���b�N���̃R�[���o�b�N�֐�
void CGameMenuBase::OnClickClose()
{
	// �v�b�V����
	//mpPushSE->Play(SE_VOLUME, true);
	
	// �O�̃��j���[������Ȃ炻���L��
	if (mpPreMenu != nullptr)
	{
		mpPreMenu->SetOnOff(true);
	}
	Close();
}
