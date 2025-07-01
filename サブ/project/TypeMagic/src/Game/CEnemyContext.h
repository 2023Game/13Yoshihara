#pragma once
#include "SpellType.h"

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
		float distanceToSpell;	// 飛んできてる呪文との距離
		bool isPlayerCasting;	// プレイヤーが詠唱しているか
		bool isSpellComing;		// 呪文が飛んできているか
		ESpellShapeType comingShape;	// 飛んできている呪文の形
		float comingSpellScore;			// 飛んできている呪文のスコア
		bool isShield;					// シールドを張っているか
		ESpellShapeType castShape;		// 詠唱中の呪文の形
	};

	// 最適な行動を取得
	CEnemyStateBase* GetBestState(const EnemyContext& context);

	// 待機のスコアを計算
	float ScoreIdle(const EnemyContext& context);
	// 詠唱(攻撃)のスコアを計算
	float ScoreCast(const EnemyContext& context);
	// 追跡のスコアを計算
	float ScoreChase(const EnemyContext& context);
	// 回避のスコアを計算
	float ScoreDodge(const EnemyContext& context);
	// 逃走のスコアを計算
	float ScoreRun(const EnemyContext& context);

	// 使う攻撃呪文を決定する
	ESpellShapeType ScoreAtkSpell(const EnemyContext& context);
	// 使うサポート呪文を決定する
	ESpellShapeType ScoreSpSpell(const EnemyContext& context);

	// 近いかどうか
	bool IsNear(float dist);
};