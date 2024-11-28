#pragma once
#include "CJobStatusBase.h"

// ゴミ拾いゲームで使用するステータス一覧
class CTrashStatus : public CJobStatusBase
{
private:
	// プレイヤーのステータス
	struct PlayerStatus
	{
		int mMaxHp;			// 最大HP
		float mMoveSpeed;	// 移動速度
		float mJumpSpeed;	// ジャンプ速度
		float mKnockBack;	// ノックバック速度
		int mPower;			// ゴミを落とす力
		float mPointRatio;	// 得点倍率
	};
	const PlayerStatus mPlayerStatus;

	// 敵のステータス
	struct EnemyStatus
	{
		int mMaxHp;			// 最大HP
		float mMoveSpeed;	// 移動速度
		float mJumpSpeed;	// ジャンプ速度
		float mKnockBack;	// ノックバック速度
		int mPower;			// ゴミを落とす力
	};
	const EnemyStatus mEnemyStatus;

	// 車のステータス
	struct CarStatus
	{
		float mMoveSpeed;	// 移動速度
		float mPopTime;		// 出現までの時間
		float mKnockBack;	// ノックバック速度
		int mPower;			// ゴミを落とす力
		float mDeleteTime;	// 消滅までの時間
	};
	const CarStatus mCarStatus;

	// ゴミ収集車のステータス
	struct TruckStatus
	{
		int mMaxHp;				// 最大HP
		float mMoveSpeed;		// 移動速度
		float mPopTime;			// 出現までの時間
		float mBackTime;		// 撤収までの時間
		float mCollectRadius;	// 回収範囲の半径 
		int mCollectors;		// 回収員の召喚人数
	};
	const TruckStatus mTruckStatus;

	// 回収員のステータス
	struct CollectorStatus
	{
		int mMaxHp;			// 最大HP
		float mMoveSpeed;	// 移動速度
	};
	const CollectorStatus mCollectorStatus;

	// ゴミ収集車（お仕置き）のステータス
	struct PunishTruckStatus
	{
		float mMoveSpeed;	// 移動速度
		float mPunishTime;	// お仕置きまでの時間
		float mBackTime;	// 撤収までの時間
		float mCollectors;	// 回収員の召喚人数
	};
	const PunishTruckStatus mPunishTruckStatus;

	// 回収員（お仕置き）のステータス
	struct PunishCollectorStatus
	{
		int mMaxHp;			// 最大HP
		float mMoveSpeed;	// 移動速度
	};
	const PunishCollectorStatus mPunishCollectorStatus;

	// ゴミ袋のステータス
	struct TrashBagStatus
	{
		float mPoint;		// 得点
		float mDeleteTime;	// 消滅までの時間
	};
	const TrashBagStatus mTrashBagStatus;

	// ゴールデンゴミ袋のステータス
	struct GoldenTrashBagStatus
	{
		float mPoint;		// 得点
		float mDeleteTime;	// 消滅までの時間
	};
	const GoldenTrashBagStatus mGoldenTrashBagStatus;

	// 住人
	struct ResidentStatus
	{
		float mMoveSpeed;	// 移動速度
		int mMaxTrash;		// ゴミの最大数
		float mthrowSpeed;	// 投げる速度
	};
	const ResidentStatus mResidentStatus;


public:
	CTrashStatus();
	~CTrashStatus();

	// プレイヤーのステータスを取得
	PlayerStatus GetPlayerStatus();

	// 敵のステータスを取得
	EnemyStatus GetEnemyStatus();

	// 車のステータスを取得
	CarStatus GetCarStatus();

	// ゴミ収集車のステータスを取得
	TruckStatus GetTruckStatus();

	// 回収員のステータスを取得
	CollectorStatus GetCollectorStatus();

	// ゴミ収集車（お仕置き）のステータスを取得
	PunishTruckStatus GetPunishTruckStatus();
	
	// 回収員（お仕置き）のステータスを取得
	PunishCollectorStatus GetPunishCollectorStatus();
	
	// ゴミ袋のステータスを取得
	TrashBagStatus GetTrashBagStatus();
	
	// ゴールデンゴミ袋のステータスを取得
	GoldenTrashBagStatus GetGoldenTrashBagStatus();
	
	// 住人のステータスを取得
	ResidentStatus GetResidentStatus();
};