#pragma once

class CTextUI2D;
class CTrashBagStatus;

// �S�~�E���̃X�R�A�\��UI
class CTrashScoreUI
{
public:
	// �X�R�A�\��UI�̃C���X�^���X���擾
	static CTrashScoreUI* Instance();
	// �R���X�g���N�^
	CTrashScoreUI();
	// �f�X�g���N�^
	~CTrashScoreUI();

	// �X�V
	void Update();

	// �X�R�A���擾����
	int GetScore() const;

private:
	// �X�R�A�\��UI�̃C���X�^���X
	static CTrashScoreUI* spInstance;
	// �X�R�A
	int mScore;

	// �S�~�܈�̓��_
	int mTrashBagScore;
	// �S�[���h�̃S�~�܈�̓��_
	int mGoldTrashBagScore;
	// �e�L�X�gUI
	CTextUI2D* mpScoreUI;
};