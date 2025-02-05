#pragma once

/*
�S�Ă̓��_�Ǘ��N���X
*/
class CScoreManager
{
public:
	// �S�~�E���̃X�R�A�f�[�^
	struct TrashGameScoreData
	{
		int mScore;					// �X�R�A
		int mBestScore;				// �ō��X�R�A
		int mTrashBagNum;			// �E�����S�~�܂̐�
		int mGoldTrashBagNum;		// �E�����S�[���h�S�~�܂̐�
		float mHpPer;				// �c��HP�̊���
		TrashGameScoreData(int score, int bestScore,
			int trashBagNum, int goldTrashBagNum, float hpPer) :
			mScore(score), mBestScore(bestScore),
			mTrashBagNum(trashBagNum), mGoldTrashBagNum(goldTrashBagNum),
			mHpPer(hpPer){}
	};

	// �C���X�^���X���擾
	static CScoreManager* Instance();
	// �C���X�^���X��j��
	static void ClearInstance();

	// �R���X�g���N�^
	CScoreManager();
	// �f�X�g���N�^
	~CScoreManager();

	// �S�~�E���̃X�R�A�f�[�^��ݒ肷��
	void SetTrashGameScoreData(int score, int trashBagNum,
		int goldTrashBagNum, float hpPer);
	// �S�~�E���̃X�R�A�f�[�^���擾����
	TrashGameScoreData* GetTrashGameScoreData() const;

	// �Q�[���̎�ނ�ݒ肷��
	void SetGameType(int gameType);
	// �Q�[���̎�ނ��擾����
	int GetGameType() const;

private:
	// ���_�Ǘ��N���X�̃C���X�^���X
	static CScoreManager* spInstance;
	// �S�~�E���̃X�R�A�f�[�^
	TrashGameScoreData* mpTrashGameScore;
	// �Q�[���̎��
	int mGameType;
};