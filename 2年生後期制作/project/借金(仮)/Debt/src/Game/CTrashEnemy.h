#pragma once
// 敵基底クラスのインクルード
#include "CEnemyBase.h"
// 敵のステータスクラスのインクルード
#include "CTrashEnemyStatus.h"

/*
ゴミ拾いゲームの敵クラス
敵基底クラスと
敵のステータスクラスを継承
*/
class CTrashEnemy : public CEnemyBase, public CTrashEnemyStatus
{
public:
	// コンストラクタ
	CTrashEnemy();
	~CTrashEnemy();

	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render() override;
private:
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
		eIdle_Close,		// 待機					（と）
		eIdle_Open,			// 待機					（開）
		eMove_Close,		// 移動					（と）
		eMove_Open,			// 移動					（開）
		eDamage_Close_Start,// 被弾開始				（と）
		eDamage_Close,		// 被弾ノックバック		（と）
		eDamage_Close_End,	// 被弾終了				（と）
		eDamage_Open_Start,	// 被弾開始				（開）
		eDamage_Open,		// 被弾ノックバック		（開）
		eDamage_Open_End,	// 被弾終了				（開）
		eJump_Close_Start,	// ジャンプ開始			（と）
		eJump_Close,		// ジャンプ中			（と）
		eJump_Close_End,	// ジャンプ終了			（と）
		eJump_Open_Start,	// ジャンプ開始			（開）
		eJump_Open,			// ジャンプ中			（開）
		eJump_Open_End,		// ジャンプ終了			（開）
		eAttack_Start,		// 攻撃開始				（開）
		eAttack,			// 攻撃中				（開閉）
		eAttack_End,		// 攻撃終了				（開閉）
		eCritical_Start,	// クリティカル攻撃開始	（開）
		eCritical,			// クリティカル攻撃中	（開閉）
		eCritical_End,		// クリティカル攻撃終了	（開閉）
		eOpen,				// 蓋を開く				（と）
		eClose,				// 蓋を閉じる			（開）


		Num
	};

	// 敵の状態
	enum class EState
	{
		eIdle,			// 待機
		ePatrol,		// 巡回
		eChase,			// 追跡
		eLost,			// 見失う
		eDamageStart,	// 被弾開始
		eDamage,		// 被弾ノックバック
		eDamageEnd,		// 被弾終了
		eJumpStart,		// ジャンプ開始
		eJump,			// ジャンプ
		eJumpEnd,		// ジャンプ終了
		eAttackStart,	// 攻撃開始
		eAttack,		// 攻撃
		eAttackEnd,		// 攻撃終了
		eCriticalStart,	// クリティカル攻撃開始
		eCritical,		// クリティカル攻撃
		eCriticalEnd,	// クリティカル攻撃終了
		eOpenClose,		// 蓋を開閉する
	};

	// 待機状態
	void UpdateIdle();
	// 巡回処理
	void UpdatePatrol();
	// 追跡処理
	void UpdateChase();
	// 見失う処理
	void UpdateLost();
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
	// クリティカル攻撃開始
	void UpdateCriticalStart();
	// クリティカル攻撃中
	void UpdateCritical();
	// クリティカル攻撃終了
	void UpdateCriticalEnd();
	// 蓋を開閉する
	void UpdateOpenClose();

	// 状態切り替え
	void ChangeState(EState state);
	EState mState;	// 敵の状態
	int mStateStep;	// 状態内のステップ数
	float mElapsedTime;	// 経過時間

#if _DEBUG
	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;
	// 状態の色を取得
	CColor GetStateColor(EState state) const;
#endif

	// 蓋が開いているか
	bool mIsOpen;
	// ジャンプしているか
	bool mIsJump;
};
