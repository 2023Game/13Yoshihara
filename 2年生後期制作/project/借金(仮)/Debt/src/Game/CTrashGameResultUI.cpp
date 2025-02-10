#include "CTrashGameResultUI.h"
#include "CText.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CScoreManager.h"
#include "CTrashBagStatus.h"
#include "CExpandButton.h"

// �w�i�摜�̃p�X
#define BACKGROUND_PATH "UI/trash_game_result_bg.png"
// �摜�̃p�X
#define BAG_PATH		"UI/trashBag.png"
#define GOLD_BAG_PATH	"UI/gold_trashBag.png"

// ���_�̃e�L�X�g�̃I�t�Z�b�g���W
#define SCORE_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.0f,   WINDOW_HEIGHT * 0.2f)
// �S�~�܂̐��̃e�L�X�g�̃I�t�Z�b�g���W
#define NUM_OFFSET_POS					CVector2(WINDOW_WIDTH * 0.3f,   WINDOW_HEIGHT * 0.4f)
// �S�~�܂̒l�i�̃e�L�X�g�̃I�t�Z�b�g���W
#define PRICE_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.475f, WINDOW_HEIGHT * 0.4f)
// �S�~�܂̒l�i�̃e�L�X�g�̃I�t�Z�b�g���W
#define TOTAL_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.7f,   WINDOW_HEIGHT * 0.4f)
// �P�ʂ̃e�L�X�g�̃I�t�Z�b�g���W
#define UNIT_OFFSET_POS					CVector2(WINDOW_WIDTH * 0.35f,  WINDOW_HEIGHT * 0.4f)
// �L���̃e�L�X�g�̃I�t�Z�b�g���W
#define SYMBOL_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.42f,  WINDOW_HEIGHT * 0.4f)
// ��ԉ��̌v�Z�̐����e�L�X�g�̃I�t�Z�b�g���W
#define DESCRIPTION_OFFSET_POS			CVector2(WINDOW_WIDTH * 0.15f, WINDOW_HEIGHT * 0.76f)
// �{�[�i�X�̐����e�L�X�g�̃I�t�Z�b�g���W
#define BONUS_DESCRIPTION_OFFSET_POS	CVector2(WINDOW_WIDTH * 0.475f, WINDOW_HEIGHT * 0.83f)

// �S�~�܂̉摜�̃T�C�Y�̔{��
#define SIZE_RATE 0.25f
// �S�~�܂̉摜�̃I�t�Z�b�g���W
#define TRASH_BAG_OFFEST_POS		CVector2(WINDOW_WIDTH * 0.225f, WINDOW_HEIGHT * 0.38f)
// �S�[���h�̃S�~�܂̉摜�̃I�t�Z�b�g���W
#define GOLD_TRASH_BAG_OFFEST_POS	CVector2(WINDOW_WIDTH * 0.225f, WINDOW_HEIGHT * 0.56f)

// �{�^���̃I�t�Z�b�g���W
#define BUTTON_OFFSET_POS CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.25f)

