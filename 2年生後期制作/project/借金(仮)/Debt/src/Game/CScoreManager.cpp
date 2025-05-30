#include "CScoreManager.h"
#include "SceneType.h"

// 得点管理クラスのインスタンス
CScoreManager* CScoreManager::spInstance = nullptr;

// インスタンスのポインタの取得
CScoreManager* CScoreManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CScoreManager();
	}
	return spInstance;
}

// インスタンスを破棄
void CScoreManager::ClearInstance()
{
	SAFE_DELETE(spInstance);
}

// コンストラクタ
CScoreManager::CScoreManager()
	: mGameType((int)EScene::eTrashGame)
{
	mpTrashGameScore = new CScoreManager::TrashGameScoreData();
	mpDeliveryGameScore = new CScoreManager::DeliveryGameScoreData();
}

// デストラクタ
CScoreManager::~CScoreManager()
{
	// インスタンスを削除
	spInstance = nullptr;
}

// ゴミ拾いのスコアデータを設定する
void CScoreManager::SetTrashGameScoreData(int score, int trashBagNum,
	int goldTrashBagNum, float hpPer)
{
	// 今回のスコアを設定
	mpTrashGameScore->mScore = score;
	// 今回のスコアが最高点より高ければ設定
	if (mpTrashGameScore->mBestScore < score)
	{
		mpTrashGameScore->mBestScore = score;
	}
	// 拾ったゴミ袋の数を設定
	mpTrashGameScore->mTrashBagNum = trashBagNum;
	// 拾ったゴールドゴミ袋の数を設定
	mpTrashGameScore->mGoldTrashBagNum = goldTrashBagNum;
	// 残りHPの割合を取得
	mpTrashGameScore->mHpPer = hpPer;
}

// ゴミ拾いのスコアデータを取得する
CScoreManager::TrashGameScoreData* CScoreManager::GetTrashGameScoreData() const
{
	return mpTrashGameScore;
}

// 配達のスコアデータを設定する
void CScoreManager::SetDeliveryGameScoreData(int score, int deliveryNum,
	int destroyEnemyNum, float accuracy)
{
	// 今回のスコアを設定
	mpDeliveryGameScore->mScore = score;
	// 今回のスコアが最高点より高ければ設定
	if (mpDeliveryGameScore->mBestScore < score)
	{
		mpDeliveryGameScore->mBestScore = score;
	}
	// 配達した数を設定
	mpDeliveryGameScore->mDeliveryNum = deliveryNum;
	// 壊した敵の数を設定
	mpDeliveryGameScore->mDestroyEnemyNum = destroyEnemyNum;
	// 命中率を設定
	mpDeliveryGameScore->mAccuracy = accuracy;
}

// 配達のスコアデータを取得する
CScoreManager::DeliveryGameScoreData* CScoreManager::GetDeliveryGameScoreData() const
{
	return mpDeliveryGameScore;
}

// ゲームの種類を設定する
void CScoreManager::SetGameType(int gameType)
{
	mGameType = gameType;
}

// ゲームの種類を取得する
int CScoreManager::GetGameType() const
{
	return mGameType;
}
