#include "CDeliveryGameScene.h"
#include "CGameCamera2.h"
#include "CGameMenu.h"
#include "CTaskManager.h"
#include "CSceneManager.h"
#include "CScoreManager.h"
#include "CResourceManager.h"
#include "CBGMManager.h"
#include "CNavManager.h"
#include "CTimeUI.h"
#include "CTrashScoreUI.h"
#include "CInput.h"
#include "Maths.h"
#include "CTrashField.h"
#include "CTrashPlayer.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemy.h"

// ��������
#define MAX_TIME 100
// ���U���g�V�[���֍s���O�̑ҋ@����
#define IDLE_TIME 1.0f

// �v���C���[�̏������W
#define PLAYER_POS CVector(0.0f,0.0f,50.0f)

//�R���X�g���N�^
CDeliveryGameScene::CDeliveryGameScene()
	: CSceneBase(EScene::eDeliveryGame)
	, mpGameMenu(nullptr)
	, mpTimeUI(nullptr)
	, mpTrashScoreUI(nullptr)
	, mElapsedTime(0.0f)
{
}

//�f�X�g���N�^
CDeliveryGameScene::~CDeliveryGameScene()
{
}

//�V�[���ǂݍ���
void CDeliveryGameScene::Load()
{
	// �Q�[����ʂ̓J�[�\����\��
	CInput::ShowCursor(false);
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	// CModelX
	CResourceManager::Load<CModelX>(	"TrashPlayer", "Character\\TrashBox\\TrashBoxPlayer.x");
	// CModel
	CResourceManager::Load<CModel>(		"TrashStage",		"Field\\TrashStage\\TrashStage.obj");
	CResourceManager::Load<CModel>(		"Sky",				"Field\\Sky\\Sky.obj");
	CResourceManager::Load<CModel>(		"DeliveryPlayer",	"Character\\DeliveryTruck\\DeliveryTruck_Player.obj");
	CResourceManager::Load<CModel>(		"DeliveryEnemy",	"Character\\DeliveryTruck\\DeliveryTruck_Enemy.obj");
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
	// ���j���[����
	CResourceManager::Load<CSound>("SelectSE", "Sound\\SE\\MenuSound\\select.wav");
	CResourceManager::Load<CSound>("PushSE", "Sound\\SE\\MenuSound\\push.wav");
	CResourceManager::Load<CSound>("BuzzerSE", "Sound\\SE\\MenuSound\\buzzer.wav");


	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eTrashGame);

	// �o�H�T���Ǘ��N���X���쐬
	new CNavManager();

	// �S�~�E���̃t�B�[���h�N���X���쐬
	CTrashField* field = new CTrashField();

	CTrashPlayer* player = new CTrashPlayer();
	player->Position(PLAYER_POS);

	CDeliveryPlayer* deliveryPlayer = new CDeliveryPlayer();
	deliveryPlayer->Position(-20.0f, 0.0f, 0.0f);
	CDeliveryEnemy* enemy = new CDeliveryEnemy();
	enemy->Position(20.0f, 0.0f, 0.0f);

	// ���ԕ\��UI����
	mpTimeUI = new CTimeUI(MAX_TIME);
	// �X�R�A�\��UI����
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
}

//�V�[���̍X�V����
void CDeliveryGameScene::Update()
{
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

	// ���ԕ\��UI�N���X�̍X�V
	mpTimeUI->Update();
	// �X�R�A�\��UI�N���X�̍X�V
	mpTrashScoreUI->Update();
	// �v���C���[�N���X���擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CTrashPlayer::Instance());
	// �������Ԃ�0�ɂȂ������A
	// �v���C���[�̎��S�ɂ���ăQ�[�����I�����鎞
	if (mpTimeUI->GetTime() < 0 ||
		player->GetGameEnd())
	{
		// �Q�[�����|�[�Y
		CTaskManager::Instance()->Pause(PAUSE_GAME);
		// �ҋ@���Ԃ��o�߂����烊�U���g�V�[����
		mElapsedTime += Times::DeltaTime();
		if (mElapsedTime >= IDLE_TIME)
		{
			// ���_�Ǘ��N���X���擾
			CScoreManager* scoreMgr = CScoreManager::Instance();
			// �V�[���Ǘ��N���X���擾
			CSceneManager* sceneMgr = CSceneManager::Instance();

			// �c��HP�̊��������߂�
			float hpPer = Math::Clamp01((float)player->GetHp() / player->GetMaxHp());
			// �X�R�A�f�[�^��ݒ�
			scoreMgr->SetTrashGameScoreData(mpTrashScoreUI->GetScore(),
				player->GetTrashBag(), player->GetGoldTrashBag(), hpPer);
			// �Q�[���̎�ނ����̃V�[���ɐݒ�
			scoreMgr->SetGameType((int)sceneMgr->GetCurrentScene());
			// ���U���g�V�[���ֈڍs
			sceneMgr->LoadScene(EScene::eResult);
		}
	}
}
