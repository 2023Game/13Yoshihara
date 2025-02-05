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
		TrashGameScoreData(int score, int bestScore,
			int trashBagNum, int goldTrashBagNum, float hpPer) :
			mScore(score), mBestScore(bestScore),
			mTrashBagNum(trashBagNum), mGoldTrashBagNum(goldTrashBagNum),
			mHpPer(hpPer){}
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

	// ゲームの種類を設定する
	void SetGameType(int gameType);
	// ゲームの種類を取得する
	int GetGameType() const;

private:
	// 得点管理クラスのインスタンス
	static CScoreManager* spInstance;
	// ゴミ拾いのスコアデータ
	TrashGameScoreData* mpTrashGameScore;
	// ゲームの種類
	int mGameType;
};