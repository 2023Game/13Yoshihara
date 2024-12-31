#include "CGameMenuBase.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"

#define MENU_ALPHA 0.75f

// コンストラクタ
CGameMenuBase::CGameMenuBase(std::vector<std::string> menuItemPathList, std::string menuSelectPath)
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mSelectIndex(0)
	, mIsOpened(false)
	, mpPrevMenu(nullptr)
{
	int menuItemMax = menuItemPathList.size();

	mpBackground = new CImage
	(
		"UI/menu_back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
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
		item->SetCenter(item->GetSize() * 0.5f);
		float posX = (1280.0f - 1024.0f + item->GetSize().X()) * 0.5f + 100.0f;
		item->SetPos(posX, spaceY * (i + 1));
		item->SetColor(1.0f, 1.0f, 1.0f, MENU_ALPHA);
		mMenuItems.push_back(item);
	}

	mpSelectFrame = new CImage
	(
		menuSelectPath.c_str(),
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpSelectFrame->SetCenter(mpSelectFrame->GetSize() * 0.5f);
	mpSelectFrame->SetColor(1.0f, 0.5f, 0.0f, MENU_ALPHA);

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CGameMenuBase::~CGameMenuBase()
{
}

// メニューを開く
void CGameMenuBase::Open()
{
	SetEnable(true);
	SetShow(true);
	mSelectIndex = 0;
	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

// メニューを閉じる
void CGameMenuBase::Close()
{
	SetEnable(false);
	SetShow(false);
	CBGMManager::Instance()->Play(EBGMType::eHome, false);
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
}

// メニューが開いているかどうか
bool CGameMenuBase::IsOpened() const
{
	return mIsOpened;
}

//　決定したボタンの処理
void CGameMenuBase::Decide(int select)
{
}

// 更新
void CGameMenuBase::Update()
{
	int itemCount = mMenuItems.size();
	if (CInput::PushKey('W'))
	{
		mSelectIndex = (mSelectIndex + itemCount - 1) % itemCount;
		// TODO : セレクト音を再生
	}
	else if (CInput::PushKey('S'))
	{
		mSelectIndex = (mSelectIndex + 1) % itemCount;
		// TODO : セレクト音を再生
	}
	else if (CInput::PushKey(VK_SPACE))
	{
		Decide(mSelectIndex);
		// TODO : 決定音を再生
	}

	mpBackground->Update();
	for (CImage* item : mMenuItems)
	{
		item->Update();
	}
	mpSelectFrame->Update();
}

// 描画
void CGameMenuBase::Render()
{
	mpBackground->Render();
	for (int i = 0; i < mMenuItems.size(); i++)
	{
		CImage* item = mMenuItems[i];
		item->Render();

		if (i == mSelectIndex)
		{
			mpSelectFrame->SetPos(item->GetPos());
			mpSelectFrame->Render();
		}
	}
}
