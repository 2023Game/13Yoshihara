#pragma once
#include "CPlayerBase.h"

/*
拠点のプレイヤーキャラクター
プレイヤーベースを継承
*/
class CHomePlayer : public CPlayerBase
{
public:
	CHomePlayer();
	~CHomePlayer();

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
	
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eMove,		// 移動


		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	enum class EState
	{
		eIdle,			// 待機
	};
	EState mState;

	CColliderLine* mpColliderLine;
}; 
