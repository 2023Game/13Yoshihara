#include "CMenu.h"
#include "CSceneManager.h"

#define MENU_ITEM1 "UI/menu_title.png"
#define MENU_CLOSE "UI/menu_close.png"

// ���ʉ��̉���
#define SE_VOLUME 0.5f

// �R���X�g���N�^
CMenu::CMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_ITEM1, MENU_CLOSE})
{
	SetSortOrder(3);
	// �Ō�̗v�f�ȊO�̃N���b�N���̃R�[���o�b�N�֐���ݒ�
	mButtons[0]->SetOnClickFunc(std::bind(&CMenu::OnClickTitle, this));
}

// �f�X�g���N�^
CMenu::~CMenu()
{
}

// �X�V
void CMenu::Update()
{
	CGameMenuBase::Update();
}

// [�^�C�g����}�N���b�N���̃R�[���o�b�N�֐�
void CMenu::OnClickTitle()
{
	// �v�b�V����
	mpPushSE->Play(SE_VOLUME, true);
	// �^�C�g����
	Close();
	CSceneManager::Instance()->LoadScene(EScene::eTitle);
}