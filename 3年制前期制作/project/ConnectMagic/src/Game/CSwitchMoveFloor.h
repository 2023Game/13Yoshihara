#pragma once
#include "CSwitchObject.h"

// スイッチで動く床
class CSwitchMoveFloor : public CSwitchObject
{
public:
	// コンストラクタ
	CSwitchMoveFloor(CModel* model, CModel* col,
		const CVector& pos, 
		const CVector& scale,
		const CVector& move,
		float moveTime);
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

	enum class EMoveState
	{
		eStop,	// 止まっている
		eGo,	// 進んでいる
		eBack,	// 戻っている
	};
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