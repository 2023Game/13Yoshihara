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

	// �X�R�A���v�Z���Ď擾����
	int GetScore() const override;

	// �S�~�܈�̓��_���擾
	int GetTrashBagScore() const;
	// �S�[���h�̃S�~�܈�̓��_���擾
	int GetGoldTrashBagScore() const;

private:
	// �S�~�܈�̓��_
	int mTrashBagScore;
	// �S�[���h�̃S�~�܈�̓��_
	int mGoldTrashBagScore;
};