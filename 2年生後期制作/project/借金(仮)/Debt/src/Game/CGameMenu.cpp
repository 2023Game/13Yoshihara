#include "CGameMenu.h"
#include "CSceneManager.h"

#define MENU_ITEM1 "UI/menu_item.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// �R���X�g���N�^
CGameMenu::CGameMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_ITEM1, MENU_ITEM1, MENU_CLOSE}, MENU_SELECT)
{
	
}

// �f�X�g���N�^
CGameMenu::~CGameMenu()
{
}

// ���菈��
void CGameMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// �^�C�g���ֈړ�
		Close();
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
		break;
	case 1:		// 
		Close();
		break;
	default:	// ��ԉ��̓��j���[�����
		Close();
		break;
	}
}

// �X�V
void CGameMenu::Update()
{
	CGameMenuBase::Update();
}
