#include "CTrashWarningUI.h"
#include "CImage.h"
#include "CTextUI2D.h"
#include "CTrashPlayer.h"
#include "CTrashEnemyManager.h"
#include "CVehicleManager.h"

// �x���摜�̃p�X
#define WARNING_PATH "UI/trash_game_result_bg.png"
// �x���e�L�X�g�̍��W
#define TEXT_POS CVector(0.0f, WINDOW_HEIGHT * 0.4f, 0.0f)

// �R���X�g���N�^
CTrashWarningUI::CTrashWarningUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
{
	// �G���A�O�̌x���摜
	mpWarningImg = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpWarningImg->SetColor(CColor::black);
	mpWarningImg->SetAlpha(0.5f);
	mpWarningImg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// �G���A�O�̌x���e�L�X�g
	mpWarningText = new CTextUI2D(ETaskPauseType::eGame, false);
	mpWarningText->SetStr("Warning");
	mpWarningText->SetFontSize(128);
	mpWarningText->SetFontColor(CColor::red);
	mpWarningText->SetOutLineColor(CColor::black);
	mpWarningText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	mpWarningText->Position(TEXT_POS);
}

// �f�X�g���N�^
CTrashWarningUI::~CTrashWarningUI()
{
	SAFE_DELETE(mpWarningImg);
}

// �X�V
void CTrashWarningUI::Update()
{
	mpWarningText->Update();
}

// �`��
void CTrashWarningUI::Render()
{
	// �v���C���[���擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CTrashPlayer::Instance());
	// �ԗ��Ǘ��N���X�擾
	CVehicleManager* vehicleMgr = CVehicleManager::Instance();
	// �v���C���[��X�̃G���A�O����
	// ���d�u���p�̃g���b�N���o�����Ă��Ȃ��ꍇ�`��
	if (player->AreaOutX() &&
		!vehicleMgr->GetPopPunisherTruck())
	{
		// TODO�F�x�����Đ�
		mpWarningImg->Render();
		mpWarningText->Render();
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
		mpWarningText->Render();
		return;
	}
}
