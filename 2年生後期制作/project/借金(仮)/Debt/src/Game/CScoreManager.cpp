#include "CScoreManager.h"
#include "SceneType.h"

// ���_�Ǘ��N���X�̃C���X�^���X
CScoreManager* CScoreManager::spInstance = nullptr;

// �C���X�^���X�̃|�C���^�̎擾
CScoreManager* CScoreManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CScoreManager();
	}
	return spInstance;
}

// �C���X�^���X��j��
void CScoreManager::ClearInstance()
{
	SAFE_DELETE(spInstance);
}

// �R���X�g���N�^
CScoreManager::CScoreManager()
	: mGameType((int)EScene::eTrashGame)
{
	mpTrashGameScore = new CScoreManager::TrashGameScoreData(0, 0, 0, 0);
}

// �f�X�g���N�^
CScoreManager::~CScoreManager()
{
	// �C���X�^���X���폜
	spInstance = nullptr;
}

// �S�~�E���̃X�R�A�f�[�^��ݒ肷��
void CScoreManager::SetTrashGameScoreData(int score, int trashBagNum, int goldTrashBagNum)
{
	// ����̃X�R�A��ݒ�
	mpTrashGameScore->mScore = score;
	// ����̃X�R�A���ō��_��荂����ΐݒ�
	if (mpTrashGameScore->mBestScore < score)
	{
		mpTrashGameScore->mBestScore = score;
	}
	// �E�����S�~�܂̐���ݒ�
	mpTrashGameScore->mTrashBagNum = trashBagNum;
	// �E�����S�[���h�S�~�܂̐���ݒ�
	mpTrashGameScore->mGoldTrashBagNum = goldTrashBagNum;
}

// �S�~�E���̃X�R�A�f�[�^���擾����
CScoreManager::TrashGameScoreData* CScoreManager::GetTrashGameScoreData() const
{
	return mpTrashGameScore;
}

// �Q�[���̎�ނ�ݒ肷��
void CScoreManager::SetGameType(int gameType)
{
	mGameType = gameType;
}

// �Q�[���̎�ނ��擾����
int CScoreManager::GetGameType() const
{
	return mGameType;
}
