#pragma once
//プレイヤー基底クラスのインクルード
#include "CPlayerBase.h"
// プレイヤーのステータスクラスのインクルード
//#include "CPlayerStatus.h"

class CSound;

/*
ゴミ拾いゲームのプレイヤークラス
プレイヤー基底クラスと
プレイヤーのステータスクラスを継承
*/
class CPlayer : public CPlayerBase , // public CPlayerStatus
{
public:
	// コンストラクタ
	CPlayer();
	// デストラクタ
	~CPlayer();

	// 攻撃中か
	bool IsAttacking() const override;
	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	/// <summary>
	/// ダメージを受ける
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	/// <param name="causer">攻撃してきた相手</param>
	void TakeDamage(int damage, CObjectBase* causer) override;

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
	/*
	アクションのキー入力
	回収員がついていないときのみ入力可能
	*/
	void ActionInput();

	/*
	アニメーションの種類
	(と)＝蓋が閉じている状態からのアニメーション
	(開)＝蓋が開いている状態からのアニメーション
	(開閉)＝両方の状態兼用のアニメーション
	*/
	enum class EAnimType
	{
		None = -1,

		eTPose,				// Tポーズ

		eIdle,		// 待機
		eMove,		// 移動
		eJump,		// ジャンプ

		eAttackW_1,	// 攻撃W
		eAttackA_1,	// 攻撃A
		eAttackS_1,	// 攻撃S
		eAttackD_1,	// 攻撃D

		eDamageW_1,	// 被弾W
		eDamageA_1,	// 被弾A
		eDamageS_1,	// 被弾S
		eDamageD_1,	// 被弾D

		eDodgeW_1,	// 回避W
		eDodgeA_1,	// 回避A
		eDodgeS_1,	// 回避S
		eDodgeD_1,	// 回避D

		eParryW_1,	// パリィW
		eParryA_1,	// パリィA
		eParryS_1,	// パリィS
		eParryD_1,	// パリィD

		eDeath,		// 死亡


		Num
	};

	// プレイヤーの状態
	enum class EState
	{
		eIdle,			// 待機
		eMove,			// 移動
		eDamageStart,	// 被弾開始
		eDamage,		// 被弾ノックバック
		eDamageEnd,		// 被弾終了
		eJumpStart,		// ジャンプ開始
		eJump,			// ジャンプ
		eJumpEnd,		// ジャンプ終了
		eAttackStart,	// 攻撃開始
		eAttack,		// 攻撃
		eAttackEnd,		// 攻撃終了
		eDeath,			// 死亡
	};

	// 待機状態
	void UpdateIdle();
	// 移動処理
	void UpdateMove();
	// 被弾開始
	void UpdateDamageStart();
	// 被弾ノックバック
	void UpdateDamage();
	// 被弾終了
	void UpdateDamageEnd();
	// ジャンプ開始
	void UpdateJumpStart();
	// ジャンプ中
	void UpdateJump();
	// ジャンプ終了
	void UpdateJumpEnd();
	// 攻撃開始
	void UpdateAttackStart();
	// 攻撃中
	void UpdateAttack();
	// 攻撃終了
	void UpdateAttackEnd();
	// 死亡の更新処理
	void UpdateDeath();
	// 死亡
	void Death() override;

	// 状態切り替え
	void ChangeState(EState state);
	EState mState;	// プレイヤーの状態
	int mStateStep;				// 状態内のステップ管理用
	float mElapsedTime;			// 経過時間計測用

#if _DEBUG
	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;
#endif

	// クリティカル攻撃コライダ―
	CCollider* mpCriticalCol;

	// ジャンプしているか
	bool mIsJump;

	// 効果音
	CSound* mpDamageSE;
	CSound* mpCriticalSE;
	CSound* mpGuardSE;
	CSound* mpCollectorDamageSE;
};
