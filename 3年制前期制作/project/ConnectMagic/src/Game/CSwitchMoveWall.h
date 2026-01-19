#pragma once
#include "CSwitchObject.h"
#include "MoveState.h"

// スイッチで動く壁
class CSwitchMoveWall : public CSwitchObject
{
public:
	// CSavableの純粋仮想関数のオーバーライド
	std::vector<char> SaveState() const override;
	void LoadState(const std::vector<char>& data) override;
	size_t GetTypeID() const override;
	unsigned int GetUniqueInstanceID() const override;

	// コンストラクタ
	CSwitchMoveWall(CModel* model, CModel* col,
		const CVector& pos,
		const CVector& scale,
		const CVector& move,
		float moveTime);
	// デストラクタ
	~CSwitchMoveWall();

	// 開いているか
	bool IsOpen() const;
	// 開いているかを設定
	void SetIsOpen(bool enable);

private:
	// データ保存で使用
	unsigned int mUniqueID;
	// 状態を設定
	void SetMoveState(EMoveState state);
	// 状態を取得
	EMoveState GetMoveState() const;
	// 経過時間を設定
	void SetElapsedTime(float time);
	// 経過時間を取得
	float GetElapsedTime() const;

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

	// 状態を変更
	void ChangeMoveState(EMoveState state);
	EMoveState mMoveState;		// 移動状態
	float mElapsedTime;		// 経過時間

	// 開いているか
	bool mIsOpen;
};