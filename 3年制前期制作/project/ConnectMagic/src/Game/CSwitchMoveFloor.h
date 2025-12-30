#pragma once
#include "CSwitchObject.h"
#include "MoveState.h"

// スイッチで動く床
class CSwitchMoveFloor : public CSwitchObject
{
public:
	// CSavableの純粋仮想関数のオーバーライド
	std::vector<char> SaveState() const override;
	void LoadState(const std::vector<char>& data) override;
	size_t GetTypeID() const override;
	unsigned int GetUniqueInstanceID() const override;

	// コンストラクタ
	CSwitchMoveFloor(CModel* model, CModel* col,
		const CVector& pos, 
		const CVector& scale,
		const CVector& move,
		float moveTime, bool isCrushed = false);
	// デストラクタ
	~CSwitchMoveFloor();

private:
	// データ保存に使用
	unsigned int mUniqueID;
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

	// コライダーを生成
	void CreateCol() override;

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