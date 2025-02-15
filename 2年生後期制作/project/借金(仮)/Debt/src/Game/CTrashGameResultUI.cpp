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

// �S�~�܂̉摜�̃T�C�Y�̔{��
#define SIZE_RATE 0.25f
// �S�~�܂̉摜�̃I�t�Z�b�g���W
#define TRASH_BAG_OFFEST_POS		CVector2(WINDOW_WIDTH * 0.225f, WINDOW_HEIGHT * 0.38f)
// �S�[���h�̃S�~�܂̉摜�̃I�t�Z�b�g���W
#define GOLD_TRASH_BAG_OFFEST_POS	CVector2(WINDOW_WIDTH * 0.225f, WINDOW_HEIGHT * 0.56f)


CTrashGameResultUI::CTrashGameResultUI()
	: CResultUIBase(BACKGROUND_PATH)
{
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


	// ���_�̃e�L�X�g��ݒ�
	mpScorePointText->ChangeToStr("%d�~\n", totalScore);

	// ���l�̃e�L�X�g1��ݒ�
	mpNumText1->ChangeToStr("%d\n�@\n�@\n%d\n�@\n�@\n%d\n",
		scoreData->mTrashBagNum, 
		scoreData->mGoldTrashBagNum, 
		trashBagTotalScore + goldBagTotalScore);
	
	// ���l�̃e�L�X�g2��ݒ�
	mpNumText2->ChangeToStr("%d\n�@\n�@\n%d\n�@\n�@\n%.1f\n",
		trashBagPoint,
		goldTrashBagPoint,
		bonus);
	
	// �v�Z���ʂ̃e�L�X�g��ݒ�
	mpTotalText->ChangeToStr("%d\n�@\n�@\n%d\n�@\n�@\n%d\n",
		trashBagTotalScore,
		goldBagTotalScore,
		bonusScore);
	
	// �{�[�i�X�̃e�L�X�g��ݒ�
	mpBonusDescriptionText->SetStr("�cHP����");

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
	SAFE_DELETE(mpTrashBagImg);
	SAFE_DELETE(mpGoldTrashBagImg);
}

// �X�V
void CTrashGameResultUI::Update()
{
	CResultUIBase::Update();
	// �摜
	mpTrashBagImg->Update();
	mpGoldTrashBagImg->Update();
}

// �`��
void CTrashGameResultUI::Render()
{
	CResultUIBase::Render();
	// �摜
	mpTrashBagImg->Render();
	mpGoldTrashBagImg->Render();
}
