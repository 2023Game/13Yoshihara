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

// �v���C���[�̏������W
#define PLAYER_POS CVector(0.0f,0.0f,50.0f)
// �G�̏������W
#define ENEMY_POS CVector(0.0f,0.0f,-50.0f)
// �G�̏�������
#define ENEMY_ROT CVector(0.0f,180.0f,0.0f)

//�R���X�g���N�^
CTestScene::CTestScene()
	: CSceneBase(EScene::eTrashGame)
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
	CResourceManager::Load<CModelX>("TrashPlayer", "Character\\TrashBox\\TrashBoxPlayer.x");
	CResourceManager::Load<CModelX>("TrashEnemy", "Character\\TrashBox\\TrashBoxEnemy.x");
	CResourceManager::Load<CModelX>("Collector", "Character\\Collector\\Fox.x");
	CResourceManager::Load<CModelX>("PunisherCollector", "Character\\PunisherCollector\\PunisherFox.x");
	CResourceManager::Load<CModelX>("Resident", "Character\\Resident\\Monkey.x");
	// CModel
	CResourceManager::Load<CModel>("TrashStage", "Field\\TrashStage\\TrashStage.obj");
	CResourceManager::Load<CModel>("Sky", "Field\\Sky\\Sky.obj");
	CResourceManager::Load<CModel>("TrashBox", "Field\\Object\\TrashBox.obj");
	CResourceManager::Load<CModel>("Car", "Character\\Car\\Car.obj");
	CResourceManager::Load<CModel>("GarbageTruck", "Character\\GarbageTruck\\GarbageTruck.obj");
	CResourceManager::Load<CModel>("BlackTruck", "Character\\BlackTruck\\BlackTruck.obj");
	CResourceManager::Load<CModel>("TrashBag", "Field\\Object\\TrashBag.obj");
	CResourceManager::Load<CModel>("TrashBagGold", "Field\\Object\\TrashBagGold.obj");
	// �����蔻��p�̃R���W�������f��
	CResourceManager::Load<CModel>("TrashStage_Ground_Collision", "Field\\TrashStage\\CollisionModel\\TrashStage_Ground_Collision.obj");
	CResourceManager::Load<CModel>("TrashStage_Wall_Collision", "Field\\TrashStage\\CollisionModel\\TrashStage_Wall_Collision.obj");
	CResourceManager::Load<CModel>("TrashStage_Object_Collision", "Field\\TrashStage\\CollisionModel\\TrashStage_Object_Collision.obj");
	/*
	���ʉ�
	*/
	// �v���C���[�ƓG�̍U���A�ԗ����v���C���[�A�G�A�ԗ��Ƀ_���[�W��^�������̉�
	CResourceManager::Load<CSound>("DamageSE", "Sound\\SE\\damage.wav");
	// �v���C���[�ƓG�̃N���e�B�J���U�����v���C���[�A�G�A�ԗ��Ƀ_���[�W��^�����Ƃ��̉�
	CResourceManager::Load<CSound>("CriticalSE", "Sound\\SE\\critical.wav");
	// �v���C���[�ƓG�̊W�����Ă��鎞�ɍU����H�������
	CResourceManager::Load<CSound>("GuardSE", "Sound\\SE\\guard.wav");
	// �v���C���[�̍U����������ɓ���������
	CResourceManager::Load<CSound>("CollectorDamageSE1", "Sound\\SE\\collectorDamage.wav");
	// �S�~���E������
	CResourceManager::Load<CSound>("GetSE", "Sound\\SE\\get.wav");
	// �x����
	CResourceManager::Load<CSound>("AlarmSE", "Sound\\SE\\alarm.wav");
	// ���j���[����
	CResourceManager::Load<CSound>("SelectSE", "Sound\\SE\\MenuSound\\select.wav");
	CResourceManager::Load<CSound>("PushSE", "Sound\\SE\\MenuSound\\push.wav");
	CResourceManager::Load<CSound>("BuzzerSE", "Sound\\SE\\MenuSound\\buzzer.wav");


	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eTrashGame);

	// �o�H�T���Ǘ��N���X���쐬
	new CNavManager();




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
	// �Փ˔��肷��R���C�_�\��ǉ�
	mainCamera->AddCollider(field->GetGroundCol());
	mainCamera->AddCollider(field->GetWallCol());
	mainCamera->AddCollider(field->GetObjCol());

	mainCamera->SetFollowTargetTf(player);

	// �Q�[�����j���[���쐬
	mpGameMenu = new CGameMenu();
	// ���������ݒ�
	mpGameMenu->SetManual(mpManual);
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

	// ���j���[�|�[�Y�łȂ����
	if (!CTaskManager::Instance()->IsPaused(PAUSE_MENU_OPEN))
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
}
