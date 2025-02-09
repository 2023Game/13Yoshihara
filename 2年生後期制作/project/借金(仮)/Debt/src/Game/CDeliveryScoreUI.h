#pragma once
#include "CScoreUIBase.h"

class CDeliveryScoreUI : public CScoreUIBase
{
public:
	// �R���X�g���N�^
	CDeliveryScoreUI();
	// �f�X�g���N�^
	~CDeliveryScoreUI();

	// �X�V
	void Update();

	// �X�R�A���擾����
	int GetScore() const override;

private:
	// �z�B1������̓��_
	int mDeliveryItemScore;
	// �G��1�̉󂷂ƖႦ�链�_
	int mDestroyEnemyScore;
};