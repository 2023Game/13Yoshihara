#include "CGameClearScene.h"
#include "CBGMManager.h"
#include "CGameClearUI.h"
#include "CSceneManager.h"

// �R���X�g���N�^
CGameClearScene::CGameClearScene()
	: CSceneBase(EScene::eClear)
{
}

// �f�X�g���N�^
CGameClearScene::~CGameClearScene()
{
}

// �V�[���ǂݍ���
void CGameClearScene::Load()
{
	// �^�C�g����ʂ̓J�[�\���\��
	CInput::ShowCursor(true);
	// �w�i�F�ݒ�
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// �Q�[���N���ABGM���Đ�
	//CBGMManager::Instance()->Play(EBGMType::eClear);

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);

	mpGameClearUI = new CGameClearUI();
	AddTask(mpGameClearUI);
}

// �V�[���̍X�V����
void CGameClearScene::Update()
{
	// �Q�[���N���A��ʂ�
	if (mpGameClearUI->IsEnd())
	{
		// �^�C�g���֖߂�Ȃ�΁A�^�C�g����
		if (mpGameClearUI->IsReturnTitle())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
		}
		// �Q�[���I���Ȃ�΁A�A�v�������
		else if (mpGameClearUI->IsExitGame())
		{
			System::ExitGame();
		}
	}
}
