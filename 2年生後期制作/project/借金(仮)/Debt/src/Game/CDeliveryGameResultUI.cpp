#include "CDeliveryGameResultUI.h"
#include "CText.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CScoreManager.h"
#include "CDeliveryItemStatus.h"
#include "CDeliveryEnemyStatus.h"
#include "CExpandButton.h"

// �w�i�摜�̃p�X
#define BACKGROUND_PATH "UI/delivery_game_result_bg.png"
// �摜�̃p�X
#define DELIVERY_ITEM_PATH "UI/deliveryItem.png"
#define DELIVERY_TRUCK_PATH "UI/deliveryTruck.png"

// �摜�̃T�C�Y�̔{��
#define SIZE_RATE 0.25f
// �z�B���̉摜�̃I�t�Z�b�g���W
#define DELIVERY_ITEM_OFFEST_POS	CVector2(WINDOW_WIDTH * 0.225f, WINDOW_HEIGHT * 0.38f)
// �g���b�N�̉摜�̃I�t�Z�b�g���W
#define DELIVERY_TRUCK_OFFSET_POS	CVector2(WINDOW_WIDTH * 0.225f, WINDOW_HEIGHT * 0.59f)

// �R���X�g���N�^
CDeliveryGameResultUI::CDeliveryGameResultUI()
	: CResultUIBase(BACKGROUND_PATH)
{
	// �z�B���̓��_�擾�p
	CDeliveryItemStatus* itemStatus = new CDeliveryItemStatus();
	// �z�B���̓��_���擾
	int deliveryItemPoint = itemStatus->GetScore();
	// �s�v�Ȃ̂ō폜
	delete itemStatus;

	// �G���󂵂����̓��_�擾�p
	CDeliveryEnemyStatus* enemyStatus = new CDeliveryEnemyStatus();
	// �G���󂵂����̓��_���擾
	int destroyEnemyPoint = enemyStatus->GetScore();
	// �s�v�Ȃ̂ō폜
	delete enemyStatus;

	// ���_�Ǘ��N���X���擾
	CScoreManager* scoreMgr = CScoreManager::Instance();
	// �z�B�̃X�R�A�f�[�^���擾
	CScoreManager::DeliveryGameScoreData* scoreData =
		scoreMgr->GetDeliveryGameScoreData();

	// �z�B�����X�R�A�̍��v
	int deliveryItemTotalScore =
		scoreData->mDeliveryNum * deliveryItemPoint;
	// �󂵂��G�̃X�R�A�̍��v
	int destroyEnemyTotalScore =
		scoreData->mDestroyEnemyNum * destroyEnemyPoint;
	// �S�ẴX�R�A�̍��v
	int totalScore = deliveryItemTotalScore + destroyEnemyTotalScore;
	// �{�[�i�X�̔{��
	float bonus = scoreData->mAccuracy;
	// �{�[�i�X�̃X�R�A���v�Z
	int bonusScore = totalScore * bonus;
	// ���v�X�R�A�ɉ��Z����
	totalScore += bonusScore;

	// �X�R�A��ݒ�
	SetScore(totalScore);

	// ���_�̃e�L�X�g��ݒ�
	mpScorePointText->ChangeToStr("%d�~\n", totalScore);
	
	// ���l�̃e�L�X�g1��ݒ�
	mpNumText1->ChangeToStr("%d\n�@\n�@\n%d\n�@\n�@\n%d\n",
		scoreData->mDeliveryNum, 
		scoreData->mDestroyEnemyNum, 
		deliveryItemTotalScore + destroyEnemyTotalScore);
	
	// ���l�̃e�L�X�g2��ݒ�
	mpNumText2->ChangeToStr("%d\n�@\n�@\n%d\n�@\n�@\n%.2f\n",
		deliveryItemPoint,
		destroyEnemyPoint,
		bonus);
	
	// �v�Z���ʂ̃e�L�X�g��ݒ�
	mpTotalText->ChangeToStr("%d\n�@\n�@\n%d\n�@\n�@\n%d\n",
		deliveryItemTotalScore,
		destroyEnemyTotalScore,
		bonusScore);
	
	// �e�L�X�g��ݒ�
	mpBonusDescriptionText->SetStr("������");
	
	// �z�B���̉摜�𐶐�
	mpDeliveryItemImg = new CImage
	(
		DELIVERY_ITEM_PATH,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	// �T�C�Y�̎擾
	CVector2 deliveryItemSize = mpDeliveryItemImg->GetSize();
	// �T�C�Y�̔{�����|�����l��ݒ�
	mpDeliveryItemImg->SetSize(deliveryItemSize* SIZE_RATE);
	// ���W��ݒ�
	mpDeliveryItemImg->SetPos(DELIVERY_ITEM_OFFEST_POS);

	// �g���b�N�̉摜�𐶐�
	mpDeliveryTruckImg = new CImage
	(
		DELIVERY_TRUCK_PATH,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	// �T�C�Y�̎擾
	CVector2 deliveryTruckSize = mpDeliveryTruckImg->GetSize();
	// �T�C�Y�̔{�����|�����l��ݒ�
	mpDeliveryTruckImg->SetSize(deliveryTruckSize* SIZE_RATE);
	// ���W��ݒ�
	mpDeliveryTruckImg->SetPos(DELIVERY_TRUCK_OFFSET_POS);
}

// �f�X�g���N�^
CDeliveryGameResultUI::~CDeliveryGameResultUI()
{
	SAFE_DELETE(mpDeliveryItemImg);
	SAFE_DELETE(mpDeliveryTruckImg);
}

// �X�V
void CDeliveryGameResultUI::Update()
{
	CResultUIBase::Update();
	// �摜
	mpDeliveryItemImg->Update();
	mpDeliveryTruckImg->Update();
}

// �`��
void CDeliveryGameResultUI::Render()
{
	CResultUIBase::Render();
	// �摜
	mpDeliveryItemImg->Render();
	mpDeliveryTruckImg->Render();
}
