#pragma once
#include "CScoreUIBase.h"

// �S�~�E���̃X�R�A�\��UI
class CTrashScoreUI : public CScoreUIBase
{
public:
	// �R���X�g���N�^
	CTrashScoreUI();
	// �f�X�g���N�^
	~CTrashScoreUI();

	// �X�V
	void Update();

	// �X�R�A���擾����
	int GetScore() const override;

private:
	// �S�~�܈�̓��_
	int mTrashBagScore;
	// �S�[���h�̃S�~�܈�̓��_
	int mGoldTrashBagScore;
};