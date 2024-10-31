#include "CStageMenu.h"
#include "CSceneManager.h"

CStageMenu::CStageMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath)
	: CGameMenuBase(menuItemPathList,menuSelectPath)
{

}

CStageMenu::~CStageMenu()
{
}

void CStageMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// �ֈړ�
		Close();
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
		break;
	case 1:		// �ֈړ�
		Close();
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
		break;
	default:	// ��ԉ��̓��j���[�����
		Close();
		break;
	}
}

void CStageMenu::Update()
{
	CGameMenuBase::Update();
}
