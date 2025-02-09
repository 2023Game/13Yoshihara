#include "CTrashScoreUI.h"
#include "CTextUI2D.h"
#include "CTrashPlayer.h"
#include "CTrashBagStatus.h"
#include "CScoreManager.h"
#include "CText.h"

// �R���X�g���N�^
CTrashScoreUI::CTrashScoreUI()
	: CScoreUIBase()
{
	// �S�~�܂̓��_�擾�p
	CTrashBagStatus* trashBagStatus = new CTrashBagStatus(false);
	// �S�~�܂̓��_���擾
	mTrashBagScore = trashBagStatus->GetPoint(false);
	mGoldTrashBagScore = trashBagStatus->GetPoint(true);
	// �s�v�Ȃ̂ō폜
	delete trashBagStatus;
}

// �f�X�g���N�^
CTrashScoreUI::~CTrashScoreUI()
{
}

// �X�V
void CTrashScoreUI::Update()
{
	CScoreUIBase::Update();
}

// �X�R�A���v�Z���Ď擾����
int CTrashScoreUI::GetScore() const
{
	// �v���C���[���擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// �v���C���[�̎����Ă���ʏ�ƃS�[���h�̃S�~�܂̐����擾
	int trashBagNum = player->GetTrashBag();
	int goldTrashBagNum = player->GetGoldTrashBag();
	// �S�~�܂̐��Ɠ��_���|���ăX�R�A���v�Z
	int score = trashBagNum * mTrashBagScore;
	score += goldTrashBagNum * mGoldTrashBagScore;
	return score;
}
