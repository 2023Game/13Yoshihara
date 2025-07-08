#include "CSelectScene.h"
#include "CSceneManager.h"
#include "CInput.h"
#include "CBGMManager.h"
#include "CSelectMenu.h"

CSelectScene::CSelectScene()
	: CSceneBase(EScene::eSelect)
{
}

// �f�X�g���N�^
CSelectScene::~CSelectScene()
{
}

// �V�[���ǂݍ���
void CSelectScene::Load()
{
	// �^�C�g����ʂ̓J�[�\���\��
	CInput::ShowCursor(true);
	// �w�i�F�ݒ�
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// �^�C�g��BGM���Đ�
	CBGMManager::Instance()->Play(EBGMType::eTitle);

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);

	mpSelectUI = new CSelectMenu();
	AddTask(mpSelectUI);
}

// �V�[���̍X�V����
void CSelectScene::Update()
{
	// �I����ʂ�
	if (mpSelectUI->IsEnd())
	{
		// �ΐ�J�n�Ȃ�΁A�ΐ�V�[����ǂݍ���
		if (mpSelectUI->IsBattleStart())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGameTest);
		}
		// �߂�Ȃ�΁A�^�C�g���V�[����ǂݍ���
		else if (mpSelectUI->IsReturn())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
		}
	}
}
