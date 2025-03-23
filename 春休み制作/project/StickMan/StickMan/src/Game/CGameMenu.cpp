#include "CGameMenu.h"
#include "CSceneManager.h"

#define MENU_ITEM1 "UI/menu_title.png"
#define MENU_CLOSE "UI/menu_close.png"

// ���ʉ��̉���
#define SE_VOLUME 0.5f

// �R���X�g���N�^
CGameMenu::CGameMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_ITEM1, MENU_CLOSE})
{
	// ���̃��j���[������
	mIsNextMenu = true;

	SetSortOrder(3);
	// �Ō�̗v�f�ȊO�̃N���b�N���̃R�[���o�b�N�֐���ݒ�
	mButtons[0]->SetOnClickFunc(std::bind(&CGameMenu::OnClickTitle, this));
}

// �f�X�g���N�^
CGameMenu::~CGameMenu()
{
}

// �X�V
void CGameMenu::Update()
{
	CGameMenuBase::Update();
}

// [�^�C�g����}�N���b�N���̃R�[���o�b�N�֐�
void CGameMenu::OnClickTitle()
{
	// �v�b�V����
	mpPushSE->Play(SE_VOLUME, true);
	// �^�C�g����
	Close();
	CSceneManager::Instance()->LoadScene(EScene::eTitle);
}
