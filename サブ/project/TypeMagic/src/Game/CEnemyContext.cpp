#include "CEnemyContext.h"
#include "Maths.h"

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
#define FAR_PLAYER_DISTANCE 350.0f

// �X�R�A�̉��Z�ʁi��j
#define SCORE_LOW_AMOUNT 0.5f
// �X�R�A�̉��Z�ʁi��j
#define SCORE_AMOUNT 1.0f
// �X�R�A�̉��Z�ʁi���j
#define SCORE_HIGH_AMOUNT 2.0f

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

// �r���i�U���j�̃X�R�A���v�Z
float CEnemyContext::ScoreCast(const EnemyContext& context)
{
    float score = 0.0f;

    // HP�c�ʂ��������
    if (context.hpRatio > HIGH_HP_THRESHOLD)
        score += SCORE_AMOUNT;          // �㏸

    // HP�c�ʂ��v���C���[��葽�����
    if (context.hpRatio > context.hpRatioP)
        score += SCORE_AMOUNT;          // �㏸
    // �v���C���[��菭�Ȃ���Εs���Ȃ̂�
    else
        score -= SCORE_AMOUNT;  // �ቺ

    // MP�c�ʂ��������
    if (context.mpRatio > HIGH_MP_THRESHOLD)
        score += SCORE_HIGH_AMOUNT;  // �㏸
    // MP�c�ʂ����Ȃ����
    else if (context.mpRatio < LOW_MP_THRESHOLD)
        return 0.0f;  // �r���͂��Ȃ�

    // �v���C���[��MP�c�ʂ����Ȃ����
    if (context.mpRatioP < LOW_MP_THRESHOLD)
        score += SCORE_AMOUNT;          // �㏸
    
    // �v���C���[���߂����
    if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;          // �㏸
    // �����Ȃ瓖���肸�炢�̂�
    else if (context.distanceToPlayer > FAR_PLAYER_DISTANCE)
        score -= SCORE_AMOUNT;          // �ቺ
    
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
    // �c�ʂ����Ȃ��Ȃ�
    else if (context.mpRatio < LOW_MP_THRESHOLD)
        return 0.0f;    // �ǐՂ͂��Ȃ�

    // �v���C���[��MP�c�ʂ����Ȃ����
    if (context.mpRatioP < LOW_MP_THRESHOLD)
        score += SCORE_AMOUNT;      // �㏸

    // �v���C���[�������Ȃ�
    if (context.distanceToPlayer > FAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // �㏸
    // �߂��Ȃ炳��ɋ߂Â��K�v���Ȃ��̂�
    else if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
        score -= SCORE_AMOUNT;  // �ቺ

    // �v���C���[���r�����Ă��Ȃ����
    if (!context.isPlayerCasting)
        score += SCORE_AMOUNT;  // �㏸
    // �r�����Ă���Ȃ��Ȃ��̂�
    else
        score -= SCORE_AMOUNT;  // �ቺ

    // ���������ł��Ă��Ȃ��Ȃ�
    if (!context.isSpellComing)
        score += SCORE_AMOUNT;  // �㏸
    // ���������ł��Ă���Ȃ��Ȃ��̂�
    else
        score -= SCORE_HIGH_AMOUNT; // �ቺ

    // �u���X�������r�����Ă�����
    if (context.castShape == ESpellShapeType::eBreath)
        score += SCORE_HIGH_AMOUNT; // �㏸

    return score;
}

// ����̃X�R�A���v�Z
float CEnemyContext::ScoreDodge(const EnemyContext& context)
{
    float score = 0.0f;
    // HP�c�ʂ����Ȃ����
    if (context.hpRatio < LOW_HP_THRESHOLD)
        score += SCORE_HIGH_AMOUNT;  // �㏸

    // MP�c�ʂ����Ȃ����
    if (context.mpRatio < LOW_MP_THRESHOLD)
        score += SCORE_HIGH_AMOUNT;  // �㏸
    // ������Ύ����ŉ���ł���̂Œʏ핪�㏸
    else if (context.mpRatio > HIGH_MP_THRESHOLD)
        score += SCORE_AMOUNT;  // �㏸

    // �v���C���[���HP�����Ȃ��Ȃ�s���Ȃ̂�
    if (context.hpRatio < context.hpRatioP)
        score += SCORE_AMOUNT;  // �㏸

    // �v���C���[���߂����
    if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // �㏸

    // �v���C���[���r�����Ă����
    if (context.isPlayerCasting)
        score += SCORE_HIGH_AMOUNT;  // �㏸
    
    // �V�[���h��\���Ă��Ȃ��Ȃ�
    if (!context.isShield)
    {
        score += SCORE_HIGH_AMOUNT;  // �㏸
    }
    // �\���Ă���Ȃ�
    else
        score -= SCORE_HIGH_AMOUNT;  // �ቺ

    // ���������ł��Ă���Ȃ�
    if (context.isSpellComing)
    {
        // ���Z�ʂ̒Ⴉ�獂�܂ł͈̔͂Ɏ��߂�
        float value = Math::Clamp(context.comingSpellScore, SCORE_LOW_AMOUNT, SCORE_HIGH_AMOUNT);
        score += value;  // �㏸
    }
    // ���������ł��Ă��Ȃ��Ȃ�
    else
    {
        score = 0.0f;
    }

    return score;
}

// �����̃X�R�A���v�Z
float CEnemyContext::ScoreRun(const EnemyContext& context)
{
    float score = 0.0f;
    // HP�c�ʂ����Ȃ����
    if (context.hpRatio < LOW_HP_THRESHOLD)
        score += SCORE_AMOUNT;  // �㏸
    
    // MP�c�ʂ����Ȃ����
    if (context.mpRatio < LOW_MP_THRESHOLD)
        score += SCORE_HIGH_AMOUNT;  // �㏸

    // �v���C���[�������Ȃ����
    if (context.distanceToPlayer < FAR_PLAYER_DISTANCE)
        score += SCORE_AMOUNT;  // �㏸
    // �v���C���[���������
    else
        return 0.0f;    // �������Ȃ�

    // �v���C���[���r�����Ă����
    if (context.isPlayerCasting)
        score += SCORE_AMOUNT;  // �㏸

    // ���������ł��Ă���Ȃ�
    if (context.isSpellComing)
    {
        // ���Z�ʂ̒Ⴉ�獂�܂ł͈̔͂Ɏ��߂�
        float value = Math::Clamp(context.comingSpellScore, SCORE_LOW_AMOUNT, SCORE_HIGH_AMOUNT);
        score += value;  // �㏸
    }

    return score;
}

// �g���U�����������肷��
ESpellShapeType CEnemyContext::ScoreAtkSpell(const EnemyContext& context)
{
    // ���ꂼ��̃X�R�A
    float scoreBall = 0.0f;
    float scoreBolt = 0.0f;
    float scoreBreath = 0.0f;
    // ��ԍ����X�R�A
    float bestScore = 0.0f;
    ESpellShapeType bestShape = ESpellShapeType::eError;

    // �v���C���[���߂��Ȃ�
    if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
        scoreBreath += SCORE_AMOUNT;    // �u���X�㏸
    // �v���C���[�������Ȃ�
    else if (context.distanceToPlayer > FAR_PLAYER_DISTANCE)
        scoreBolt += SCORE_AMOUNT;      // �{���g�㏸
    // ����ȊO
    else
        scoreBall += SCORE_AMOUNT;      // �{�[���㏸

    // �v���C���[���r�����Ă����瓖�Ă�ꂻ���Ȃ̂�
    if (context.isPlayerCasting)
        scoreBolt += SCORE_HIGH_AMOUNT;  // �{���g�㏸

    // ��ԍ����X�R�A���{�[���̃X�R�A�ɐݒ�
    bestScore = scoreBall;
    bestShape = ESpellShapeType::eBall;

    // �{���g�̃X�R�A�̕���������ΐݒ�
    if (scoreBolt > bestScore)
    {
        bestScore = scoreBolt;
        bestShape = ESpellShapeType::eBolt;
    }
    // �u���X�̃X�R�A�̕���������ΐݒ�
    if (scoreBreath > bestScore)
    {
        bestScore = scoreBreath;
        bestShape = ESpellShapeType::eBreath;
    }

    return bestShape;
}

// �g���T�|�[�g���������肷��
ESpellShapeType CEnemyContext::ScoreSpSpell(const EnemyContext& context)
{
    float scoreTeleport = 0.0f;
    float scoreShield = 0.0f;
    float scoreReflector = 0.0f;
    // ��ԍ����X�R�A
    float bestScore = 0.0f;
    ESpellShapeType bestShape = ESpellShapeType::eError;

    // ���ł��Ă���̂��u���X�Ȃ�
    if (context.comingShape == ESpellShapeType::eBreath)
    {
        scoreTeleport += SCORE_AMOUNT;  // �e���|�[�g�㏸
    }
    // �{���g�Ȃ�
    else if (context.comingShape == ESpellShapeType::eBolt)
    {
        scoreReflector += SCORE_AMOUNT;     // ���t���N�^�[�㏸
    }
    // �{�[���Ȃ�
    else if (context.comingShape == ESpellShapeType::eBall)
    {
        scoreShield += SCORE_AMOUNT;    // �V�[���h�㏸
    }

    // �v���C���[���߂��Ȃ�
    if (context.distanceToPlayer < NEAR_PLAYER_DISTANCE)
    {
        scoreTeleport += SCORE_AMOUNT;  // �e���|�[�g�㏸
    }
    // �v���C���[�������Ȃ�
    else if (context.distanceToPlayer > FAR_PLAYER_DISTANCE)
    {
        scoreShield += SCORE_AMOUNT;    // �V�[���h�㏸
    }
    // ����ȊO
    else
    {
        scoreReflector += SCORE_AMOUNT; // ���t���N�^�[�㏸
    }

    // ���ɃV�[���h�𒣂��Ă���Ȃ�
    if (context.isShield)
    {
        scoreShield = -1.0f;     // ����Ȃ����Ȃ�
    }

    // ��ԍ����X�R�A���V�[���h�̃X�R�A�ɐݒ�
    bestScore = scoreShield;
    bestShape = ESpellShapeType::eShield;

    // �e���|�[�g�̃X�R�A�̕���������ΐݒ�
    if (scoreTeleport > bestScore)
    {
        bestScore = scoreTeleport;
        bestShape = ESpellShapeType::eTeleport;
    }
    // ���t���N�^�[�̃X�R�A�̕���������ΐݒ�
    if (scoreReflector > bestScore)
    {
        bestScore = scoreReflector;
        bestShape = ESpellShapeType::eReflector;
    }

    return bestShape;
}

// �߂����ǂ���
bool CEnemyContext::IsNear(float dist)
{
    return dist < NEAR_PLAYER_DISTANCE ? true : false;
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
