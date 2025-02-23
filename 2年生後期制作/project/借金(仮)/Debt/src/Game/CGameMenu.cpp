#include "CGameMenu.h"
#include "CSceneManager.h"

#define MENU_ITEM1 "UI/menu_title.png"
#define MENU_ITEM2 "UI/menu_manual.png"
#define MENU_CLOSE "UI/menu_close.png"

// ���ʉ��̉���
#define SE_VOLUME 0.5f

// �R���X�g���N�^
CGameMenu::CGameMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_ITEM1, MENU_ITEM2, MENU_CLOSE})
	, mpManual(nullptr)
{
	SetSortOrder(3);
	// �Ō�̗v�f�ȊO�̃N���b�N���̃R�[���o�b�N�֐���ݒ�
	mButtons[0]->SetOnClickFunc(std::bind(&CGameMenu::OnClickTitle, this));
	mButtons[1]->SetOnClickFunc(std::bind(&CGameMenu::OnClickManual, this));
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

// ���������ݒ�
void CGameMenu::SetManual(CManualMenu* menu)
{
	mpManual = menu;
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

// [�������}�N���b�N���̃R�[���o�b�N�֐�
void CGameMenu::OnClickManual()
{
	// �v�b�V����
	mpPushSE->Play(SE_VOLUME, true);
	// �ݒ���J��
	if (mpManual != nullptr)
	{
		// ��O�̃��j���[��ݒ�
		mpManual->SetPreMenu(this);
		// ��������
		Close();
		// �J��
		mpManual->Open();
	}
}
