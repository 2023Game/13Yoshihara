#pragma once
#include "CSwitchObject.h"
#include "MoveState.h"

class CAirConnectObj;

// スイッチオンで動く空中オブジェクト
class CSwitchMoveAirObj : public CSwitchObject
{
public:
	// 状態を設定
	void SetState(EMoveState state);
	// 状態を取得
	EMoveState GetState() const;
	// 経過時間を設定
	void SetElapsedTime(float time);
	// 経過時間を取得
	float GetElapsedTime() const;

	// コンストラクタ
	CSwitchMoveAirObj(
		const CVector& pos,
		std::vector<CVector> movePos);
	// デストラクタ
	~CSwitchMoveAirObj();

	CAirConnectObj* GetAirConnectObj();

private:
	// 作用していないときの処理
	void UpdateOff() override;
	// 作用しているときの処理
	void UpdateOn() override;

	// 移動の処理
	void Move();

	// 移動する座標のリスト
	std::vector<CVector> mTargetPos;
	// 目標座標の番号
	int mTargetPosNum;


	// 状態を変更
	void ChangeMoveState(EMoveState state);
	EMoveState mMoveState;	// 移動状態
	float mElapsedTime;	// 経過時間

	CAirConnectObj* mpConnectObj;
};