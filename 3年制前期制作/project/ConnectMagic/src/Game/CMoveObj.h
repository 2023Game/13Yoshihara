#pragma once
#include "CRideableObject.h"
#include "CModel.h"
#include "MoveState.h"
#include "CSavable.h"
#include "CollisionLayer.h"

class CMoveObj : public CRideableObject, public CSavable
{
public:
	// CSavableの純粋仮想関数のオーバーライド
	std::vector<char> SaveState() const override;
	void LoadState(const std::vector<char>& data) override;
	size_t GetTypeID() const override;
	unsigned int GetUniqueInstanceID() const override;

	// 状態を設定
	void SetMoveState(EMoveState state);
	// 状態を取得
	EMoveState GetMoveState() const;
	// 前回の状態を設定
	void SetPreMoveState(EMoveState state);
	// 前回の状態を取得
	EMoveState GetPreMoveState() const;
	// 経過時間を設定
	void SetElapsedTime(float time);
	// 経過時間を取得
	float GetElapsedTime() const;

	CMoveObj(CModel* model, CModel* col,
		const CVector& pos,
		const CVector& scale,
		const CVector& move,
		float moveTime,
		ELayer layer);
	~CMoveObj();

	void Update();
	void Render();

private:
	void CreateCol() override;

	// データ保存に使用
	unsigned int mUniqueID;

	CModel* mpModel;

	CVector mDefaultPos;
	CVector mMoveVec;
	float mMoveTime;

	// 状態を変更
	void ChangeMoveState(EMoveState state);
	EMoveState mMoveState;		// 状態
	EMoveState mPreMoveState;	// 一つ前の状態
	float mElapsedTime;

	// 止まっているときの更新
	void UpdateStop();
	// 進んでいるときの更新
	void UpdateGo();
	// 戻っているときの更新
	void UpdateBack();
};