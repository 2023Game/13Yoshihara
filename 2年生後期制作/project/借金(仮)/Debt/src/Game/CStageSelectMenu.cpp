#include "CStageSelectMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_STAGE_TRASH "UI/menu_stage_trash.png"
#define MENU_STAGE_DELIVERY "UI/menu_stage_delivery.png"

#define MENU_CLOSE "UI/menu_close.png"

// 効果音の音量
#define SE_VOLUME 0.5f

// 説明テキストの内容
#define TRASH_TRUE_TEXT		"ゴミ拾いを選択\n（解放済）\n"
#define TRASH_FALSE_TEXT	"ゴミ拾いを選択\n（未解放）\n"
#define TRASH_SELECT_TEXT	"ゴミ拾いを選択\n（選択済）\n"
#define DELIVERY_TRUE_TEXT		"配達を選択\n（解放済）\n"
#define DELIVERY_FALSE_TEXT		"配達を選択\n（未解放）\n"
#define DELIVERY_SELECT_TEXT	"配達を選択\n（選択済）\n"

// コンストラクタ
CStageSelectMenu::CStageSelectMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_STAGE_TRASH, MENU_STAGE_DELIVERY, MENU_CLOSE})
{
	mpPreMenu = prevMenu;
	// ゲームはすべて無効
	SetMenuOnOff(0, false);
	SetMenuOnOff(1, false);

	// 最後の要素以外のクリック時のコールバック関数を設定
	mButtons[0]->SetOnClickFunc(std::bind(&CStageSelectMenu::OnClickTrash, this));
	mButtons[1]->SetOnClickFunc(std::bind(&CStageSelectMenu::OnClickDelivery, this));
	// 説明テキストの設定
	mMenuTexts[0]->SetStr(TRASH_FALSE_TEXT);
	mMenuTexts[1]->SetStr(DELIVERY_FALSE_TEXT);
}

// デストラクタ
CStageSelectMenu::~CStageSelectMenu()
{
}

// 更新
void CStageSelectMenu::Update()
{
	CGameMenuBase::Update();
	CJobStatusManager* jobStatusMgr = CJobStatusManager::Instance();

	// ゴミ拾いを選択済みなら
	if (jobStatusMgr->GetSelectJob() == EJobType::eTrash)
	{
		// 無効
		SetMenuOnOff(0, false);
		// ゴミ拾いを選択済みのテキストに変更
		mMenuTexts[0]->SetStr(TRASH_SELECT_TEXT);
	}
	// ゴミ拾いがアンロック済みだったら
	else if (jobStatusMgr->GetUnlock(EJobType::eTrash))
	{
		// 有効
		SetMenuOnOff(0, true);
		// ゴミ拾いを解放済みのテキストに変更
		mMenuTexts[0]->SetStr(TRASH_TRUE_TEXT);
	}

	// 配達を選択済みなら
	if (jobStatusMgr->GetSelectJob() == EJobType::eDelivery)
	{
		// 無効
		SetMenuOnOff(1, false);
		// 配達を選択済みのテキストに変更
		mMenuTexts[1]->SetStr(DELIVERY_SELECT_TEXT);
	}
	// 配達がアンロック済みだったら
	else if (jobStatusMgr->GetUnlock(EJobType::eDelivery))
	{
		// 有効
		SetMenuOnOff(1, true);
		// 配達を解放済みのテキストに変更
		mMenuTexts[1]->SetStr(DELIVERY_TRUE_TEXT);
	}
}

// [TRASH]クリック時のコールバック関数
void CStageSelectMenu::OnClickTrash()
{
	// ゴミ拾いが有効なら
	if (mMenuOnOff[0])
	{
		// プッシュ音
		mpPushSE->Play(SE_VOLUME, true);
		// 選択されている仕事をゴミ拾いに設定
		CJobStatusManager::Instance()->SetSelectJob(EJobType::eTrash);
	}
	// 無効ならブザー音
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}

// [DELIVERY]クリック時のコールバック関数
void CStageSelectMenu::OnClickDelivery()
{
	// 配達が有効なら
	if (mMenuOnOff[1])
	{
		// プッシュ音
		mpPushSE->Play(SE_VOLUME, true);
		// 選択されている仕事を配達に設定
		CJobStatusManager::Instance()->SetSelectJob(EJobType::eDelivery);
	}
	// 無効ならブザー音
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}
