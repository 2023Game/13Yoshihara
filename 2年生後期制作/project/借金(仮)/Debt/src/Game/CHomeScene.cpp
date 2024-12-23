#include "CHomeScene.h"
#include "CSceneManager.h"
#include "CHomeField.h"
#include "CHomePlayer.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"

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
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\player.x");

	// ���_
	CResourceManager::Load<CModel>("HomeBase", "Field\\HomeBase\\HomeBase.obj");
	CResourceManager::Load<CModel>("Bed", "Field\\Object\\Bed.obj");
	CResourceManager::Load<CModel>("Chair", "Field\\Object\\Chair.obj");
	CResourceManager::Load<CModel>("Computer", "Field\\Object\\Computer.obj");
	CResourceManager::Load<CModel>("Desk", "Field\\Object\\Desk.obj");
	CResourceManager::Load<CModel>("Door", "Field\\Object\\Door.obj");
	CResourceManager::Load<CModel>("Rack", "Field\\Object\\Rack.obj");
	// �����蔻��p�̃R���W�������f��
	CResourceManager::Load<CModel>("HomeBase_Ground&Ceiling_Collision", "Field\\HomeBase\\CollisionModel\\HomeBase_Ground&Ceiling_Collision.obj");
	CResourceManager::Load<CModel>("HomeBase_Wall_Collision", "Field\\HomeBase\\CollisionModel\\HomeBase_Wall_Collision.obj");
	CResourceManager::Load<CModel>("HomeBase_Object_Collision", "Field\\HomeBase\\CollisionModel\\HomeBase_Object_Collision.obj");


	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eHome);

	new CHomeField();

	CHomePlayer* player = new CHomePlayer();
	player->Scale(1.0f, 1.0f, 1.0f);

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
void CHomeScene::Update()
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
}
