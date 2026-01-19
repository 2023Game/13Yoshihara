#pragma once
#include "CSwitchObject.h"
#include "MoveState.h"

class CAirConnectObj;

// スイッチオンで動く空中オブジェクト
class CSwitchMoveAirObj : public CSwitchObject
{
public:
	// CSavableの純粋仮想関数のオーバーライド
	std::vector<char> SaveState() const override;
	void LoadState(const std::vector<char>& data) override;
	size_t GetTypeID() const override;
	unsigned int GetUniqueInstanceID() const override;

	// 移動状態を設定
	void SetMoveState(EMoveState state);
	// 移動状態を取得
	EMoveState GetMoveState() const;
	// 経過時間を設定
	void SetElapsedTime(float time);
	// 経過時間を取得
	float GetElapsedTime() const;

	// コンストラクタ
	CSwitchMoveAirObj(const CVector& pos);
	// デストラクタ
	~CSwitchMoveAirObj();

	// 空中オブジェクトを取得
	CAirConnectObj* GetAirConnectObj();

	// 有効無効の切り替え
	void SetEnable(bool enable) override;

	// 移動先を追加
	void AddTargetPos(const CVector& pos);

private:
	// データ保存に使用
	unsigned int mUniqueID;

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

	// データ保存時に使用する
	void SetTargetPosNum(int num);
	int GetTargetPosNum() const;
	


	// 状態を変更
	void ChangeMoveState(EMoveState state);
	EMoveState mMoveState;	// 移動状態
	float mElapsedTime;	// 経過時間

	CAirConnectObj* mpConnectObj;
};