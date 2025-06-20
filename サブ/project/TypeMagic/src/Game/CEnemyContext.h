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
		float distanceToPlayer;	// プレイヤーまでの距離
		bool isPlayerCasting;	// プレイヤーが詠唱しているか
		bool isNearWall;		// 近くに壁があるか
	};

	// 最適な行動を取得
	CEnemyStateBase* GetBestState(const EnemyContext& context);

private:
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