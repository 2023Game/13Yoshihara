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
#include "CTestField.h"
#include "CEnemy.h"
#include "CEnemyManager.h"

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
	// ������
	CResourceManager::Load<CModel>("FireBall",			"Wepon\\Spell_Attack\\FireBall.obj");
	CResourceManager::Load<CModel>("FireBolt",			"Wepon\\Spell_Attack\\FireBolt.obj");
	CResourceManager::Load<CModel>("FireBreath",		"Wepon\\Spell_Attack\\FireBreath.obj");
	CResourceManager::Load<CModel>("FireTeleport",		"Wepon\\Spell_Support\\FireTeleport.obj");
	CResourceManager::Load<CModel>("FireShield",		"Wepon\\Spell_Support\\FireShield.obj");
	CResourceManager::Load<CModel>("FireReflector",		"Wepon\\Spell_Support\\FireReflector.obj");
	// ������
	CResourceManager::Load<CModel>("WindBall",			"Wepon\\Spell_Attack\\WindBall.obj");
	CResourceManager::Load<CModel>("WindBolt",			"Wepon\\Spell_Attack\\WindBolt.obj");
	CResourceManager::Load<CModel>("WindBreath",		"Wepon\\Spell_Attack\\WindBreath.obj");
	CResourceManager::Load<CModel>("WindTeleport",		"Wepon\\Spell_Support\\WindTeleport.obj");
	CResourceManager::Load<CModel>("WindShield",		"Wepon\\Spell_Support\\WindShield.obj");
	CResourceManager::Load<CModel>("WindReflector",		"Wepon\\Spell_Support\\WindReflector.obj");
	// �n����
	CResourceManager::Load<CModel>("EarthBall",			"Wepon\\Spell_Attack\\EarthBall.obj");
	CResourceManager::Load<CModel>("EarthBolt",			"Wepon\\Spell_Attack\\EarthBolt.obj");
	CResourceManager::Load<CModel>("EarthBreath",		"Wepon\\Spell_Attack\\EarthBreath.obj");
	CResourceManager::Load<CModel>("EarthTeleport",		"Wepon\\Spell_Support\\EarthTeleport.obj");
	CResourceManager::Load<CModel>("EarthShield",		"Wepon\\Spell_Support\\EarthShield.obj");
	CResourceManager::Load<CModel>("EarthReflector",	"Wepon\\Spell_Support\\EarthReflector.obj");
	// ������
	CResourceManager::Load<CModel>("ThunderBall",		"Wepon\\Spell_Attack\\ThunderBall.obj");
	CResourceManager::Load<CModel>("ThunderBolt",		"Wepon\\Spell_Attack\\ThunderBolt.obj");
	CResourceManager::Load<CModel>("ThunderBreath",		"Wepon\\Spell_Attack\\ThunderBreath.obj");
	CResourceManager::Load<CModel>("ThunderTeleport",	"Wepon\\Spell_Support\\ThunderTeleport.obj");
	CResourceManager::Load<CModel>("ThunderShield",		"Wepon\\Spell_Support\\ThunderShield.obj");
	CResourceManager::Load<CModel>("ThunderReflector",	"Wepon\\Spell_Support\\ThunderReflector.obj");
	// ������
	CResourceManager::Load<CModel>("WaterBall",			"Wepon\\Spell_Attack\\WaterBall.obj");
	CResourceManager::Load<CModel>("WaterBolt",			"Wepon\\Spell_Attack\\WaterBolt.obj");
	CResourceManager::Load<CModel>("WaterBreath",		"Wepon\\Spell_Attack\\WaterBreath.obj");
	CResourceManager::Load<CModel>("WaterTeleport",		"Wepon\\Spell_Support\\WaterTeleport.obj");
	CResourceManager::Load<CModel>("WaterShield",		"Wepon\\Spell_Support\\WaterShield.obj");
	CResourceManager::Load<CModel>("WaterReflector",	"Wepon\\Spell_Support\\WaterReflector.obj");
	// ������
	CResourceManager::Load<CModel>("NeutralBall",		"Wepon\\Spell_Attack\\NeutralBall.obj");
	CResourceManager::Load<CModel>("NeutralBolt",		"Wepon\\Spell_Attack\\NeutralBolt.obj");
	CResourceManager::Load<CModel>("NeutralBreath",		"Wepon\\Spell_Attack\\NeutralBreath.obj");
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

	// �G�̊Ǘ��N���X�𐶐�
	mpEnemyMgr = new CEnemyManager();

	// CGameCamera2
	CVector atPos = player->Position() + CVector(0.0f, 20.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 20.0f, 0.0f),
		atPos
	);

	// �t�B�[���h�N���X����
	//CTestField* field = new CTestField();

	//// �Փ˔��肷��R���C�_�\��ǉ�
	//mainCamera->AddCollider(field->GetGroundCol());
	//mainCamera->AddCollider(field->GetWallCol());

	mainCamera->SetFollowTargetTf(player);

	// �Q�[�����j���[���쐬
	mpGameMenu = new CGameMenu();
}

//�V�[���̍X�V����
void CTestScene::Update()
{
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

	// �G�̊Ǘ��N���X�̍X�V
	mpEnemyMgr->Update();
}
