#pragma once
#include "CSwitchObject.h"
#include "MoveState.h"

// スイッチで動く押し出す壁
class CSwitchPushWall : public CSwitchObject
{
public:
	// 状態を設定
	void SetState(EMoveState state);
	// 状態を取得
	EMoveState GetState() const;
	// 経過時間を設定
	void SetElapsedTime(float time);
	// 経過時間を取得
	float GetElapsedTime() const;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="col">コライダーモデル</param>
	/// <param name="pos">初期座標</param>
	/// <param name="scale">スケール</param>
	/// <param name="move">移動ベクトル</param>
	/// <param name="moveTime">移動時間</param>
	/// <param name="isCrushed">プレイヤーが挟まれたときに死ぬか</param>
	CSwitchPushWall(CModel* model, CModel* col,
		const CVector& pos,
		const CVector& scale,
		const CVector& move,
		float moveTime, bool isCrushed = false);
	// デストラクタ
	~CSwitchPushWall();

private:
	// コライダーを生成
	void CreateCol() override;
	// プレイヤーが挟まれた時用のコライダー
	CColliderMesh* mpCrushedCol;

	// 作用していない時の処理
	void UpdateOff() override;
	// 作用している時の処理
	void UpdateOn() override;

	// 止まっているときの更新
	void UpdateStop();
	// 進んでいるときの更新
	void UpdateGo();
	// 戻っているときの更新
	void UpdateBack();

	CVector mDefaultPos;	// 初期座標
	CVector mMoveVec;		// 移動ベクトル
	float mMoveTime;		// 移動時間

	// 状態を変更
	void ChangeMoveState(EMoveState state);
	EMoveState mMoveState;		// 移動状態
	EMoveState mPreMoveState;	// 前回の移動状態
	float mElapsedTime;			// 経過時間
};