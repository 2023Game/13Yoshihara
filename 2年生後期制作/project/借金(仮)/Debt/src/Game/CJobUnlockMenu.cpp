#include "CJobUnlockMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"
#include "CTextUI2D.h"

#define MENU_UNLOCK_TRASH "UI/menu_unlock_trash.png"
#define MENU_UNLOCK_DELIVERY "UI/menu_unlock_delivery.png"

#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// 効果音の音量
#define SE_VOLUME 1.0f

// 説明テキストの内容
#define TRASH_UNLOCK_TRUE_TEXT		"ゴミ拾いを解放する\n（解放済）"
#define TRASH_UNLOCK_FALSE_TEXT		"ゴミ拾いを解放する\n（未解放）"
#define DELIVERY_UNLOCK_TRUE_TEXT	"配達を解放する\n（解放済）\n"
#define DELIVERY_UNLOCK_FALSE_TEXT	"配達を解放する\n（未解放）\n"

// コンストラクタ
CJobUnlockMenu::CJobUnlockMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UNLOCK_TRASH, MENU_UNLOCK_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mpPrevMenu = prevMenu;

	// 説明テキストの設定
	mMenuTexts[0]->SetStr(TRASH_UNLOCK_FALSE_TEXT);
	mMenuTexts[1]->SetStr(DELIVERY_UNLOCK_FALSE_TEXT);
}

// デストラクタ
CJobUnlockMenu::~CJobUnlockMenu()
{
}

// 決定処理
void CJobUnlockMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// ゴミ拾いをアンロック
		// 有効ならアンロック
		if (mMenuOnOff[select])
		{
			CJobStatusManager::Instance()->SetUnlock(EJobType::eTrash, true);
		}
		// 無効ならブザー音
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;
	case 1:		// 配達をアンロック
		// 有効ならアンロック
		if (mMenuOnOff[select])
		{
			CJobStatusManager::Instance()->SetUnlock(EJobType::eDelivery, true);
		}
		// 無効ならブザー音
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;
	default:	// 一つ前のメニューへ戻る
		Close();
		mpPrevMenu->Open();
		break;
	}
}

// 更新
void CJobUnlockMenu::Update()
{
	CGameMenuBase::Update();
	// ゴミ拾いがアンロック済みだったら
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eTrash))
	{
		// 無効
		SetMenuOnOff(0, false);
		// ゴミ拾いを解放済みテキストに変更
		mMenuTexts[0]->SetStr(TRASH_UNLOCK_TRUE_TEXT);
	}
	// 配達がアンロック済みだったら
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
	{
		// 無効
		SetMenuOnOff(1, false);
		// 配達を解放済みテキストに変更
		mMenuTexts[1]->SetStr(DELIVERY_UNLOCK_TRUE_TEXT);
	}
}
