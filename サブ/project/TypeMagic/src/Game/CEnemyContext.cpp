#include "CEnemyContext.h"

#include "CEnemyIdleState.h"
#include "CEnemyCastState.h"
#include "CEnemyChaseState.h"
#include "CEnemyDodgeState.h"
#include "CEnemyRunState.h"

// ���Ȃ�HP��臒l
#define LOW_HP_THRESHOLD 0.4f
// ����HP��臒l
#define HIGH_HP_THRESHOLD 0.6f
// ���Ȃ�MP��臒l
#define LOW_MP_THRESHOLD 0.2f
// ����MP��臒l
#define HIGH_MP_THRESHOLD 0.6f
// �߂��v���C���[�Ƃ̋�����臒l
#define NEAR_PLAYER_DISTANCE 100.0f
// �����v���C���[�Ƃ̋�����臒l
#define FAR_PLAYER_DISTANCE 250.0f
// �X�R�A�̉��Z��
#define SCORE_AMOUNT 1.0f

// �C���X�^���X
CEnemyContext* CEnemyContext::Instance()
{
    static CEnemyContext instance;
    return &instance;
}

// �ҋ@�̃X�R�A���v�Z
float CEnemyContext::ScoreIdle(const EnemyContext& context)
{
    // ��ɗD��x���
    return 0.1f;
}

// �r���̃X�R�A���v�Z
float CEnemyContext::ScoreCast(const EnemyContext& context)
{
    float score = 0.0f;
    // HP�c�ʂ��������
    if (context.hpRatio > HIGH_HP_THRESHOLD)
        score += SCORE_AMOUNT;  // �㏸

    // HP�c�ʂ��v���C���[��葽�����
    if (context.hpRatio > context.hpRatioP)
        score += SCORE_AMOUNT;  // �㏸

    // MP�c�ʂ��������
    if (context.mpRatio > HIGH_MP_THRESHOLD)
        score += SCORE_AMOUNT * 2;  // �㏸
    // MP�c�ʂ����Ȃ����
    else if (context.mpRatio < LOW_MP_THRESHOLD)
        score -= SCORE_AMOUNT * 2;  // �ቺ

    // �v���C���[��MP�c�ʂ����Ȃ����
    if (context.mpRatioP < LOW_MP_THRESHOLD)
        score += SCORE_AMOUNT;      // �㏸
    
    // �v���C���[���߂����
    if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // �㏸
    // �߂��Ȃ��Ȃ�
    else
        score -= SCORE_AMOUNT * 4;  // �ቺ
    
    // �v���C���[���r�����Ă��Ȃ����
    if (!context.isPlayerCasting)
        score += SCORE_AMOUNT;  // �㏸

    // ���������ł��Ă��Ȃ��Ȃ�
    if (!context.isSpellComing)
        score += SCORE_AMOUNT;  // �㏸

    return score;
}

// �ǐՂ̃X�R�A���v�Z
float CEnemyContext::ScoreChase(const EnemyContext& context)
{
    float score = 0.0f;
    // HP�c�ʂ��������
    if (context.hpRatio > HIGH_HP_THRESHOLD)
        score += SCORE_AMOUNT;  // �㏸

    // HP�c�ʂ��v���C���[��葽�����
    if (context.hpRatio > context.hpRatioP)
        score += SCORE_AMOUNT;  // �㏸

    // MP�c�ʂ��������
    if (context.mpRatio > HIGH_MP_THRESHOLD)
        score += SCORE_AMOUNT;  // �㏸

    // �v���C���[��MP�c�ʂ����Ȃ����
    if (context.mpRatioP < LOW_MP_THRESHOLD)
        score += SCORE_AMOUNT;      // �㏸

    // �v���C���[���߂��Ȃ����
    if (context.distanceToPlayer > NEAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // �㏸
    // �߂��Ȃ�
    else
        score -= SCORE_AMOUNT * 4;  // �ቺ

    // �v���C���[���r�����Ă��Ȃ����
    if (!context.isPlayerCasting)
        score += SCORE_AMOUNT;  // �㏸

    // ���������ł��Ă��Ȃ��Ȃ�
    if (!context.isSpellComing)
        score += SCORE_AMOUNT;  // �㏸


    return score;
}

// ����̃X�R�A���v�Z
float CEnemyContext::ScoreDodge(const EnemyContext& context)
{
    float score = 0.0f;
    // HP�c�ʂ����Ȃ����
    if (context.hpRatio < LOW_HP_THRESHOLD)
        score += SCORE_AMOUNT;  // �㏸

    // MP�c�ʂ����Ȃ����
    if (context.mpRatio < LOW_MP_THRESHOLD)
        score += SCORE_AMOUNT;  // �㏸

    // �v���C���[���߂����
    if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // �㏸

    // �v���C���[���r�����Ă����
    if (context.isPlayerCasting)
        score += SCORE_AMOUNT;  // �㏸
    // ���Ȃ����
    else
        score -= SCORE_AMOUNT;  // �ቺ

    // ���������ł��Ă���Ȃ�
    if (context.isSpellComing)
        score += context.comingSpellScore * 2;  // �㏸

    return score;
}

// �����̃X�R�A���v�Z
float CEnemyContext::ScoreRun(const EnemyContext& context)
{
    float score = 0.0f;
    // HP�c�ʂ����Ȃ����
    if (context.hpRatio < LOW_HP_THRESHOLD)
        score += SCORE_AMOUNT;  // �㏸
    /// ����ȊO
    else
        score -= SCORE_AMOUNT;  // �ቺ
    
    // MP�c�ʂ����Ȃ����
    if (context.mpRatio < LOW_MP_THRESHOLD)
        score += SCORE_AMOUNT * 2;  // �㏸
    /// ����ȊO
    else
        score -= SCORE_AMOUNT;  // �ቺ

    // �v���C���[�������Ȃ����
    if (context.distanceToPlayer < FAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // �㏸

    // �v���C���[���r�����Ă����
    if (context.isPlayerCasting)
        score += SCORE_AMOUNT;  // �㏸

    // ���������ł��Ă���Ȃ�
    if (context.isSpellComing)
        score += context.comingSpellScore;  // �㏸

    return score;
}

// �œK�ȍs�����擾
CEnemyStateBase* CEnemyContext::GetBestState(const EnemyContext& context)
{
    std::vector<std::pair<float, CEnemyStateBase*>> scores = {
        {ScoreIdle(context),    CEnemyIdleState::Instance()},
        {ScoreCast(context),    CEnemyCastState::Instance()},
        {ScoreChase(context),   CEnemyChaseState::Instance()},
        {ScoreDodge(context),   CEnemyDodgeState::Instance()},
        {ScoreRun(context),     CEnemyRunState::Instance()},
    };

    // �����l�͑ҋ@���œK
    auto best = scores[0];

    // �S�Ă��ׂ�
    for (int i = 1; i < scores.size(); i++)
    {
        // ���݂̍œK�ȍs�����X�R�A���������
        if (best.first < scores[i].first)
        {
            // �œK�ȍs���ɐݒ�
            best = scores[i];
        }
    }

    // �œK�ȍs���̏�Ԃ�Ԃ�
    return best.second;
}
