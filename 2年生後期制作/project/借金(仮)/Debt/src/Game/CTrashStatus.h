#pragma once
#include "CJobStatusBase.h"

// ゴミ拾いゲームで使用するステータス一覧
class CTrashStatus : public CJobStatusBase
{
public:
	CTrashStatus();
	~CTrashStatus();
private:
	// プレイヤーのステータス
	int mPlayerMaxHp;		// 最大HP
	float mPlayerMoveSpeed;	// 移動速度
	float mPlayerJumpSpeed;	// ジャンプ速度
	float mPlayerKnockBack;	// ノックバック速度
	int mPlayerPower;		// ゴミを落とす力
	float mPlayerPointRatio;// 得点倍率

	// 敵のステータス
	int mEnemyMaxHp;		// 最大HP
	float mEnemyMoveSpeed;	// 移動速度
	float mEnemyJumpSpeed;	// ジャンプ速度
	float mEnemyKnockBack;	// ノックバック速度
	int mEnemyPower;		// ゴミを落とす力

	// 車のステータス
	float mCarMoveSpeed;	// 移動速度
	float mCarKnockBack;	// ノックバック速度
	int mCarPower;			// ゴミを落とす力
	float mCarDeleteTime;	// 消滅までの時間

	// ゴミ収集車のステータス
	int mTruckMaxHp;			// 最大HP
	float mTruckMoveSpeed;		// 移動速度
	float mTruckPopTime;		// 出現までの時間
	float mTruckBackTime;		// 撤収までの時間
	float mTruckCollectRadius;	// 回収範囲の半径 
	int mTruckMens;				// 
	// TODO：ステータスの追加
};