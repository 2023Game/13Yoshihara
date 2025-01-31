#include "CTrashGameResultUI.h"
#include "CText.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CScoreManager.h"
#include "CTrashBagStatus.h"

// �w�i�摜�̃p�X
#define BACKGROUND_PATH "UI/trash_game_result_bg.png"

// ���_�̃e�L�X�g�̃I�t�Z�b�g���W
#define SCORE_OFFSET_POS	CVector(WINDOW_WIDTH * 0.48f, WINDOW_HEIGHT * 0.2f, 0.0f)
// �S�~�܂̐��̃e�L�X�g�̃I�t�Z�b�g���W
#define NUM_OFFSET_POS		CVector(WINDOW_WIDTH * 0.4f, WINDOW_HEIGHT * 0.4f, 0.0f)
// �S�~�܂̒l�i�̃e�L�X�g�̃I�t�Z�b�g���W
#define PRICE_OFFSET_POS	CVector(WINDOW_WIDTH * 0.55f, WINDOW_HEIGHT * 0.4f, 0.0f)
// �P�ʂ̃e�L�X�g�̃I�t�Z�b�g���W
#define UNIT_OFFSET_POS		CVector(WINDOW_WIDTH * 0.45f, WINDOW_HEIGHT * 0.4f, 0.0f)
// �~�̃e�L�X�g�̃I�t�Z�b�g���W
#define X_OFFSET_POS		CVector(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.4f, 0.0f)

// �S�~�܂̉摜�̃T�C�Y�̔{��
#define SIZE_RATE 0.25f
// �S�~�܂̉摜�̃I�t�Z�b�g���W
#define TRASH_BAG_OFFEST_POS		CVector2(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.38f)
// �S�[���h�̃S�~�܂̉摜�̃I�t�Z�b�g���W
#define GOLD_TRASH_BAG_OFFEST_POS	CVector2(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.56f)

CTrashGameResultUI::CTrashGameResultUI()
	: CResultUIBase(BACKGROUND_PATH)
{
	// �S�~�܂̓��_�擾�p
	CTrashBagStatus* trashBagStatus = new CTrashBagStatus(false);
	// �S�~�܂̓��_���L��
	int trashBagPoint = trashBagStatus->GetPoint(false);
	int goldTrashBagPoint = trashBagStatus->GetPoint(true);
	// �s�v�Ȃ̂ō폜
	delete trashBagStatus;

	// ���_�Ǘ��N���X���擾
	CScoreManager* scoreMgr = CScoreManager::Instance();
	// �S�~�E���̃X�R�A�f�[�^���擾
	CScoreManager::TrashGameScoreData* scoreData =
		scoreMgr->GetTrashGameScoreData();

	// ���_�̃e�L�X�g�𐶐�
	mpScorePointText = new CTextUI2D();
	// �e�L�X�g��ݒ�
	mpScorePointText->ChangeToStr("%d\n", scoreData->mScore);
	// ���W��ݒ�
	mpScorePointText->Position(SCORE_OFFSET_POS);
	// �t�H���g�T�C�Y��ݒ�
	mpScorePointText->SetFontSize(128);

	// �S�~�܂̐��̃e�L�X�g�𐶐�
	mpNumText = new CTextUI2D();
	// �e�L�X�g��ݒ�
	mpNumText->ChangeToStr("%d\n�@\n�@\n%d\n", scoreData->mTrashBagNum, scoreData->mGoldTrashBagNum);
	// ���W��ݒ�
	mpNumText->Position(NUM_OFFSET_POS);

	// �S�~�܂̒l�i�̃e�L�X�g�𐶐�
	mpPriceText = new CTextUI2D();
	// �e�L�X�g��ݒ�
	mpPriceText->ChangeToStr("%d\n�@\n�@\n%d\n", trashBagPoint, goldTrashBagPoint);
	// ���W��ݒ�
	mpPriceText->Position(PRICE_OFFSET_POS);

	// �P�ʂ̃e�L�X�g�𐶐�
	mpUnitText = new CTextUI2D();
	// �e�L�X�g��ݒ�
	mpUnitText->SetStr("�R�@�@�@�@�@�~\n�@\n�@\n�R�@�@�@�@�@�~\n");
	// ���W��ݒ�
	mpUnitText->Position(UNIT_OFFSET_POS);

	// �~�̃e�L�X�g�𐶐�
	mpXText= new CTextUI2D();
	// �e�L�X�g��ݒ�
	mpXText->SetStr("�~\n�@\n�@\n�~\n");
	// ���W��ݒ�
	mpXText->Position(X_OFFSET_POS);

	mpTrashBagImg = new CImage
	(
		"UI/trashBag.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	// �T�C�Y�̎擾
	CVector2 trashBagSize = mpTrashBagImg->GetSize();
	// �T�C�Y�̔{�����|�����l��ݒ�
	mpTrashBagImg->SetSize(trashBagSize * SIZE_RATE);
	// ���W��ݒ�
	mpTrashBagImg->SetPos(TRASH_BAG_OFFEST_POS);

	mpGoldTrashBagImg = new CImage
	(
		"UI/gold_trashBag.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	// �T�C�Y�̎擾
	trashBagSize = mpGoldTrashBagImg->GetSize();
	// �T�C�Y�̔{�����|�����l��ݒ�
	mpGoldTrashBagImg->SetSize(trashBagSize * SIZE_RATE);
	// ���W��ݒ�
	mpGoldTrashBagImg->SetPos(GOLD_TRASH_BAG_OFFEST_POS);
}

CTrashGameResultUI::~CTrashGameResultUI()
{
	SAFE_DELETE(mpTrashBagImg);
	SAFE_DELETE(mpGoldTrashBagImg);
}

// �X�V
void CTrashGameResultUI::Update()
{
	CResultUIBase::Update();
	mpTrashBagImg->Update();
	mpGoldTrashBagImg->Update();
}

// �`��
void CTrashGameResultUI::Render()
{
	CResultUIBase::Render();
	mpTrashBagImg->Render();
	mpGoldTrashBagImg->Render();
}
