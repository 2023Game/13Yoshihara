#include "CTrashScoreUI.h"
#include "CTextUI2D.h"
#include "CTrashPlayer.h"
#include "CTrashBagStatus.h"

// �X�R�AUI�̃I�t�Z�b�g���W
#define SCORE_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.75f,WINDOW_HEIGHT - 100.0f,0.0f)

// �R���X�g���N�^
CTrashScoreUI::CTrashScoreUI()
	: mScore(0)
{
	mpScoreUI = new CTextUI2D();
	mpTrashBagStatus = new CTrashBagStatus(false);
}

// �f�X�g���N�^
CTrashScoreUI::~CTrashScoreUI()
{
	SAFE_DELETE(mpTrashBagStatus);
}

// �X�V
void CTrashScoreUI::Update()
{
	// �`�悷�镶�����ݒ�
	mpScoreUI->ChangeToStr("Score�F%d\n", GetScore());
	// ���W��ݒ�
	mpScoreUI->Position(SCORE_UI_OFFSET_POS);
}

// �X�R�A���擾����
int CTrashScoreUI::GetScore() const
{
	// �v���C���[���擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// �v���C���[�̎����Ă���ʏ�ƃS�[���h�̃S�~�܂̐����擾
	int trashBagNum = player->GetTrashBag();
	int goldTrashBagNum = player->GetGoldTrashBag();
	// �S�~�܃X�e�[�^�X����S�~�܂̃|�C���g���擾����
	// �S�~�܂̐��Ɠ��_���|���ăX�R�A���v�Z
	int score = trashBagNum * mpTrashBagStatus->GetPoint(false);
	score += goldTrashBagNum * mpTrashBagStatus->GetPoint(true);
	return score;
}
