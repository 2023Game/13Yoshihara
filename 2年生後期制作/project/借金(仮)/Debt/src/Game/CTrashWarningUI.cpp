#include "CTrashWarningUI.h"
#include "CImage.h"
#include "CTextUI2D.h"
#include "CTrashPlayer.h"
#include "CTrashEnemyManager.h"
#include "CTrashVehicleManager.h"
#include "CSound.h"

// �A���t�@�̕ω��Ԋu
#define CHANGE_ALPHA_INTERVAL 0.01f
// �ϓ�����A���t�@�l
#define CHANGE_ALPHA 0.05f
// �A���t�@�̍Œ�l
#define MIN_ALPHA 0.0f
// �A���t�@�̍ō��l
#define MAX_ALPHA 0.50f
// �ҋ@����
#define WAIT_TIME 0.2f

#define SE_VOLUME 0.5f
// �Đ��Ԋu
#define SE_INTERVAL 3.0f

// �x���摜�̃p�X
#define WARNING_PATH "UI/trash_game_warning.png"

// �R���X�g���N�^
CTrashWarningUI::CTrashWarningUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIntervalTime(0.0f)
{
	// �x������ݒ�
	mpAlarmSE = CResourceManager::Get<CSound>("AlarmSE");

	// �G���A�O�̌x���摜
	mpWarningImg = new CImage
	(
		WARNING_PATH,
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpWarningImg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

// �f�X�g���N�^
CTrashWarningUI::~CTrashWarningUI()
{
	SAFE_DELETE(mpWarningImg);
}

// �X�V
void CTrashWarningUI::Update()
{
	mElapsedTime += Times::DeltaTime();
	switch (mStateStep)
	{
		// �ҋ@
	case 0:
		// �ҋ@���Ԃ��o�߂�����
		if (mElapsedTime >= WAIT_TIME)
		{
			mElapsedTime = 0.0f;
			// ����
			mStateStep++;
		}
	
		break;

		// �A���t�@�̌���
	case 1:
		// �ω��̊Ԋu�̎��Ԃ��߂�����
		if (mElapsedTime >= CHANGE_ALPHA_INTERVAL)
		{
			mElapsedTime = 0.0f;
			// �A���t�@������
			mpWarningImg->SetAlpha(mpWarningImg->GetAlpha() - CHANGE_ALPHA);
			// �A���t�@���Œ�l�ȉ��Ȃ�
			if (mpWarningImg->GetAlpha() <= MIN_ALPHA)
			{
				// ����
				mStateStep++;
			}
		}
		break;

		// �A���t�@�̑���
	case 2:
		// �ω��̊Ԋu�̎��Ԃ��߂�����
		if (mElapsedTime >= CHANGE_ALPHA_INTERVAL)
		{
			mElapsedTime = 0.0f;
			// �A���t�@�𑝉�
			mpWarningImg->SetAlpha(mpWarningImg->GetAlpha() + CHANGE_ALPHA);
			// �A���t�@���ō��l�ȏ�Ȃ�
			if (mpWarningImg->GetAlpha() >= MAX_ALPHA)
			{
				// 0��
				mStateStep = 0;
			}
		}
		break;
	}
}

// �`��
void CTrashWarningUI::Render()
{
	mIntervalTime += Times::DeltaTime();
	// �v���C���[���擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CTrashPlayer::Instance());

	// �G�Ǘ��N���X�擾
	CTrashEnemyManager* enemyMgr = CTrashEnemyManager::Instance();
	// �ԗ��Ǘ��N���X�擾
	CTrashVehicleManager* vehicleMgr = CTrashVehicleManager::Instance();
	// �v���C���[��Z�̃G���A�O����
	// ���d�u���p�̓G���o�����Ă��Ȃ��ꍇ�`��
	if (player->AreaOutZ() &&
		!enemyMgr->GetPopPunisherEnemy())
	{
		// �x�����Đ�
			// �Đ��Ԋu���o�߂�����
		if (mIntervalTime >= SE_INTERVAL)
		{
			mIntervalTime = 0.0f;
			mpAlarmSE->Play(SE_VOLUME, true);
		}
		mpWarningImg->Render();
	}	
	// �v���C���[��X�̃G���A�O����
	// ���d�u���p�̃g���b�N���o�����Ă��Ȃ��ꍇ�`��
	else if (player->AreaOutX() &&
		!vehicleMgr->GetPopPunisherTruck())
	{
		// �x�����Đ�
		// �Đ��Ԋu���o�߂�����
		if (mIntervalTime >= SE_INTERVAL)
		{
			mIntervalTime = 0.0f;
			mpAlarmSE->Play(SE_VOLUME, true);
		}
		mpWarningImg->Render();
	}
	// �x�����o�Ă��Ȃ��Ȃ�
	else
	{
		// ���͈͊O�ɏo���Ƃ������Đ��ł���悤�ɐݒ�
		mIntervalTime = SE_INTERVAL;
		// �x�������~
		mpAlarmSE->Stop();
	}
}
