#include "CHomeScene.h"
#include "CSceneManager.h"
#include "CHomeField.h"
#include "CHomePlayer.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CToDoUI.h"
#include "CSelectJobUI.h"
#include "CJobStatusManager.h"
#include "JobType.h"
#include "CMoneyUI.h"
#include "CDebtMoneyUI.h"
#include "CTaskManager.h"

// ��邱�Ƃ̃e�L�X�g
#define TODO_TEXT0 "PC����d����I������"
#define TODO_TEXT1 "�h�A����d���֍s��"

//�R���X�g���N�^
CHomeScene::CHomeScene()
	: CSceneBase(EScene::eHome)
	, mpGameMenu(nullptr)
{
}

//�f�X�g���N�^
CHomeScene::~CHomeScene()
{
}

//�V�[���ǂݍ���
void CHomeScene::Load()
{
	// �Q�[����ʂ̓J�[�\����\��
	CInput::ShowCursor(false);
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��
	CResourceManager::Load<CModelX>(	"Player",	"Character\\Player\\player.x");

	// ���_
	CResourceManager::Load<CModel>(		"HomeBase",								"Field\\HomeBase\\HomeBase.obj");
	CResourceManager::Load<CModel>(		"Bed",									"Field\\Object\\Bed.obj");
	CResourceManager::Load<CModel>(		"Chair",								"Field\\Object\\Chair.obj");
	CResourceManager::Load<CModel>(		"Computer",								"Field\\Object\\Computer.obj");
	CResourceManager::Load<CModel>(		"Desk",									"Field\\Object\\Desk.obj");
	CResourceManager::Load<CModel>(		"Door",									"Field\\Object\\Door.obj");
	CResourceManager::Load<CModel>(		"Rack",									"Field\\Object\\Rack.obj");
	// �����蔻��p�̃R���W�������f��
	CResourceManager::Load<CModel>(		"HomeBase_Ground&Ceiling_Collision",	"Field\\HomeBase\\CollisionModel\\HomeBase_Ground&Ceiling_Collision.obj");
	CResourceManager::Load<CModel>(		"HomeBase_Wall_Collision",				"Field\\HomeBase\\CollisionModel\\HomeBase_Wall_Collision.obj");
	CResourceManager::Load<CModel>(		"HomeBase_Object_Collision",			"Field\\HomeBase\\CollisionModel\\HomeBase_Object_Collision.obj");
	/*
	���ʉ�
	*/
	CResourceManager::Load<CSound>(		"SelectSE",		"Sound\\SE\\MenuSound\\select.wav");
	CResourceManager::Load<CSound>(		"PushSE",		"Sound\\SE\\MenuSound\\push.wav");
	CResourceManager::Load<CSound>(		"BuzzerSE",		"Sound\\SE\\MenuSound\\buzzer.wav");

	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eHome);

	CHomeField* field = new CHomeField();

	CHomePlayer* player = new CHomePlayer();
	player->Scale(1.0f, 1.0f, 1.0f);

	// ��邱�ƕ\��UI�𐶐�
	mpToDoUI = new CToDoUI();
	mpToDoUI->AddText(TODO_TEXT0);
	mpToDoUI->AddText(TODO_TEXT1);

	// �I�𒆂̎d�����\��UI�𐶐�
	mpSelectJobUI = new CSelectJobUI();

	// ������UI
	mpMoneyUI = new CMoneyUI();

	// �ԍϊzUI
	CDebtMoneyUI* debtMoneyUI = new CDebtMoneyUI();

	// CGameCamera�̃e�X�g
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);

	// CGameCamera2�̃e�X�g
	CVector atPos = player->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 10.0f, 15.0f),
		atPos
	);
	// �Փ˔��肷��R���C�_�\��ǉ�
	mainCamera->AddCollider(field->GetGroundCol());
	mainCamera->AddCollider(field->GetWallCol());
	mainCamera->AddCollider(field->GetObjCol());

	mainCamera->SetFollowTargetTf(player);

	// �Q�[�����j���[���쐬
	mpGameMenu = new CMenu();
}

//�V�[���̍X�V����
void CHomeScene::Update()
{
#if _DEBUG
	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}
#endif

	// ���j���[�|�[�Y�łȂ����
	if(!CTaskManager::Instance()->IsPaused(PAUSE_MENU_OPEN))
	{
		// �Q�[�����j���[���J���ĂȂ���΁A[TAB]�L�[�Ń��j���[���J��
		if (!mpGameMenu->IsOpened())
		{
			if (CInput::PushKey(VK_TAB))
			{
				mpGameMenu->Open();
			}
		}
	}
	EJobType jobType = CJobStatusManager::Instance()->GetSelectJob();
	// �d�����I������Ă��Ȃ��ꍇ
	if (jobType == EJobType::eNone)
	{
		mpToDoUI->SetText(0);
	}
	// �d�����I������Ă���ꍇ
	else
	{
		mpToDoUI->SetText(1);
	}
	// �I�𒆂̎d�����\��UI�̍X�V
	mpSelectJobUI->Update();
	// ������UI�̍X�V
	mpMoneyUI->Update();
}
