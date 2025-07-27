#include "CMap1Scene.h"
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
#include "CWand.h"
#include "CMap1.h"
#include "CConnectPointManager.h"

// �R���X�g���N�^
CMap1Scene::CMap1Scene()
	: CSceneBase(EScene::eMap1)
	, mpGameMenu(nullptr)
{
}

// �f�X�g���N�^
CMap1Scene::~CMap1Scene()
{
}

// �V�[���ǂݍ���
void CMap1Scene::Load()
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
	CResourceManager::Load<CModel>("Map1",			"Field\\Map\\Map1\\Map1.obj");
	CResourceManager::Load<CModel>("Wand",			"Wepon\\Wand\\Wand.obj", true);
	CResourceManager::Load<CModel>("Box",			"Field\\Box\\Box.obj");
	CResourceManager::Load<CModel>("Door",			"Field\\Door\\Door.obj");
	CResourceManager::Load<CModel>("SwitchFrame",	"Field\\Switch\\Switch_Frame.obj");
	CResourceManager::Load<CModel>("SwitchButton",	"Field\\Switch\\Switch_Button.obj");
	CResourceManager::Load<CModel>("AirConnectObj", "Field\\AirConnectPoint\\AirConnectPoint.obj");
	CResourceManager::Load<CModel>("Torch",			"Field\\Torch\\Torch.obj");
	CResourceManager::Load<CModel>("Bridge",		"Field\\Bridge\\Bridge.obj");
	CResourceManager::Load<CModel>("CampFire",		"Field\\CampFire\\CampFire.obj");
	CResourceManager::Load<CModel>("DownWall",		"Field\\DownWall\\DownWall.obj");
	// �����蔻��p�̃R���W�������f��
	CResourceManager::Load<CModel>("Map1_Ground_Col",	"Field\\Map\\Map1\\Col\\Map1_Ground_Col.obj");
	CResourceManager::Load<CModel>("Map1_Wall_Col",		"Field\\Map\\Map1\\Col\\Map1_Wall_Col.obj");
	CResourceManager::Load<CModel>("Door_Col",			"Field\\Door\\Col\\Door_Col.obj");
	CResourceManager::Load<CModel>("Box_Col",			"Field\\Box\\Col\\Box_Col.obj");
	CResourceManager::Load<CModel>("Bridge_Col",		"Field\\Bridge\\Col\\Bridge_Col.obj");
	CResourceManager::Load<CModel>("CampFire_Col",		"Field\\CampFire\\Col\\CampFire_Col.obj");
	/*
	���ʉ�
	*/

	// �ڑ����Ǘ��N���X���擾
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// �Փ˔��������R���C�_�[�����Z�b�g
	pointMgr->ResetCollider();

	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// �v���C���[����
	CPlayer* player = new CPlayer();

	// CGameCamera2�̃e�X�g
	CVector atPos = player->Position() + CVector(0.0f, 20.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 20.0f),
		atPos
	);
	// �J�����̈ʒu�𔽑΂�
	mainCamera->SetRotateAngle(CVector(0.0f, 180.0f, 0.0f));

	// �t�B�[���h�N���X����
	mpField = new CMap1();

	// �Փ˔��肷��R���C�_�[��ǉ�
	pointMgr->AddCollider(mpField->GetGroundCol());
	pointMgr->AddCollider(mpField->GetWallCol());

	// �Փ˔��肷��R���C�_�\��ǉ�
	mainCamera->AddCollider(mpField->GetGroundCol());
	mainCamera->AddCollider(mpField->GetWallCol());
	mainCamera->AddCollider(mpField->GetObjCol());

	mainCamera->SetFollowTargetTf(player);

	// �Q�[�����j���[���쐬
	mpGameMenu = new CGameMenu();
}

// �V�[���̍X�V����
void CMap1Scene::Update()
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
