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
#include "CWand.h"
#include "CTestField.h"
#include "CEnemy.h"

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
	CResourceManager::Load<CModelX>("Player", "Character\\Adventurer\\Adventurer.x", true);
	// CModel
	CResourceManager::Load<CModel>("TestField",			"Field\\field.obj");
	CResourceManager::Load<CModel>("TestModel",			"Character\\TestModel.obj");
	CResourceManager::Load<CModel>("FireBall",			"Wepon\\Spell_Attack\\FireBall.obj");
	CResourceManager::Load<CModel>("FireBolt",			"Wepon\\Spell_Attack\\FireBolt.obj");
	CResourceManager::Load<CModel>("FireBreath",		"Wepon\\Spell_Attack\\FireBreath.obj");
	CResourceManager::Load<CModel>("NeutralTeleport",	"Wepon\\Spell_Support\\NeutralTeleport.obj");
	CResourceManager::Load<CModel>("NeutralShield",		"Wepon\\Spell_Support\\NeutralShield.obj");
	CResourceManager::Load<CModel>("NeutralReflector",	"Wepon\\Spell_Support\\NeutralReflector.obj");
	// �����蔻��p�̃R���W�������f��
	/*
	���ʉ�
	*/

	// �Q�[��BGM��ǂݍ���
	//CBGMManager::Instance()->Play(EBGMType::eGame);

	// �v���C���[����
	CPlayer* player = new CPlayer();
	player->Position(0.0f, 50.0f, 0.0f);

	CEnemy* enemy = new CEnemy();
	enemy->Position(0.0f, 50.0f, -50.0f);

	// ���ꂼ��̑����ݒ�
	enemy->SetOpponent(player);
	player->SetOpponent(enemy);

	// CGameCamera2
	CVector atPos = player->Position() + CVector(0.0f, 20.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 20.0f, 0.0f),
		atPos
	);

	// �t�B�[���h�N���X����
	CTestField* field = new CTestField();

	//// �Փ˔��肷��R���C�_�\��ǉ�
	mainCamera->AddCollider(field->GetGroundCol());
	mainCamera->AddCollider(field->GetWallCol());

	mainCamera->SetFollowTargetTf(player);
	mainCamera->SetLockOnTarget(enemy);

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
