#include "CGameMenuBase.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"

#define MENU_ALPHA 0.75f
// �傫���̔{��
#define SIZE_RATIO 1.5f

// ���ʉ��̉���
#define SE_VOLUME 1.0f

// ��v�b�V�����p�̑ҋ@����
#define WAIT_TIME 0.1f

// �R���X�g���N�^
CGameMenuBase::CGameMenuBase(std::vector<std::string> menuItemPathList, std::string menuSelectPath)
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mSelectIndex(0)
	, mIsOpened(false)
	, mpPrevMenu(nullptr)
	, mElapsedTime(0.0f)
{
	// ���ʉ�
	mpSelectSE = CResourceManager::Get<CSound>("SelectSE");
	mpPushSE = CResourceManager::Get<CSound>("PushSE");
	mpBuzzerSE = CResourceManager::Get<CSound>("BuzzerSE");

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
		CImage* item = new CImage
		(
			menuItemPathList[i].c_str(),
			ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
			false, false
		);
		item->SetSize(item->GetSize() * SIZE_RATIO);
		item->SetCenter(item->GetSize() * 0.5f);
		float posX = (1280.0f - 1024.0f + item->GetSize().X()) * 0.5f + 100.0f;
		item->SetPos(posX, spaceY * (i + 1));
		item->SetColor(1.0f, 1.0f, 1.0f, MENU_ALPHA);
		mMenuItems.push_back(item);
		mMenuOnOff.push_back(true);

		CText* text = new CText
		(
			nullptr, 40,
			CVector::zero,
			CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
			CColor::white,
			ETaskPriority::eUI,
			0,
			ETaskPauseType::eGame,
			false, false
		);
	}

	mpSelectFrame = new CImage
	(
		menuSelectPath.c_str(),
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpSelectFrame->SetSize(mpSelectFrame->GetSize() * SIZE_RATIO);
	mpSelectFrame->SetCenter(mpSelectFrame->GetSize() * 0.5f);
	mpSelectFrame->SetColor(1.0f, 0.5f, 0.0f, MENU_ALPHA);


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
	mSelectIndex = 0;
	mElapsedTime = 0.0f;
	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

// ���j���[�����
void CGameMenuBase::Close()
{
	SetEnable(false);
	SetShow(false);
	CBGMManager::Instance()->Play(EBGMType::eHome, false);
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
}

// ���j���[���J���Ă��邩�ǂ���
bool CGameMenuBase::IsOpened() const
{
	return mIsOpened;
}

//�@���肵���{�^���̏���
void CGameMenuBase::Decide(int select)
{
}

// �X�V
void CGameMenuBase::Update()
{
	// �ҋ@���Ԃ��o�߂��Ă���Ȃ瑀��\
	if (mElapsedTime >= WAIT_TIME)
	{
		int itemCount = mMenuItems.size();
		if (CInput::PushKey('W'))
		{
			mSelectIndex = (mSelectIndex + itemCount - 1) % itemCount;
			// �Z���N�g�����Đ�
			mpSelectSE->Play(SE_VOLUME, true);
		}
		else if (CInput::PushKey('S'))
		{
			mSelectIndex = (mSelectIndex + 1) % itemCount;
			// �Z���N�g�����Đ�
			mpSelectSE->Play(SE_VOLUME, true);
		}
		else if (CInput::PushKey(VK_SPACE))
		{
			Decide(mSelectIndex);
			// ���艹���Đ�
			mpPushSE->Play(SE_VOLUME, true);
		}
	}
	// �o�߂��Ă��Ȃ��Ȃ�
	else
	{
		mElapsedTime += Times::DeltaTime();
	}

	mpBackground->Update();
	for (CImage* item : mMenuItems)
	{
		item->Update();
	}
	mpSelectFrame->Update();
}

// �`��
void CGameMenuBase::Render()
{
	mpBackground->Render();
	for (int i = 0; i < mMenuItems.size(); i++)
	{
		CImage* item = mMenuItems[i];
		// �I�t�̏ꍇ�Â�����
		if (!mMenuOnOff[i])
		{
			item->SetColor(0.1f, 0.1f, 0.1f);
		}
		// �I���̏ꍇ���邭����
		else
		{
			item->SetColor(1.0f, 1.0f, 1.0f);
		}
		item->Render();

		if (i == mSelectIndex)
		{
			mpSelectFrame->SetPos(item->GetPos());
			mpSelectFrame->Render();
		}
	}
}

// ���j���[�̗v�f�̗L��������ݒ�
void CGameMenuBase::SetMenuOnOff(int num, bool isOnOff)
{
	mMenuOnOff[num] = isOnOff;
}
