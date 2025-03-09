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
		TrashGameScoreData() :
			mScore(0), mBestScore(0),
			mTrashBagNum(0), mGoldTrashBagNum(0),
			mHpPer(0.0f){}
	};
	// �z�B�̃X�R�A�f�[�^
	struct DeliveryGameScoreData
	{
		int mScore;				// �X�R�A
		int mBestScore;			// �ō��X�R�A
		int mDeliveryNum;		// �z�B������
		int mDestroyEnemyNum;	// �󂵂��G�̐�
		float mAccuracy;		// ������
		DeliveryGameScoreData() :
			mScore(0), mBestScore(0),
			mDeliveryNum(0), 
			mDestroyEnemyNum(0),
			mAccuracy(0.0f){}
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

	// �z�B�̃X�R�A�f�[�^��ݒ肷��
	void SetDeliveryGameScoreData(int score, int deliveryNum,
		int destroyEnemyNum, float accuracy);
	// �z�B�̃X�R�A�f�[�^���擾����
	DeliveryGameScoreData* GetDeliveryGameScoreData() const;

	// �Q�[���̎�ނ�ݒ肷��
	void SetGameType(int gameType);
	// �Q�[���̎�ނ��擾����
	int GetGameType() const;

private:
	// ���_�Ǘ��N���X�̃C���X�^���X
	static CScoreManager* spInstance;
	// �S�~�E���̃X�R�A�f�[�^
	TrashGameScoreData* mpTrashGameScore;
	// �z�B�̃X�R�A�f�[�^
	DeliveryGameScoreData* mpDeliveryGameScore;
	// �Q�[���̎��
	int mGameType;
};