#pragma once
#include "SpellType.h"

class CEnemyStateBase;

// �󋵏�񂩂�X�R�A�]�������߂�N���X
class CEnemyContext
{
public:
	// �C���X�^���X
	static CEnemyContext* Instance();

	// �󋵏��
	struct EnemyContext
	{
		float hpRatio;			// HP�̊���
		float mpRatio;			// MP�̊���
		float hpRatioP;			// �v���C���[��HP�̊���
		float mpRatioP;			// �v���C���[��MP�̊���
		float distanceToPlayer;	// �v���C���[�܂ł̋���
		float distanceToSpell;	// ���ł��Ă�����Ƃ̋���
		bool isPlayerCasting;	// �v���C���[���r�����Ă��邩
		bool isSpellComing;		// ���������ł��Ă��邩
		ESpellShapeType comingShape;	// ���ł��Ă�������̌`
		float comingSpellScore;			// ���ł��Ă�������̃X�R�A
		bool isShield;					// �V�[���h�𒣂��Ă��邩
		ESpellShapeType castShape;		// �r�����̎����̌`
	};

	// �œK�ȍs�����擾
	CEnemyStateBase* GetBestState(const EnemyContext& context);

	// �ҋ@�̃X�R�A���v�Z
	float ScoreIdle(const EnemyContext& context);
	// �r��(�U��)�̃X�R�A���v�Z
	float ScoreCast(const EnemyContext& context);
	// �ǐՂ̃X�R�A���v�Z
	float ScoreChase(const EnemyContext& context);
	// ����̃X�R�A���v�Z
	float ScoreDodge(const EnemyContext& context);
	// �����̃X�R�A���v�Z
	float ScoreRun(const EnemyContext& context);

	// �g���U�����������肷��
	ESpellShapeType ScoreAtkSpell(const EnemyContext& context);
	// �g���T�|�[�g���������肷��
	ESpellShapeType ScoreSpSpell(const EnemyContext& context);

	// �߂����ǂ���
	bool IsNear(float dist);
};