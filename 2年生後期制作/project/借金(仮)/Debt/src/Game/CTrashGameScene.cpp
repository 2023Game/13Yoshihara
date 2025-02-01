#include "CTrashGameScene.h"
#include "CSceneManager.h"
#include "CTrashField.h"
#include "CTrashPlayer.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CVehicleManager.h"
#include "CResidentManager.h"
#include "CTrashEnemy.h"
#include "CNavManager.h"
#include "CTimeUI.h"
#include "CTrashScoreUI.h"
#include "CScoreManager.h"
#include "CTaskManager.h"

// ��������
#define MAX_TIME 100
// ���U���g�V�[���֍s���O�̑ҋ@����
#define IDLE_TIME 1.0f

//�R���X�g���N�^
CTrashGameScene::CTrashGameScene()
	: CSceneBase(EScene::eTrashGame)
	, mpGameMenu(nullptr)
	, mpVehicleManager(nullptr)
	, mpResidentManager(nullptr)
	, mElapsedTime(0.0f)
	, mIsPause(false)
{
}

//�f�X�g���N�^
CTrashGameScene::~CTrashGameScene()
{
}

//�V�[���ǂݍ���
void CTrashGameScene::Load()
{
	// �Q�[����ʂ̓J�[�\����\��
	CInput::ShowCursor(false);
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��
	
	// CModelX
	CResourceManager::Load<CModelX>(	"TrashPlayer",					"Character\\TrashBox\\TrashBoxPlayer.x");
	CResourceManager::Load<CModelX>(	"TrashEnemy",					"Character\\TrashBox\\TrashBoxEnemy.x");
	CResourceManager::Load<CModelX>(	"Collector",					"Character\\Collector\\Fox.x");
	CResourceManager::Load<CModelX>(	"PunisherCollector",			"Character\\PunisherCollector\\PunisherFox.x");
	CResourceManager::Load<CModelX>(	"Resident",						"Character\\Resident\\Monkey.x");
	// CModel
	CResourceManager::Load<CModel>(		"TrashStage",					"Field\\TrashStage\\TrashStage.obj");
	CResourceManager::Load<CModel>(		"Sky",							"Field\\Sky\\Sky.obj");
	CResourceManager::Load<CModel>(		"TrashBox",						"Field\\Object\\TrashBox.obj");
	CResourceManager::Load<CModel>(		"Car",							"Character\\Car\\Car.obj");
	CResourceManager::Load<CModel>(		"GarbageTruck",					"Character\\GarbageTruck\\GarbageTruck.obj");
	CResourceManager::Load<CModel>(		"BlackTruck",					"Character\\BlackTruck\\BlackTruck.obj");
	CResourceManager::Load<CModel>(		"TrashBag",						"Field\\Object\\TrashBag.obj");
	CResourceManager::Load<CModel>(		"TrashBagGold",					"Field\\Object\\TrashBagGold.obj");
	// �����蔻��p�̃R���W�������f��
	CResourceManager::Load<CModel>(		"TrashStage_Ground_Collision",	"Field\\TrashStage\\CollisionModel\\TrashStage_Ground_Collision.obj");
	CResourceManager::Load<CModel>(		"TrashStage_Wall_Collision",	"Field\\TrashStage\\CollisionModel\\TrashStage_Wall_Collision.obj");
	CResourceManager::Load<CModel>(		"TrashStage_Object_Collision",	"Field\\TrashStage\\CollisionModel\\TrashStage_Object_Collision.obj");
	/*
	���ʉ�
	*/
	// �v���C���[�ƓG�̍U���A�ԗ����v���C���[�A�G�A�ԗ��Ƀ_���[�W��^�������̉�
	CResourceManager::Load<CSound>(		"DamageSE",						"Sound\\SE\\damage.wav");
	// �v���C���[�ƓG�̃N���e�B�J���U�����v���C���[�A�G�A�ԗ��Ƀ_���[�W��^�����Ƃ��̉�
	CResourceManager::Load<CSound>(		"CriticalSE",					"Sound\\SE\\critical.wav");
	// �v���C���[�ƓG�̊W�����Ă��鎞�ɍU����H�������
	CResourceManager::Load<CSound>(		"GuardSE",						"Sound\\SE\\guard.wav");
	// �v���C���[�̍U����������ɓ���������
	CResourceManager::Load<CSound>(		"CollectorDamageSE1",			"Sound\\SE\\collectorDamage.wav");


	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eHome);

	// �o�H�T���Ǘ��N���X���쐬
	new CNavManager();

	// �S�~�E���̃t�B�[���h�N���X���쐬
	CTrashField* field = new CTrashField();

	// �ԗ��̊Ǘ��N���X�쐬
	mpVehicleManager =
		new CVehicleManager();

	// �Z�l�̊Ǘ��N���X�쐬
	mpResidentManager =
		new CResidentManager();

	CTrashPlayer* player = new CTrashPlayer();

	CTrashEnemy* enemy = new CTrashEnemy(false);
	enemy->Position(0.0f, 0.0f, 5.0f);

	// ���ԕ\��UI�쐬
	mpTimeUI = new CTimeUI(MAX_TIME);
	// �X�R�A�\��UI�쐬
	mpTrashScoreUI = new CTrashScoreUI();

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
		atPos + CVector(0.0f, 0.0f, 50.0f),
		atPos
	);
	// �Փ˔��肷��R���C�_�\��ǉ�
	mainCamera->AddCollider(field->GetGroundCol());
	mainCamera->AddCollider(field->GetWallCol());
	mainCamera->AddCollider(field->GetObjCol());

	mainCamera->SetFollowTargetTf(player);

	// �Q�[�����j���[���쐬
	mpGameMenu = new CGameMenu();
}

//�V�[���̍X�V����
void CTrashGameScene::Update()
{
	// BGM�Đ����łȂ���΁ABGM���Đ�
	//if (!mpGameBGM->IsPlaying())
	//{
	//	mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
	//}

	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}

	// �Q�[�����j���[���J���ĂȂ���΁A[�l]�L�[�Ń��j���[���J��
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			mpGameMenu->Open();
		}
	}

	// �ԗ��Ǘ��N���X�̍X�V
	mpVehicleManager->Update();
	// ���ԕ\��UI�N���X�̕`��A�X�V
	mpTimeUI->Update();
	// �X�R�A�\��UI�N���X�̍X�V
	mpTrashScoreUI->Update();

	// �������Ԃ�0�ɂȂ�����
	if (mpTimeUI->GetTime() < 0)
	{
		// �Q�[�����|�[�Y
		CTaskManager::Instance()->Pause(PAUSE_GAME);
		// �ҋ@���Ԃ��o�߂����烊�U���g�V�[����
		mElapsedTime += Times::DeltaTime();
		if (mElapsedTime >= IDLE_TIME)
		{
			// �v���C���[�N���X���擾
			CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CTrashPlayer::Instance());
			// ���_�Ǘ��N���X���擾
			CScoreManager* scoreMgr = CScoreManager::Instance();
			// �V�[���Ǘ��N���X���擾
			CSceneManager* sceneMgr = CSceneManager::Instance();

			// �X�R�A�f�[�^��ݒ�
			scoreMgr->SetTrashGameScoreData(mpTrashScoreUI->GetScore(),
				player->GetTrashBag(), player->GetGoldTrashBag());
			// �Q�[���̎�ނ����̃V�[���ɐݒ�
			scoreMgr->SetGameType((int)sceneMgr->GetCurrentScene());
			// ���U���g�V�[���ֈڍs
			sceneMgr->LoadScene(EScene::eResult);
		}
	}
}
