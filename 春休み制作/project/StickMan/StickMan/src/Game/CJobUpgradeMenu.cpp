#include "CJobUpgradeMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_UPGRADE_TRASH "UI/menu_upgrade_trash.png"
#define MENU_UPGRADE_DELIVERY "UI/menu_upgrade_delivery.png"

#define MENU_CLOSE "UI/menu_close.png"
#define UPGRADE_FLOAT 0.1f

// コンストラクタ
CJobUpgradeMenu::CJobUpgradeMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UPGRADE_TRASH, MENU_UPGRADE_DELIVERY, MENU_CLOSE})
{
	mpPreMenu = prevMenu;

	// 最後の要素以外のクリック時のコールバック関数を設定
	mButtons[0]->SetOnClickFunc(std::bind(&CJobUpgradeMenu::OnClickTrash, this));
	mButtons[1]->SetOnClickFunc(std::bind(&CJobUpgradeMenu::OnClickDelivery, this));
}

// デストラクタ
CJobUpgradeMenu::~CJobUpgradeMenu()
{
}

// 更新
void CJobUpgradeMenu::Update()
{
	CGameMenuBase::Update();
}

// [TRASH]クリック時のコールバック関数
void CJobUpgradeMenu::OnClickTrash()
{
	// ゴミ拾いを強化
	CJobStatusManager::Instance()->SetBonus(EJobType::eTrash, UPGRADE_FLOAT);
	// TODO : 強化音声を再生
}

// [DELIVERY]クリック時のコールバック関数
void CJobUpgradeMenu::OnClickDelivery()
{
	// 配達を強化
	CJobStatusManager::Instance()->SetBonus(EJobType::eDelivery, UPGRADE_FLOAT);
	// TODO : 強化音声を再生
}
