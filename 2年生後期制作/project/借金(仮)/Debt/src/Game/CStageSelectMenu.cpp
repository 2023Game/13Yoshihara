#include "CStageSelectMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_STAGE_TRASH "UI/menu_stage_trash.png"
#define MENU_STAGE_DELIVERY "UI/menu_stage_delivery.png"

#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// 効果音の音量
#define SE_VOLUME 1.0f

// 説明テキストの内容
#define TRASH_TRUE_TEXT		"ゴミ拾いを選択する\n（解放済）\n"
#define TRASH_FALSE_TEXT	"ゴミ拾いを選択する\n（未解放）\n"
#define DELIVERY_TRUE_TEXT	"配達を選択する\n（解放済）\n"
#define DELIVERY_FALSE_TEXT	"配達を選択する\n（未解放）\n"

// コンストラクタ
CStageSelectMenu::CStageSelectMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_STAGE_TRASH, MENU_STAGE_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mpPrevMenu = prevMenu;
	// ゲームはすべて無効
	SetMenuOnOff(0, false);
	SetMenuOnOff(1, false);

	// 説明テキストの設定
	mMenuTexts[0]->SetStr(TRASH_FALSE_TEXT);
	mMenuTexts[1]->SetStr(DELIVERY_FALSE_TEXT);
}

// デストラクタ
CStageSelectMenu::~CStageSelectMenu()
{
}

// 決定処理
void CStageSelectMenu::Decide(int select)
{
	switch (select)
	{
	case 0:
		// ゴミ拾いが有効なら
		if (mMenuOnOff[select])
		{
			// 選択されている仕事をゴミ拾いに設定
			CJobStatusManager::Instance()->SetSelectJob(EJobType::eTrash);
		}
		// 無効ならブザー音
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;
	case 1:
		// 配達が有効なら
		if (mMenuOnOff[select])
		{
			// 選択されている仕事を配達に設定
			CJobStatusManager::Instance()->SetSelectJob(EJobType::eDelivery);
		}
		// 無効ならブザー音
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;
	default:	// 一つ前のメニューに戻る
		Close();
		mpPrevMenu->Open();
		break;
	}
}

// 更新
void CStageSelectMenu::Update()
{
	CGameMenuBase::Update();
	// ゴミ拾いがアンロック済みだったら
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eTrash))
	{
		// 有効
		SetMenuOnOff(0, true);
		// ゴミ拾いを選択中のテキストに変更
		mMenuTexts[0]->SetStr(TRASH_TRUE_TEXT);
	}
	// 配達がアンロック済みだったら
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
	{
		// 有効
		SetMenuOnOff(1, true);
		// 配達を解放済みのテキストに変更
		mMenuTexts[1]->SetStr(DELIVERY_TRUE_TEXT);
	}
}
