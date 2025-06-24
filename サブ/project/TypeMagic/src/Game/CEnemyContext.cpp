#include "CEnemyContext.h"

#include "CEnemyIdleState.h"
#include "CEnemyCastState.h"
#include "CEnemyChaseState.h"
#include "CEnemyDodgeState.h"
#include "CEnemyRunState.h"

// 少ないHPの閾値
#define LOW_HP_THRESHOLD 0.4f
// 多いHPの閾値
#define HIGH_HP_THRESHOLD 0.6f
// 少ないMPの閾値
#define LOW_MP_THRESHOLD 0.2f
// 多いMPの閾値
#define HIGH_MP_THRESHOLD 0.6f
// 近いプレイヤーとの距離の閾値
#define NEAR_PLAYER_DISTANCE 100.0f
// 遠いプレイヤーとの距離の閾値
#define FAR_PLAYER_DISTANCE 250.0f
// スコアの加算量
#define SCORE_AMOUNT 1.0f

// インスタンス
CEnemyContext* CEnemyContext::Instance()
{
    static CEnemyContext instance;
    return &instance;
}

// 待機のスコアを計算
float CEnemyContext::ScoreIdle(const EnemyContext& context)
{
    // 常に優先度低め
    return 0.1f;
}

// 詠唱のスコアを計算
float CEnemyContext::ScoreCast(const EnemyContext& context)
{
    float score = 0.0f;
    // HP残量が多ければ
    if (context.hpRatio > HIGH_HP_THRESHOLD)
        score += SCORE_AMOUNT;  // 上昇

    // HP残量がプレイヤーより多ければ
    if (context.hpRatio > context.hpRatioP)
        score += SCORE_AMOUNT;  // 上昇

    // MP残量が多ければ
    if (context.mpRatio > HIGH_MP_THRESHOLD)
        score += SCORE_AMOUNT * 2;  // 上昇
    // MP残量が少なければ
    else if (context.mpRatio < LOW_MP_THRESHOLD)
        score -= SCORE_AMOUNT * 2;  // 低下

    // プレイヤーのMP残量が少なければ
    if (context.mpRatioP < LOW_MP_THRESHOLD)
        score += SCORE_AMOUNT;      // 上昇
    
    // プレイヤーが近ければ
    if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // 上昇
    // 近くないなら
    else
        score -= SCORE_AMOUNT * 4;  // 低下
    
    // プレイヤーが詠唱していなければ
    if (!context.isPlayerCasting)
        score += SCORE_AMOUNT;  // 上昇

    // 呪文が飛んできていないなら
    if (!context.isSpellComing)
        score += SCORE_AMOUNT;  // 上昇

    return score;
}

// 追跡のスコアを計算
float CEnemyContext::ScoreChase(const EnemyContext& context)
{
    float score = 0.0f;
    // HP残量が多ければ
    if (context.hpRatio > HIGH_HP_THRESHOLD)
        score += SCORE_AMOUNT;  // 上昇

    // HP残量がプレイヤーより多ければ
    if (context.hpRatio > context.hpRatioP)
        score += SCORE_AMOUNT;  // 上昇

    // MP残量が多ければ
    if (context.mpRatio > HIGH_MP_THRESHOLD)
        score += SCORE_AMOUNT;  // 上昇

    // プレイヤーのMP残量が少なければ
    if (context.mpRatioP < LOW_MP_THRESHOLD)
        score += SCORE_AMOUNT;      // 上昇

    // プレイヤーが近くなければ
    if (context.distanceToPlayer > NEAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // 上昇
    // 近いなら
    else
        score -= SCORE_AMOUNT * 4;  // 低下

    // プレイヤーが詠唱していなければ
    if (!context.isPlayerCasting)
        score += SCORE_AMOUNT;  // 上昇

    // 呪文が飛んできていないなら
    if (!context.isSpellComing)
        score += SCORE_AMOUNT;  // 上昇


    return score;
}

// 回避のスコアを計算
float CEnemyContext::ScoreDodge(const EnemyContext& context)
{
    float score = 0.0f;
    // HP残量が少なければ
    if (context.hpRatio < LOW_HP_THRESHOLD)
        score += SCORE_AMOUNT;  // 上昇

    // MP残量が少なければ
    if (context.mpRatio < LOW_MP_THRESHOLD)
        score += SCORE_AMOUNT;  // 上昇

    // プレイヤーが近ければ
    if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // 上昇

    // プレイヤーが詠唱していれば
    if (context.isPlayerCasting)
        score += SCORE_AMOUNT;  // 上昇
    // いなければ
    else
        score -= SCORE_AMOUNT;  // 低下

    // 呪文が飛んできているなら
    if (context.isSpellComing)
        score += context.comingSpellScore * 2;  // 上昇

    return score;
}

// 逃走のスコアを計算
float CEnemyContext::ScoreRun(const EnemyContext& context)
{
    float score = 0.0f;
    // HP残量が少なければ
    if (context.hpRatio < LOW_HP_THRESHOLD)
        score += SCORE_AMOUNT;  // 上昇
    /// それ以外
    else
        score -= SCORE_AMOUNT;  // 低下
    
    // MP残量が少なければ
    if (context.mpRatio < LOW_MP_THRESHOLD)
        score += SCORE_AMOUNT * 2;  // 上昇
    /// それ以外
    else
        score -= SCORE_AMOUNT;  // 低下

    // プレイヤーが遠くなければ
    if (context.distanceToPlayer < FAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // 上昇

    // プレイヤーが詠唱していれば
    if (context.isPlayerCasting)
        score += SCORE_AMOUNT;  // 上昇

    // 呪文が飛んできているなら
    if (context.isSpellComing)
        score += context.comingSpellScore;  // 上昇

    return score;
}

// 最適な行動を取得
CEnemyStateBase* CEnemyContext::GetBestState(const EnemyContext& context)
{
    std::vector<std::pair<float, CEnemyStateBase*>> scores = {
        {ScoreIdle(context),    CEnemyIdleState::Instance()},
        {ScoreCast(context),    CEnemyCastState::Instance()},
        {ScoreChase(context),   CEnemyChaseState::Instance()},
        {ScoreDodge(context),   CEnemyDodgeState::Instance()},
        {ScoreRun(context),     CEnemyRunState::Instance()},
    };

    // 初期値は待機が最適
    auto best = scores[0];

    // 全てを比べる
    for (int i = 1; i < scores.size(); i++)
    {
        // 現在の最適な行動よりスコアが高ければ
        if (best.first < scores[i].first)
        {
            // 最適な行動に設定
            best = scores[i];
        }
    }

    // 最適な行動の状態を返す
    return best.second;
}
