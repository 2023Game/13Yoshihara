#include "CTestScene.h"
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
#include "CFieldManager.h"
#include "CClassRoom.h"
#include "CTable.h"

#include "CMap_1.h"
#include "CMap_2.h"

//�R���X�g���N�^
CTestScene::CTestScene()
	: CSceneBase(EScene::eGameTest)
	, mpGameMenu(nullptr)
{
}

//�f�X�g���N�^
CTestScene::~CTestScene()
{
}

//�V�[���ǂݍ���
void CTestScene::Load()
{
	// �Q�[����ʂ̓J�[�\����\��
	CInput::ShowCursor(false);
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��

	// CModelX
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\Player.x");
	// CModel
	CResourceManager::Load<CModel>("TestField", "Field\\Field.obj");
	CResourceManager::Load<CModel>("ClassRoom", "Field\\Map\\ClassRoom.obj");
	CResourceManager::Load<CModel>("Table",		"Field\\Map\\SchoolTable.obj");
	CResourceManager::Load<CModel>("Book",		"Field\\Map\\Book.obj");
	// �����蔻��p�̃R���W�������f��
	CResourceManager::Load<CModel>("TableCol",	"Field\\Map\\Col\\SchoolTableCol.obj");
	/*
	���ʉ�
	*/
	


	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// �o�H�T���Ǘ��N���X���쐬
	new CNavManager();

	// �v���C���[����
	CPlayer* player = new CPlayer();

	// �����̐���
	CClassRoom* classRoom = new CClassRoom();
	// ���̐���
	CTable* table = new CTable();
	// �����̈ʒu�𒲐�
	classRoom->Position(0.0f, -110.0f * table->Scale().Y(), 0.0f);


	// �t�B�[���h�Ǘ��N���X����
	/*CFieldManager* fieldMgr = new CFieldManager();*/




	// CGameCamera�̃e�X�g
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);
	// 
	// CGameCamera2�̃e�X�g
	CVector atPos = player->Position() + CVector(0.0f, 8.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 10.0f, 50.0f),
		atPos
	);
	//// �Փ˔��肷��R���C�_�\��ǉ�
	//mainCamera->AddCollider(fieldMgr->GetGroundCol());
	//mainCamera->AddCollider(fieldMgr->GetWallCol());
	//mainCamera->AddCollider(fieldMgr->GetObjCol());

	mainCamera->SetFollowTargetTf(player);

	// �Q�[�����j���[���쐬
	mpGameMenu = new CGameMenu();
}

//�V�[���̍X�V����
void CTestScene::Update()
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
