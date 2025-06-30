#include "CTutorialScene.h"
#include "CSceneManager.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CNavManager.h"
#include "CTaskManager.h"
#include "Maths.h"
#include "CResourceManager.h"
#include "CPlayer.h"
#include "CTutorialMap.h"

//�R���X�g���N�^
CTutorialScene::CTutorialScene()
	: CSceneBase(EScene::eTutorial)
	, mpGameMenu(nullptr)
{
}

//�f�X�g���N�^
CTutorialScene::~CTutorialScene()
{
}

//�V�[���ǂݍ���
void CTutorialScene::Load()
{
	// �Q�[����ʂ̓J�[�\����\��
	CInput::ShowCursor(false);
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��

	// CModelX
	CResourceManager::Load<CModelX>("Player", "Character\\Adventurer\\Adventurer.x", true);
	// CModel
	CResourceManager::Load<CModel>("TutorialMap",	"Field\\Map\\TutorialMap\\TutorialMap.obj");
	CResourceManager::Load<CModel>("Wand",			"Wepon\\Wand\\Wand.obj", true);
	CResourceManager::Load<CModel>("Box",			"Field\\Box\\Box.obj");
	CResourceManager::Load<CModel>("Door",			"Field\\Door\\Door.obj");
	CResourceManager::Load<CModel>("SwitchFrame",	"Field\\Switch\\Switch_Frame.obj");
	CResourceManager::Load<CModel>("SwitchButton",	"Field\\Switch\\Switch_Button.obj");
	CResourceManager::Load<CModel>("AirConnectObj", "Field\\AirConnectPoint\\AirConnectPoint.obj");
	// �����蔻��p�̃R���W�������f��
	CResourceManager::Load<CModel>("TutorialMap_Ground_Col",	"Field\\Map\\TutorialMap\\Col\\TutorialMap_Ground_Col.obj");
	CResourceManager::Load<CModel>("TutorialMap_Wall_Col",		"Field\\Map\\TutorialMap\\Col\\TutorialMap_Wall_Col.obj");
	CResourceManager::Load<CModel>("Door_Col",					"Field\\Door\\Col\\Door_Col.obj");
	CResourceManager::Load<CModel>("Box_Col",					"Field\\Box\\Col\\Box_Col.obj");
	/*
	���ʉ�
	*/

	// �Q�[��BGM��ǂݍ���
	// CBGMManager::Instance()->Play(EBGMType::eGame);

	// �v���C���[����
	CPlayer* player = new CPlayer();


	// CGameCamera2
	CVector atPos = player->Position() + CVector(0.0f, 20.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 20.0f),
		atPos
	);

	// �t�B�[���h�N���X����
	CTutorialMap* field = new CTutorialMap();

	//// �Փ˔��肷��R���C�_�\��ǉ�
	mainCamera->AddCollider(field->GetGroundCol());
	mainCamera->AddCollider(field->GetWallCol());

	mainCamera->SetFollowTargetTf(player);

	// �Q�[�����j���[���쐬
	mpGameMenu = new CGameMenu();
}

//�V�[���̍X�V����
void CTutorialScene::Update()
{
#if _DEBUG
	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}
#endif

	// �Q�[�����j���[���J���ĂȂ���΁A[TAB]�L�[�Ń��j���[���J��
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey(VK_TAB))
		{
			mpGameMenu->Open();
		}
	}
	// �Q�[�����j���[���J���Ă���΁A[TAB]�L�[�Ń��j���[�����
	else
	{
		if (CInput::PushKey(VK_TAB))
		{
			mpGameMenu->Close();
		}
	}
}
