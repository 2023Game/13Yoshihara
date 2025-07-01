#include "CEnemyContext.h"
#include "Maths.h"

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
#define FAR_PLAYER_DISTANCE 350.0f

// スコアの加算量（低）
#define SCORE_LOW_AMOUNT 0.5f
// スコアの加算量（基準）
#define SCORE_AMOUNT 1.0f
// スコアの加算量（高）
#define SCORE_HIGH_AMOUNT 2.0f

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

// 詠唱（攻撃）のスコアを計算
float CEnemyContext::ScoreCast(const EnemyContext& context)
{
    float score = 0.0f;

    // HP残量が多ければ
    if (context.hpRatio > HIGH_HP_THRESHOLD)
        score += SCORE_AMOUNT;          // 上昇

    // HP残量がプレイヤーより多ければ
    if (context.hpRatio > context.hpRatioP)
        score += SCORE_AMOUNT;          // 上昇
    // プレイヤーより少なければ不利なので
    else
        score -= SCORE_AMOUNT;  // 低下

    // MP残量が多ければ
    if (context.mpRatio > HIGH_MP_THRESHOLD)
        score += SCORE_HIGH_AMOUNT;  // 上昇
    // MP残量が少なければ
    else if (context.mpRatio < LOW_MP_THRESHOLD)
        return 0.0f;  // 詠唱はしない

    // プレイヤーのMP残量が少なければ
    if (context.mpRatioP < LOW_MP_THRESHOLD)
        score += SCORE_AMOUNT;          // 上昇
    
    // プレイヤーが近ければ
    if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;          // 上昇
    // 遠いなら当たりずらいので
    else if (context.distanceToPlayer > FAR_PLAYER_DISTANCE)
        score -= SCORE_AMOUNT;          // 低下
    
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
    // 残量が少ないなら
    else if (context.mpRatio < LOW_MP_THRESHOLD)
        return 0.0f;    // 追跡はしない

    // プレイヤーのMP残量が少なければ
    if (context.mpRatioP < LOW_MP_THRESHOLD)
        score += SCORE_AMOUNT;      // 上昇

    // プレイヤーが遠いなら
    if (context.distanceToPlayer > FAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // 上昇
    // 近いならさらに近づく必要がないので
    else if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
        score -= SCORE_AMOUNT;  // 低下

    // プレイヤーが詠唱していなければ
    if (!context.isPlayerCasting)
        score += SCORE_AMOUNT;  // 上昇
    // 詠唱しているなら危ないので
    else
        score -= SCORE_AMOUNT;  // 低下

    // 呪文が飛んできていないなら
    if (!context.isSpellComing)
        score += SCORE_AMOUNT;  // 上昇
    // 呪文が飛んできているなら危ないので
    else
        score -= SCORE_HIGH_AMOUNT; // 低下

    // ブレス呪文を詠唱していたら
    if (context.castShape == ESpellShapeType::eBreath)
        score += SCORE_HIGH_AMOUNT; // 上昇

    return score;
}

// 回避のスコアを計算
float CEnemyContext::ScoreDodge(const EnemyContext& context)
{
    float score = 0.0f;
    // HP残量が少なければ
    if (context.hpRatio < LOW_HP_THRESHOLD)
        score += SCORE_HIGH_AMOUNT;  // 上昇

    // MP残量が少なければ
    if (context.mpRatio < LOW_MP_THRESHOLD)
        score += SCORE_HIGH_AMOUNT;  // 上昇
    // 多ければ呪文で回避できるので通常分上昇
    else if (context.mpRatio > HIGH_MP_THRESHOLD)
        score += SCORE_AMOUNT;  // 上昇

    // プレイヤーよりHPが少ないなら不利なので
    if (context.hpRatio < context.hpRatioP)
        score += SCORE_AMOUNT;  // 上昇

    // プレイヤーが近ければ
    if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // 上昇

    // プレイヤーが詠唱していれば
    if (context.isPlayerCasting)
        score += SCORE_HIGH_AMOUNT;  // 上昇
    
    // シールドを貼っていないなら
    if (!context.isShield)
    {
        score += SCORE_HIGH_AMOUNT;  // 上昇
    }
    // 貼っているなら
    else
        score -= SCORE_HIGH_AMOUNT;  // 低下

    // 呪文が飛んできているなら
    if (context.isSpellComing)
    {
        // 加算量の低から高までの範囲に収める
        float value = Math::Clamp(context.comingSpellScore, SCORE_LOW_AMOUNT, SCORE_HIGH_AMOUNT);
        score += value;  // 上昇
    }
    // 呪文が飛んできていないなら
    else
    {
        score = 0.0f;
    }

    return score;
}

// 逃走のスコアを計算
float CEnemyContext::ScoreRun(const EnemyContext& context)
{
    float score = 0.0f;
    // HP残量が少なければ
    if (context.hpRatio < LOW_HP_THRESHOLD)
        score += SCORE_AMOUNT;  // 上昇
    
    // MP残量が少なければ
    if (context.mpRatio < LOW_MP_THRESHOLD)
        score += SCORE_HIGH_AMOUNT;  // 上昇

    // プレイヤーが遠くなければ
    if (context.distanceToPlayer < FAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // 上昇
    // プレイヤーが遠ければ
    else
        return 0.0f;    // 逃走しない

    // プレイヤーが詠唱していれば
    if (context.isPlayerCasting)
        score += SCORE_AMOUNT;  // 上昇

    // 呪文が飛んできているなら
    if (context.isSpellComing)
    {
        // 加算量の低から高までの範囲に収める
        float value = Math::Clamp(context.comingSpellScore, SCORE_LOW_AMOUNT, SCORE_HIGH_AMOUNT);
        score += value;  // 上昇
    }

    return score;
}

// 使う攻撃呪文を決定する
ESpellShapeType CEnemyContext::ScoreAtkSpell(const EnemyContext& context)
{
    // それぞれのスコア
    float scoreBall = 0.0f;
    float scoreBolt = 0.0f;
    float scoreBreath = 0.0f;
    // 一番高いスコア
    float bestScore = 0.0f;
    ESpellShapeType bestShape = ESpellShapeType::eError;

    // プレイヤーが近いなら
    if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
        scoreBreath += SCORE_AMOUNT;    // ブレス上昇
    // プレイヤーが遠いなら
    else if (context.distanceToPlayer > FAR_PLAYER_DISTANCE)
        scoreBolt += SCORE_AMOUNT;      // ボルト上昇
    // それ以外
    else
        scoreBall += SCORE_AMOUNT;      // ボール上昇

    // プレイヤーが詠唱していたら当てられそうなので
    if (context.isPlayerCasting)
        scoreBolt += SCORE_HIGH_AMOUNT;  // ボルト上昇

    // 一番高いスコアをボールのスコアに設定
    bestScore = scoreBall;
    bestShape = ESpellShapeType::eBall;

    // ボルトのスコアの方が高ければ設定
    if (scoreBolt > bestScore)
    {
        bestScore = scoreBolt;
        bestShape = ESpellShapeType::eBolt;
    }
    // ブレスのスコアの方が高ければ設定
    if (scoreBreath > bestScore)
    {
        bestScore = scoreBreath;
        bestShape = ESpellShapeType::eBreath;
    }

    return bestShape;
}

// 使うサポート呪文を決定する
ESpellShapeType CEnemyContext::ScoreSpSpell(const EnemyContext& context)
{
    float scoreTeleport = 0.0f;
    float scoreShield = 0.0f;
    float scoreReflector = 0.0f;
    // 一番高いスコア
    float bestScore = 0.0f;
    ESpellShapeType bestShape = ESpellShapeType::eError;

    // 飛んできているのがブレスなら
    if (context.comingShape == ESpellShapeType::eBreath)
    {
        scoreTeleport += SCORE_AMOUNT;  // テレポート上昇
    }
    // ボルトなら
    else if (context.comingShape == ESpellShapeType::eBolt)
    {
        scoreReflector += SCORE_AMOUNT;     // リフレクター上昇
    }
    // ボールなら
    else if (context.comingShape == ESpellShapeType::eBall)
    {
        scoreShield += SCORE_AMOUNT;    // シールド上昇
    }

    // プレイヤーが近いなら
    if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
    {
        scoreTeleport += SCORE_AMOUNT;  // テレポート上昇
    }
    // プレイヤーが遠いなら
    else if (context.distanceToPlayer > FAR_PLAYER_DISTANCE)
    {
        scoreShield += SCORE_AMOUNT;    // シールド上昇
    }
    // それ以外
    else
    {
        scoreReflector += SCORE_AMOUNT; // リフレクター上昇
    }

    // 既にシールドを張っているなら
    if (context.isShield)
    {
        scoreShield = -1.0f;     // 張りなおさない
    }

    // 一番高いスコアをシールドのスコアに設定
    bestScore = scoreShield;
    bestShape = ESpellShapeType::eShield;

    // テレポートのスコアの方が高ければ設定
    if (scoreTeleport > bestScore)
    {
        bestScore = scoreTeleport;
        bestShape = ESpellShapeType::eTeleport;
    }
    // リフレクターのスコアの方が高ければ設定
    if (scoreReflector > bestScore)
    {
        bestScore = scoreReflector;
        bestShape = ESpellShapeType::eReflector;
    }

    return bestShape;
}

// 近いかどうか
bool CEnemyContext::IsNear(float dist)
{
    return dist < NEAR_PLAYER_DISTANCE ? true : false;
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
