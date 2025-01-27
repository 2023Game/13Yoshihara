#pragma once

class CTextUI2D;
class CTrashBagStatus;

// �S�~�E���̃X�R�A�\��UI
class CTrashScoreUI
{
public:
	// �R���X�g���N�^
	CTrashScoreUI();
	// �f�X�g���N�^
	~CTrashScoreUI();

	// �X�V
	void Update();

	// �X�R�A���擾����
	int GetScore() const;

private:
	// �X�R�A
	int mScore;

	// �S�~�܂��Ƃ̓��_�擾�p
	CTrashBagStatus* mpTrashBagStatus;
	// �e�L�X�gUI
	CTextUI2D* mpScoreUI;
};