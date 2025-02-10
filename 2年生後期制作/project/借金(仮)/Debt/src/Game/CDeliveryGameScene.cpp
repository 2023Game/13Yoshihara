#include "CDeliveryGameScene.h"
#include "CCamera.h"
#include "CGameMenu.h"
#include "CTaskManager.h"
#include "CSceneManager.h"
#include "CScoreManager.h"
#include "CResourceManager.h"
#include "CBGMManager.h"
#include "CNavManager.h"
#include "CTimeUI.h"
#include "CDeliveryScoreUI.h"
#include "CInput.h"
#include "Maths.h"
#include "CDeliveryField.h"
#include "CDeliveryFieldManager.h"
#include "CTrashPlayer.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemy.h"

// ��������
#define MAX_TIME 100
// ���U���g�V�[���֍s���O�̑ҋ@����
#define IDLE_TIME 1.0f

// �v���C���[�̏������W
#define PLAYER_POS CVector(0.0f,10.0f,50.0f)

//�R���X�g���N�^
CDeliveryGameScene::CDeliveryGameScene()
	: CSceneBase(EScene::eDeliveryGame)
	, mpGameMenu(nullptr)
	, mpTimeUI(nullptr)
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
	CResourceManager::Load<CModelX>(	"TrashPlayer",		"Character\\TrashBox\\TrashBoxPlayer.x");
	// CModel
	CResourceManager::Load<CModel>(		"DeliveryStage",	"Field\\DeliveryStage\\DeliveryStage.obj");
	CResourceManager::Load<CModel>(		"Sky",				"Field\\Sky\\Sky.obj");
	CResourceManager::Load<CModel>(		"DeliveryPlayer",	"Character\\DeliveryTruck\\DeliveryTruck_Player.obj");
	CResourceManager::Load<CModel>(		"DeliveryEnemy",	"Character\\DeliveryTruck\\DeliveryTruck_Enemy.obj");
	CResourceManager::Load<CModel>(		"DeliveryItem",		"Field\\Object\\DeliveryItem.obj");
	CResourceManager::Load<CModel>(		"DeliveryHome",		"Field\\Object\\DeliveryHome\\DeliveryHome.obj");
	// �����蔻��p�̃R���W�������f��
	CResourceManager::Load<CModel>(		"DeliveryStage_Ground_Collision",	"Field\\DeliveryStage\\CollisionModel\\DeliveryStage_Ground_Collision.obj");
	CResourceManager::Load<CModel>(		"DeliveryHome_Wall_Collision",		"Field\\Object\\DeliveryHome\\CollisionModel\\DeliveryHome_Wall_Collision.obj");
	CResourceManager::Load<CModel>(		"DeliveryHome_Goal_Collision",		"Field\\Object\\DeliveryHome\\CollisionModel\\DeliveryHome_Goal_Collision.obj");
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

	// �z�B�̃t�B�[���h�Ǘ��N���X���쐬
	mpFieldMgr = new CDeliveryFieldManager();

	CDeliveryPlayer* player = new CDeliveryPlayer();
	player->Position(-30.0f, 0.0f, 0.0f);
	player->SetRoadType(ERoadType::eLeft2);

	CDeliveryEnemy* enemy = new CDeliveryEnemy();
	enemy->Position(30.0f, 0.0f, 0.0f);

	// ���ԕ\��UI����
	mpTimeUI = new CTimeUI(MAX_TIME);
	// �X�R�A�\��UI����
	mpDeliveryScoreUI = new CDeliveryScoreUI();

	// Camera�̃e�X�g
	CVector atPos = CVector(0.0f, 8.0f, 0.0f);
	CCamera* mainCamera = new CCamera
	(
		atPos + CVector(0.0f, 400.0f, 200.0f),
		atPos
	);
	 
	// CGameCamera2�̃e�X�g
	//CVector atPos = player->Position() + CVector(0.0f, 8.0f, 0.0f);
	//CGameCamera2* mainCamera = new CGameCamera2
	//(
	//	atPos + CVector(0.0f, 10.0f, 400.0f),
	//	atPos
	//);
	//bool end = false;
	//// �S�Ẵt�B�[���h�̃R���C�_�[��ǉ�����܂Ń��[�v
	//for (int i = 0; i >= 0; i++)
	//{
	//	CDeliveryField* field = mpFieldMgr->GetField(i, end);
	//	// �Փ˔��肷��R���C�_�\��ǉ�
	//	mainCamera->AddCollider(field->GetGroundCol());
	//	mainCamera->AddCollider(field->GetWallCol());
	//	mainCamera->AddCollider(field->GetObjCol());

	//	// �Ō�̗v�f�̃R���C�_�\�܂Œǉ������烋�[�v�I��
	//	if (end) break;
	//}

	//mainCamera->SetFollowTargetTf(player);

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

	// �t�B�[���h�Ǘ��N���X�̍X�V
	mpFieldMgr->Update();
	// ���ԕ\��UI�N���X�̍X�V
	mpTimeUI->Update();
	// �X�R�A�\��UI�N���X�̍X�V
	mpDeliveryScoreUI->Update();

	// �v���C���[�N���X���擾
	CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(CDeliveryPlayer::Instance());
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
			
			int accuracyInt = 0;
			float accuracy = 0.0f;
			// 1�����ł��Ă��Ȃ��ꍇ�͏������Ȃ�
			if (!player->GetShotNum() == 0)
			{
				// �����������߂�
				// �����v�Z��ɏ����_�\�L����%�\�L�ɕϊ����ď����_�ȉ�2�������؂�̂Ă�
				accuracyInt = Math::Clamp01((float)player->GetHitNum() / player->GetShotNum()) * 100;
				// �����_�\�L�ɖ߂�
				accuracy = accuracyInt / 100.0f;
			}
			
			// �X�R�A�f�[�^��ݒ�
			scoreMgr->SetDeliveryGameScoreData(mpDeliveryScoreUI->GetScore(),
				player->GetDeliveryNum(), player->GetDestroyEnemyNum(),
				accuracy);
			// �Q�[���̎�ނ����̃V�[���ɐݒ�
			scoreMgr->SetGameType((int)sceneMgr->GetCurrentScene());
			// ���U���g�V�[���ֈڍs
			sceneMgr->LoadScene(EScene::eResult);
		}
	}
}
