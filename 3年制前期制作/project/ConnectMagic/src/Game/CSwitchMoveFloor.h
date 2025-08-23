#pragma once
#include "CSwitchObject.h"
#include "MoveState.h"

// スイッチで動く床
class CSwitchMoveFloor : public CSwitchObject
{
public:
	// 状態を設定
	void SetState(EMoveState state);
	// 状態を取得
	EMoveState GetState() const;
	// 前回の状態を設定
	void SetPreState(EMoveState state);
	// 前回の状態を取得
	EMoveState GetPreState() const;
	// 経過時間を設定
	void SetElapsedTime(float time);
	// 経過時間を取得
	float GetElapsedTime() const;

	// コンストラクタ
	CSwitchMoveFloor(CModel* model, CModel* col,
		const CVector& pos, 
		const CVector& scale,
		const CVector& move,
		float moveTime, bool isCrushed = false);
	// デストラクタ
	~CSwitchMoveFloor();

private:
	// コライダーを生成
	void CreateCol() override;
	// プレイヤーが挟まれた時用のコライダー
	CColliderMesh* mpCrushedCol;

	// 作用している時の処理
	void UpdateOn() override;

	CVector mDefaultPos;	// 初期座標
	CVector mMoveVec;		// 移動ベクトル
	float mMoveTime;		// 移動時間

	// 状態を変更
	void ChangeMoveState(EMoveState state);
	EMoveState mMoveState;		// 移動状態
	EMoveState mPreMoveState;	// 一つ前の移動状態
	float mElapsedTime;

	// 止まっているときの更新
	void UpdateStop();
	// 進んでいるときの更新
	void UpdateGo();
	// 戻っているときの更新
	void UpdateBack();
};