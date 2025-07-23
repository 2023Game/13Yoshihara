#pragma once
#include "CSwitchObject.h"

// スイッチで動く床
class CSwitchMoveFloor : public CSwitchObject
{
public:
	// コンストラクタ
	CSwitchMoveFloor(CModel* model, CModel* colModel,
		CVector& pos,
		CVector& move,
		float moveTime);
	// デストラクタ
	~CSwitchMoveFloor();

private:
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