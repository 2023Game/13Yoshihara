#include "CDeliveryScoreUI.h"
#include "CDeliveryItemStatus.h"
#include "CDeliveryEnemyStatus.h"
#include "CDeliveryPlayer.h"

// �R���X�g���N�^
CDeliveryScoreUI::CDeliveryScoreUI()
	: CScoreUIBase()
{
	// �z�B���̓��_�擾�p
	CDeliveryItemStatus* itemStatus = new CDeliveryItemStatus();
	// �z�B���̓��_���擾
	mDeliveryItemScore = itemStatus->GetScore();
	// �s�v�Ȃ̂ō폜
	delete itemStatus;

	// �G���󂵂����̓��_�擾�p
	CDeliveryEnemyStatus* enemyStatus = new CDeliveryEnemyStatus();
	// �G���󂵂����̓��_���擾
	mDestroyEnemyScore = enemyStatus->GetScore();
	// �s�v�Ȃ̂ō폜
	delete enemyStatus;
}

// �f�X�g���N�^
CDeliveryScoreUI::~CDeliveryScoreUI()
{
}

// �X�V
void CDeliveryScoreUI::Update()
{
	CScoreUIBase::Update();
}

// �X�R�A���v�Z���Ď擾����
int CDeliveryScoreUI::GetScore() const
{
	// �v���C���[���擾
	CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(CDeliveryPlayer::Instance());
	// �z�B���������擾
	int deliveryNum = player->GetDeliveryNum();
	// �󂵂��G�̐����擾
	int destroyNum = player->GetDestroyEnemyNum();
	// ���Ɠ��_���|���ăX�R�A���v�Z
	int score = deliveryNum * mDeliveryItemScore;
	score += destroyNum * mDestroyEnemyScore;
	return score;
}

// �z�B1������̓��_���擾
int CDeliveryScoreUI::GetDeliveryItemScore() const
{
	return mDeliveryItemScore;
}

// �G��1�̉󂷂ƖႦ�链�_���擾
int CDeliveryScoreUI::GetDestroyEnemyScore() const
{
	return mDestroyEnemyScore;
}