CTrashGameResultUI::CTrashGameResultUI()
	: CResultUIBase(BACKGROUND_PATH)
{
	// �{�^���̍��W��ݒ�
	mpReturnButton->SetPos(BUTTON_OFFSET_POS);
	// �S�~�܂̓��_�擾�p
	CTrashBagStatus* trashBagStatus = new CTrashBagStatus(false);
	// �S�~�܂̓��_���擾
	int trashBagPoint = trashBagStatus->GetPoint(false);
	int goldTrashBagPoint = trashBagStatus->GetPoint(true);
	// �s�v�Ȃ̂ō폜
	delete trashBagStatus;

	// ���_�Ǘ��N���X���擾
	CScoreManager* scoreMgr = CScoreManager::Instance();
	// �S�~�E���̃X�R�A�f�[�^���擾
	CScoreManager::TrashGameScoreData* scoreData =
		scoreMgr->GetTrashGameScoreData();

	// ���ꂼ��̃S�~�܂̃X�R�A�̍��v���v�Z
	int trashBagTotalScore = 
		scoreData->mTrashBagNum * trashBagPoint;
	int goldBagTotalScore =
		scoreData->mGoldTrashBagNum * goldTrashBagPoint;
	// �S�ẴX�R�A�̍��v
	int totalScore = trashBagTotalScore + goldBagTotalScore;
	// �{�[�i�X�̔{��
	float bonus = scoreData->mHpPer;
	// �{�[�i�X�̃X�R�A���v�Z
	int bonusScore = totalScore * bonus;
	// ���v�X�R�A�ɉ��Z����
	totalScore += bonusScore;


	// ���_�̃e�L�X�g�𐶐�
	mpScorePointText = new CTextUI2D(ETaskPauseType::eGame, false);
	// �e�L�X�g��ݒ�
	mpScorePointText->ChangeToStr("%d�~\n", totalScore);
	// ���W��ݒ�
	mpScorePointText->Position(SCORE_OFFSET_POS);
	// �t�H���g�T�C�Y��ݒ�
	mpScorePointText->SetFontSize(128);
	// �����̑����̊��ݒ�
	mpScorePointText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);

	// ���l�̃e�L�X�g1�𐶐�
	mpNumText1 = new CTextUI2D(ETaskPauseType::eGame, false);
	// �e�L�X�g��ݒ�
	mpNumText1->ChangeToStr("%d\n�@\n�@\n%d\n�@\n�@\n%d\n",
		scoreData->mTrashBagNum, 
		scoreData->mGoldTrashBagNum, 
		trashBagTotalScore + goldBagTotalScore);
	// ���W��ݒ�
	mpNumText1->Position(NUM_OFFSET_POS);

	// ���l�̃e�L�X�g2�𐶐�
	mpNumText2 = new CTextUI2D(ETaskPauseType::eGame, false);
	// �e�L�X�g��ݒ�
	mpNumText2->ChangeToStr("%d\n�@\n�@\n%d\n�@\n�@\n%.1f\n",
		trashBagPoint,
		goldTrashBagPoint,
		bonus);
	// ���W��ݒ�
	mpNumText2->Position(PRICE_OFFSET_POS);

	// �v�Z���ʂ̃e�L�X�g�𐶐�
	mpTotalText = new CTextUI2D(ETaskPauseType::eGame, false);
	// �e�L�X�g��ݒ�
	mpTotalText->ChangeToStr("%d\n�@\n�@\n%d\n�@\n�@\n%d\n",
		trashBagTotalScore,
		goldBagTotalScore,
		bonusScore);
	// ���W��ݒ�
	mpTotalText->Position(TOTAL_OFFSET_POS);

	// �P�ʂ̃e�L�X�g�𐶐�
	mpUnitText = new CTextUI2D(ETaskPauseType::eGame, false);
	// �e�L�X�g��ݒ�
	mpUnitText->SetStr("�R�@�@�@�@�@�@�~\n�@\n�@\n�R�@�@�@�@�@�@�~\n");
	// ���W��ݒ�
	mpUnitText->Position(UNIT_OFFSET_POS);

	// �~�̃e�L�X�g�𐶐�
	mpSymbolText= new CTextUI2D(ETaskPauseType::eGame, false);
	// �e�L�X�g��ݒ�
	mpSymbolText->SetStr("�~�@�@�@�@�@�@��\n�@\n�@\n�~�@�@�@�@�@�@��\n�@\n�@\n�~�@�@�@�@�@�@��\n");
	mpSymbolText->SetFontAligment(FTGL::TextAlignment::ALIGN_LEFT);
	// ���W��ݒ�
	mpSymbolText->Position(SYMBOL_OFFSET_POS);

	// ��ԉ��̌v�Z�̐����e�L�X�g
	mpDescriptionText = new CTextUI2D(ETaskPauseType::eGame, false);
	// �e�L�X�g��ݒ�
	mpDescriptionText->SetStr("�{�[�i�X�I�I");
	// ���W��ݒ�
	mpDescriptionText->Position(DESCRIPTION_OFFSET_POS);
	// �����̐F��ݒ�
	mpDescriptionText->SetFontColor(CColor(0.9f, 0.0f, 0.0f));

	// �{�[�i�X�̐����e�L�X�g
	mpBonusDescriptionText = new CTextUI2D(ETaskPauseType::eGame, false, nullptr);
	// �e�L�X�g��ݒ�
	mpBonusDescriptionText->SetStr("�cHP����");
	// �t�H���g�T�C�Y��ݒ�
	mpBonusDescriptionText->SetFontSize(32);
	// ���W��ݒ�
	mpBonusDescriptionText->Position(BONUS_DESCRIPTION_OFFSET_POS);

	// �S�~�܂̉摜�𐶐�
	mpTrashBagImg = new CImage
	(
		BAG_PATH,
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

	// �S�[���h�S�~�܂̉摜�𐶐�
	mpGoldTrashBagImg = new CImage
	(
		GOLD_BAG_PATH,
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
	SAFE_DELETE(mpScorePointText);
	SAFE_DELETE(mpNumText1);
	SAFE_DELETE(mpNumText2);
	SAFE_DELETE(mpUnitText);
	SAFE_DELETE(mpSymbolText);
	SAFE_DELETE(mpDescriptionText);
	SAFE_DELETE(mpBonusDescriptionText);

	SAFE_DELETE(mpTrashBagImg);
	SAFE_DELETE(mpGoldTrashBagImg);
}

// �X�V
void CTrashGameResultUI::Update()
{
	CResultUIBase::Update();
	// �e�L�X�g
	mpScorePointText->Update();
	mpNumText1->Update();
	mpNumText2->Update();
	mpTotalText->Update();
	mpUnitText->Update();
	mpSymbolText->Update();
	mpDescriptionText->Update();
	mpBonusDescriptionText->Update();
	// �摜
	mpTrashBagImg->Update();
	mpGoldTrashBagImg->Update();
}

// �`��
void CTrashGameResultUI::Render()
{
	CResultUIBase::Render();
	// �e�L�X�g
	mpScorePointText->Render();
	mpNumText1->Render();
	mpNumText2->Render();
	mpTotalText->Render();
	mpUnitText->Render();
	mpSymbolText->Render();
	mpDescriptionText->Render();
	mpBonusDescriptionText->Render();
	// �摜
	mpTrashBagImg->Render();
	mpGoldTrashBagImg->Render();
}
