#include "CTrashScoreUI.h"
#include "CTextUI2D.h"
#include "CTrashPlayer.h"
#include "CTrashBagStatus.h"
#include "CScoreManager.h"
#include "CText.h"

// �X�R�AUI�̃I�t�Z�b�g���W
#define SCORE_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.35f,WINDOW_HEIGHT - 100.0f,0.0f)

// �X�R�A�\��UI�̃C���X�^���X
CTrashScoreUI* CTrashScoreUI::spInstance = nullptr;

// �X�R�A�\��UI�̃C���X�^���X���擾
CTrashScoreUI* CTrashScoreUI::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CTrashScoreUI::CTrashScoreUI()
	: mScore(0)
{
	// �C���X�^���X��ݒ�
	spInstance = this;
	// �X�R�A��UI�𐶐�
	mpScoreUI = new CTextUI2D();
	// �����̑����̊��ݒ�
	mpScoreUI->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// ���W��ݒ�
	mpScoreUI->Position(SCORE_UI_OFFSET_POS);

	// �S�~�܂̓��_�擾�p
	CTrashBagStatus* trashBagStatus = new CTrashBagStatus(false);
	// �S�~�܂̓��_��ݒ�
	mTrashBagScore = trashBagStatus->GetPoint(false);
	mGoldTrashBagScore = trashBagStatus->GetPoint(true);
	// �s�v�Ȃ̂ō폜
	delete trashBagStatus;
}

// �f�X�g���N�^
CTrashScoreUI::~CTrashScoreUI()
{
	SAFE_DELETE(spInstance);
}

// �X�V
void CTrashScoreUI::Update()
{
	// �`�悷�镶�����ݒ�
	mpScoreUI->ChangeToStr("Score�F%d\n", GetScore());
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
	int score = trashBagNum * mTrashBagScore;
	score += goldTrashBagNum * mGoldTrashBagScore;
	return score;
}
