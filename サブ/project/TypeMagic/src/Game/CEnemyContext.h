#pragma once

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
		bool isPlayerCasting;	// �v���C���[���r�����Ă��邩
		bool isSpellComing;		// ���������ł��Ă��邩
		float comingSpellScore;	// ���ł��Ă�������̃X�R�A
	};

	// �œK�ȍs�����擾
	CEnemyStateBase* GetBestState(const EnemyContext& context);

	// �ҋ@�̃X�R�A���v�Z
	float ScoreIdle(const EnemyContext& context);
	// �r���̃X�R�A���v�Z
	float ScoreCast(const EnemyContext& context);
	// �ǐՂ̃X�R�A���v�Z
	float ScoreChase(const EnemyContext& context);
	// ����̃X�R�A���v�Z
	float ScoreDodge(const EnemyContext& context);
	// �����̃X�R�A���v�Z
	float ScoreRun(const EnemyContext& context);
};