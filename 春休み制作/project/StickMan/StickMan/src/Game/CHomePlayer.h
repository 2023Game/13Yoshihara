#pragma once
#include "CPlayerBase.h"

/*
拠点のプレイヤーキャラクター
プレイヤーベースを継承
*/
class CHomePlayer : public CPlayerBase
{
public:
	// コンストラクタ
	CHomePlayer();
	// デストラクタ
	~CHomePlayer();

	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	// 待機状態
	void UpdateIdle();
	// 移動処理
	void UpdateMove();

	// アクションのキー入力
	void ActionInput();
	
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eMove,		// 移動


		Num
	};

	enum class EState
	{
		eIdle,			// 待機
	};
	EState mState;
}; 
