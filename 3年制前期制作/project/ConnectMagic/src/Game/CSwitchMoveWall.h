#pragma once
#include "CSwitchObject.h"

// スイッチで動く壁
class CSwitchMoveWall : public CSwitchObject
{
public:
	// コンストラクタ
	CSwitchMoveWall(CModel* model,
		const CVector& pos,
		const CVector& scale,
		const CVector& move,
		float moveTime);
	// デストラクタ
	~CSwitchMoveWall();

private:
	// コライダーを生成
	void CreateCol() override;

	// 作用していない時の処理
	void UpdateOff() override;
	// 作用している時の処理
	void UpdateOn() override;

	// 移動の処理
	void Move();

	CVector mDefaultPos;	// 初期座標
	CVector mMoveVec;		// 移動ベクトル
	float mMoveTime;		// 移動時間
	float mElapsedTime;		// 経過時間
};