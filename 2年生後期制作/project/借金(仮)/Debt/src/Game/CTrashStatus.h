#pragma once
#include "CJobStatusBase.h"

// �S�~�E���Q�[���Ŏg�p����X�e�[�^�X�ꗗ
class CTrashStatus : public CJobStatusBase
{
private:
	// �v���C���[�̃X�e�[�^�X
	struct PlayerStatus
	{
		int mMaxHp;			// �ő�HP
		float mMoveSpeed;	// �ړ����x
		float mJumpSpeed;	// �W�����v���x
		float mKnockBack;	// �m�b�N�o�b�N���x
		int mPower;			// �S�~�𗎂Ƃ���
		float mPointRatio;	// ���_�{��
	};
	const PlayerStatus mPlayerStatus;

	// �G�̃X�e�[�^�X
	struct EnemyStatus
	{
		int mMaxHp;			// �ő�HP
		float mMoveSpeed;	// �ړ����x
		float mJumpSpeed;	// �W�����v���x
		float mKnockBack;	// �m�b�N�o�b�N���x
		int mPower;			// �S�~�𗎂Ƃ���
	};
	const EnemyStatus mEnemyStatus;

	// �Ԃ̃X�e�[�^�X
	struct CarStatus
	{
		float mMoveSpeed;	// �ړ����x
		float mPopTime;		// �o���܂ł̎���
		float mKnockBack;	// �m�b�N�o�b�N���x
		int mPower;			// �S�~�𗎂Ƃ���
		float mDeleteTime;	// ���ł܂ł̎���
	};
	const CarStatus mCarStatus;

	// �S�~���W�Ԃ̃X�e�[�^�X
	struct TruckStatus
	{
		int mMaxHp;				// �ő�HP
		float mMoveSpeed;		// �ړ����x
		float mPopTime;			// �o���܂ł̎���
		float mBackTime;		// �P���܂ł̎���
		float mCollectRadius;	// ����͈͂̔��a 
		int mCollectors;		// ������̏����l��
	};
	const TruckStatus mTruckStatus;

	// ������̃X�e�[�^�X
	struct CollectorStatus
	{
		int mMaxHp;			// �ő�HP
		float mMoveSpeed;	// �ړ����x
	};
	const CollectorStatus mCollectorStatus;

	// �S�~���W�ԁi���d�u���j�̃X�e�[�^�X
	struct PunishTruckStatus
	{
		float mMoveSpeed;	// �ړ����x
		float mPunishTime;	// ���d�u���܂ł̎���
		float mBackTime;	// �P���܂ł̎���
		float mCollectors;	// ������̏����l��
	};
	const PunishTruckStatus mPunishTruckStatus;

	// ������i���d�u���j�̃X�e�[�^�X
	struct PunishCollectorStatus
	{
		int mMaxHp;			// �ő�HP
		float mMoveSpeed;	// �ړ����x
	};
	const PunishCollectorStatus mPunishCollectorStatus;

	// �S�~�܂̃X�e�[�^�X
	struct TrashBagStatus
	{
		float mPoint;		// ���_
		float mDeleteTime;	// ���ł܂ł̎���
	};
	const TrashBagStatus mTrashBagStatus;

	// �S�[���f���S�~�܂̃X�e�[�^�X
	struct GoldenTrashBagStatus
	{
		float mPoint;		// ���_
		float mDeleteTime;	// ���ł܂ł̎���
	};
	const GoldenTrashBagStatus mGoldenTrashBagStatus;

	// �Z�l
	struct ResidentStatus
	{
		float mMoveSpeed;	// �ړ����x
		int mMaxTrash;		// �S�~�̍ő吔
		float mthrowSpeed;	// �����鑬�x
	};
	const ResidentStatus mResidentStatus;


public:
	CTrashStatus();
	~CTrashStatus();

	// �v���C���[�̃X�e�[�^�X���擾
	PlayerStatus GetPlayerStatus();

	// �G�̃X�e�[�^�X���擾
	EnemyStatus GetEnemyStatus();

	// �Ԃ̃X�e�[�^�X���擾
	CarStatus GetCarStatus();

	// �S�~���W�Ԃ̃X�e�[�^�X���擾
	TruckStatus GetTruckStatus();

	// ������̃X�e�[�^�X���擾
	CollectorStatus GetCollectorStatus();

	// �S�~���W�ԁi���d�u���j�̃X�e�[�^�X���擾
	PunishTruckStatus GetPunishTruckStatus();
	
	// ������i���d�u���j�̃X�e�[�^�X���擾
	PunishCollectorStatus GetPunishCollectorStatus();
	
	// �S�~�܂̃X�e�[�^�X���擾
	TrashBagStatus GetTrashBagStatus();
	
	// �S�[���f���S�~�܂̃X�e�[�^�X���擾
	GoldenTrashBagStatus GetGoldenTrashBagStatus();
	
	// �Z�l�̃X�e�[�^�X���擾
	ResidentStatus GetResidentStatus();
};