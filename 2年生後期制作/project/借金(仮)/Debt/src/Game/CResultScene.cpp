#include "CResultScene.h"
#include "CSceneManager.h"
#include "CHomeScene.h"
#include "CCamera.h"
#include "CSound.h"
#include "CBGMManager.h"
#include "CResultUIBase.h"
#include "CTrashGameResultUI.h"
#include "CDeliveryGameResultUI.h"
#include "CScoreManager.h"
#include "SceneType.h"
#include "CTaskManager.h"
#if _DEBUG
#include "CInput.h"
#endif

// �R���X�g���N�^
CResultScene::CResultScene()
	: CSceneBase(EScene::eResult)
{
}

// �f�X�g���N�^
CResultScene::~CResultScene()
{
}

// �V�[���ǂݍ���
void CResultScene::Load()
{
	// �|�[�Y������
	CTaskManager::Instance()->UnPause(PAUSE_GAME);
	// �^�C�g����ʂ̓J�[�\���\��
	CInput::ShowCursor(true);
	// �w�i�F�ݒ�
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//�����Ń��U���g��ʂɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��

	// TODO�F���U���gBGM���Đ�
	CBGMManager::Instance()->Play(EBGMType::eTitle);

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);

	CScoreManager* scoreMgr = CScoreManager::Instance();
	// �Q�[���̎�ނ��S�~�E���Ȃ�
	if (scoreMgr->GetGameType() == (int)EScene::eTrashGame)
	{
		// �S�~�E���̃��U���gUI�𐶐�
		mpResultUI = new CTrashGameResultUI();
	}
	if (scoreMgr->GetGameType() == (int)EScene::eDeliveryGame)
	{
		// �z�B�̃��U���gUI�𐶐�
		mpResultUI = new CDeliveryGameResultUI();
	}
	AddTask(mpResultUI);
}

// �V�[���̍X�V����
void CResultScene::Update()
{
	// �^�C�g����ʂ�
	if (mpResultUI->IsEnd())
	{
		// ���_�֋A��Ȃ�΁A���_�V�[����ǂݍ���
		if (mpResultUI->IsReturnHome())
		{
			CSceneManager::Instance()->LoadScene(EScene::eHome);
		}
	}
#if _DEBUG
	if (CInput::PushKey('1'))
	{
		// �S�~�E��
		CScoreManager::Instance()->SetGameType((int)EScene::eTrashGame);
		// �ǂݍ��݂Ȃ���
		CSceneManager::Instance()->LoadScene(EScene::eResult);
	}
	else if (CInput::PushKey('2'))
	{
		// �z�B
		CScoreManager::Instance()->SetGameType((int)EScene::eDeliveryGame);
		// �ǂݍ��݂Ȃ���
		CSceneManager::Instance()->LoadScene(EScene::eResult);
	}
#endif
}
