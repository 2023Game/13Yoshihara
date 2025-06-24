#pragma once

class CEnemyStateBase;

// 状況情報からスコア評価を決めるクラス
class CEnemyContext
{
public:
	// インスタンス
	static CEnemyContext* Instance();

	// 状況情報
	struct EnemyContext
	{
		float hpRatio;			// HPの割合
		float mpRatio;			// MPの割合
		float hpRatioP;			// プレイヤーのHPの割合
		float mpRatioP;			// プレイヤーのMPの割合
		float distanceToPlayer;	// プレイヤーまでの距離
		bool isPlayerCasting;	// プレイヤーが詠唱しているか
		bool isSpellComing;		// 呪文が飛んできているか
		float comingSpellScore;	// 飛んできている呪文のスコア
	};

	// 最適な行動を取得
	CEnemyStateBase* GetBestState(const EnemyContext& context);

	// 待機のスコアを計算
	float ScoreIdle(const EnemyContext& context);
	// 詠唱のスコアを計算
	float ScoreCast(const EnemyContext& context);
	// 追跡のスコアを計算
	float ScoreChase(const EnemyContext& context);
	// 回避のスコアを計算
	float ScoreDodge(const EnemyContext& context);
	// 逃走のスコアを計算
	float ScoreRun(const EnemyContext& context);
};