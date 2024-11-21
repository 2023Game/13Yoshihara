#include "CTrashGameScene.h"
#include "CSceneManager.h"
#include "CTrashField.h"
#include "CPlayer.h"
#include "CTrashPlayer.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CCarAndTruckManager.h"

//�R���X�g���N�^
CTrashGameScene::CTrashGameScene()
	: CSceneBase(EScene::eTrashGame)
	, mpGameMenu(nullptr)
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

	CResourceManager::Load<CModel>("Field", "Field\\Field.obj");
	CResourceManager::Load<CModel>("FieldCube", "Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>("FieldCylinder", "Field\\Object\\cylinder.obj");
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\player.x");
	CResourceManager::Load<CTexture>("Laser", "Effect\\laser.png");
	CResourceManager::Load<CTexture>("LightningBolt", "Effect\\lightning_bolt.png");
	CResourceManager::Load<CModel>("Slash", "Effect\\slash.obj");
	CResourceManager::Load<CSound>("SlashSound", "Sound\\SE\\slash.wav");
	
	// �S�~�E��
	CResourceManager::Load<CModelX>("TrashPlayer", "Character\\TrashBox\\TrashBox.x");
	CResourceManager::Load<CModel>("TrashStage", "Field\\TrashStage\\TrashStage.obj");
	CResourceManager::Load<CModel>("TrashBox", "Field\\Object\\TrashBox.obj");
	CResourceManager::Load<CModel>("Car", "Character\\Car\\Car.obj");
	CResourceManager::Load<CModel>("GarbageTruck", "Character\\GarbageTruck\\GarbageTruck.obj");

	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eHome);

	new CTrashField();

	CPlayer* player = new CPlayer();
	player->Scale(1.0f, 1.0f, 1.0f);

	CTrashPlayer* trashPlayer = new CTrashPlayer();
	trashPlayer->Scale(0.1f, 0.1f, 0.1f);

	// �Ԃƃg���b�N�̊Ǘ��N���X�쐬
	mpCCarAndTruckManager =
		new CCarAndTruckManager();

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
		atPos + CVector(0.0f, 0.0f, 15.0f),
		atPos
	);

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

	mpCCarAndTruckManager->Update();
}
