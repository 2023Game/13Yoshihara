#pragma once
#include "CPlayerBase.h"
#include "CCharaStatusBase.h"

/*
配達ゲームのプレイヤークラス
*/
class CDeliveryPlayer : public CPlayerBase , public CCharaStatusBase
{
public:
	// コンストラクタ
	CDeliveryPlayer();
	// デストラクタ
	~CDeliveryPlayer();

	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);// override;

	// 描画
	void Render();

private:
	// 状態
	enum class EState
	{
		eMove,			// 移動
		eChangeRoad,	// 車線変更
	};
	// 状態切り替え
	void ChangeState(EState state);
	EState mState;	// プレイヤーの状態
	int mStateStep;				// 状態内のステップ管理用
	float mElapsedTime;			// 経過時間計測用

#if _DEBUG
	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;
#endif
	// 移動の更新処理
	void UpdateMove();
	// 車線変更の更新処理
	void UpdateChangeRoad();

	// コライダ―を生成
	void CreateCol();
	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed, float rotateSpeed);
	// アクションのキー入力
	void ActionInput();

	// 3dモデル
	CModel* mpModel;
	// 車線変更の目的地
	CVector mTargetPos;
};