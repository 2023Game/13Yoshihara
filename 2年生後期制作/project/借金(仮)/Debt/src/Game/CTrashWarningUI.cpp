#include "CTrashWarningUI.h"
#include "CImage.h"
#include "CTextUI2D.h"
#include "CTrashPlayer.h"
#include "CTrashEnemyManager.h"
#include "CTrashVehicleManager.h"

// �x���摜�̃p�X
#define WARNING_PATH "UI/trash_game_warning.png"

// �R���X�g���N�^
CTrashWarningUI::CTrashWarningUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
{
	// �G���A�O�̌x���摜
	mpWarningImg = new CImage
	(
		WARNING_PATH,
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpWarningImg->SetAlpha(0.5f);
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
}

// �`��
void CTrashWarningUI::Render()
{
	// �v���C���[���擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CTrashPlayer::Instance());
	// �ԗ��Ǘ��N���X�擾
	CTrashVehicleManager* vehicleMgr = CTrashVehicleManager::Instance();
	// �v���C���[��X�̃G���A�O����
	// ���d�u���p�̃g���b�N���o�����Ă��Ȃ��ꍇ�`��
	if (player->AreaOutX() &&
		!vehicleMgr->GetPopPunisherTruck())
	{
		// TODO�F�x�����Đ�
		mpWarningImg->Render();
		return;
	}
	// �G�Ǘ��N���X�擾
	CTrashEnemyManager* enemyMgr = CTrashEnemyManager::Instance();
	// �v���C���[��Z�̃G���A�O����
	// ���d�u���p�̓G���o�����Ă��Ȃ��ꍇ�`��
	if (player->AreaOutZ() &&
		!enemyMgr->GetPopPunisherEnemy())
	{
		// TODO�F�x�����Đ�
		mpWarningImg->Render();
		return;
	}
}
