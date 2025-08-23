#pragma once
#include "CRideableObject.h"
#include "CModel.h"
#include "CColliderMesh.h"
#include "MoveState.h"

class CMoveObj : public CRideableObject
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

	CMoveObj(CModel* model, CModel* col,
		const CVector& pos,
		const CVector& scale,
		const CVector& move,
		float moveTime,
		ELayer layer, bool isCrushed = false);
	~CMoveObj();

	void Update();
	void Render();

private:
	CModel* mpModel;
	// 本体コライダ―
	CColliderMesh* mpColliderMesh;
	// プレイヤーが挟まれた時用のコライダー
	CColliderMesh* mpCrushedCol;

	CVector mDefaultPos;
	CVector mMoveVec;
	float mMoveTime;

	// 状態を変更
	void ChangeState(EMoveState state);
	EMoveState mState;		// 状態
	EMoveState mPreState;	// 一つ前の状態
	float mElapsedTime;

	// 止まっているときの更新
	void UpdateStop();
	// 進んでいるときの更新
	void UpdateGo();
	// 戻っているときの更新
	void UpdateBack();
};