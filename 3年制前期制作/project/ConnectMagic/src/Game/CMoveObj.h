#pragma once
#include "CRideableObject.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CMoveObj : public CRideableObject
{
public:
	CMoveObj(CModel* model, 
		const CVector& pos,
		const CVector& scale,
		const CVector& move,
		float moveTime,
		ELayer layer);
	~CMoveObj();

	void Update();
	void Render();

private:
	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CVector mDefaultPos;
	CVector mMoveVec;
	float mMoveTime;

	enum class EMoveState
	{
		eStop,	// 止まっている
		eGo,	// 進んでいる
		eBack,	// 戻っている
	};
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