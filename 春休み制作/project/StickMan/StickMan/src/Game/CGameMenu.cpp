#include "CGameMenu.h"
#include "CSceneManager.h"

#define MENU_ITEM1 "UI/menu_title.png"
#define MENU_CLOSE "UI/menu_close.png"

// 効果音の音量
#define SE_VOLUME 0.5f

// コンストラクタ
CGameMenu::CGameMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_ITEM1, MENU_CLOSE})
{
	// 次のメニューがある
	mIsNextMenu = true;

	SetSortOrder(3);
	// 最後の要素以外のクリック時のコールバック関数を設定
	mButtons[0]->SetOnClickFunc(std::bind(&CGameMenu::OnClickTitle, this));
}

// デストラクタ
CGameMenu::~CGameMenu()
{
}

// 更新
void CGameMenu::Update()
{
	CGameMenuBase::Update();
}

// [タイトルへ}クリック時のコールバック関数
void CGameMenu::OnClickTitle()
{
	// プッシュ音
	mpPushSE->Play(SE_VOLUME, true);
	// タイトルへ
	Close();
	CSceneManager::Instance()->LoadScene(EScene::eTitle);
}
