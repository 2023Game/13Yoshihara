#pragma once

/*
全ての得点管理クラス
*/
class CScoreManager
{
public:
	// ゴミ拾いのスコアデータ
	struct TrashGameScoreData
	{
		int mScore;					// スコア
		int mBestScore;				// 最高スコア
		int mTrashBagNum;			// 拾ったゴミ袋の数
		int mGoldTrashBagNum;		// 拾ったゴールドゴミ袋の数
		float mHpPer;				// 残りHPの割合
		TrashGameScoreData() :
			mScore(0), mBestScore(0),
			mTrashBagNum(0), mGoldTrashBagNum(0),
			mHpPer(0.0f){}
	};
	// 配達のスコアデータ
	struct DeliveryGameScoreData
	{
		int mScore;				// スコア
		int mBestScore;			// 最高スコア
		int mDeliveryNum;		// 配達した数
		int mDestroyEnemyNum;	// 壊した敵の数
		float mAccuracy;		// 命中率
		DeliveryGameScoreData() :
			mScore(0), mBestScore(0),
			mDeliveryNum(0), 
			mDestroyEnemyNum(0),
			mAccuracy(0.0f){}
	};

	// インスタンスを取得
	static CScoreManager* Instance();
	// インスタンスを破棄
	static void ClearInstance();

	// コンストラクタ
	CScoreManager();
	// デストラクタ
	~CScoreManager();

	// ゴミ拾いのスコアデータを設定する
	void SetTrashGameScoreData(int score, int trashBagNum,
		int goldTrashBagNum, float hpPer);
	// ゴミ拾いのスコアデータを取得する
	TrashGameScoreData* GetTrashGameScoreData() const;

	// 配達のスコアデータを設定する
	void SetDeliveryGameScoreData(int score, int deliveryNum,
		int destroyEnemyNum, float accuracy);
	// 配達のスコアデータを取得する
	DeliveryGameScoreData* GetDeliveryGameScoreData() const;

	// ゲームの種類を設定する
	void SetGameType(int gameType);
	// ゲームの種類を取得する
	int GetGameType() const;

private:
	// 得点管理クラスのインスタンス
	static CScoreManager* spInstance;
	// ゴミ拾いのスコアデータ
	TrashGameScoreData* mpTrashGameScore;
	// 配達のスコアデータ
	DeliveryGameScoreData* mpDeliveryGameScore;
	// ゲームの種類
	int mGameType;
};