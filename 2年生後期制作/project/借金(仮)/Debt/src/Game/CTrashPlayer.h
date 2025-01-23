#pragma once
//プレイヤー基底クラスのインクルード
#include "CPlayerBase.h"
// プレイヤーのステータスクラスのインクルード
#include "CTrashPlayerStatus.h"
#include "CCollector.h"

/*
ゴミ拾いゲームのプレイヤークラス
プレイヤー基底クラスと
プレイヤーのステータスクラスを継承
*/
class CTrashPlayer : public CPlayerBase , public CTrashPlayerStatus
{
public:
	// コンストラクタ
	CTrashPlayer();
	// デストラクタ
	~CTrashPlayer();

	// 攻撃中か
	bool IsAttacking() const override;
	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// ダメージを受ける
	void TakeDamage(int damage, CObjectBase* causer) override;
	// クリティカルダメージを受ける
	void TakeCritical(int damage, CObjectBase* causer);

	// 死んでいるかどうか
	bool IsDead() override;

	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 回収員がついているかを取得
	bool GetStickCollector() const;
	// 回収員がついているかを設定
	void SetStickCollector(bool stickCollector);
	// ついている回収員のポインタを取得
	CCollector* GetStickCollectorPointer() const;
	// ついている回収員のポインタを設定
	void SetStickCollectorPointer(CCollector* collector);

	// 開いているかを取得
	bool GetOpen() const;

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
		eIdle_Close,		// 待機					（と）
		eIdle_Open,			// 待機					（開）
		eMove_Close,		// 移動					（と）
		eMove_Open,			// 移動					（開）
		eDamage_Close_Start,// 被弾開始				（と）
		eDamage_Open_Start,	// 被弾開始				（開）
		eDamage,			// 被弾ノックバック		（開閉）
		eDamage_End,		// 被弾終了				（開閉）
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
		eDeath,				// 死亡					（開）


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
		eCriticalStart,	// クリティカル攻撃開始
		eCritical,		// クリティカル攻撃
		eCriticalEnd,	// クリティカル攻撃終了
		eOpenClose,		// 蓋を開閉する
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
	// クリティカル開始
	void UpdateCriticalStart();
	// クリティカル中
	void UpdateCritical();
	// クリティカル終了
	void UpdateCriticalEnd();
	// 蓋を開閉する
	void UpdateOpenClose();
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

	// 蓋が開いているか
	bool mIsOpen;
	// ジャンプしているか
	bool mIsJump;
	// 回収員がついているかどうか
	bool mIsStickCollector;
	// ついている回収員のポインタ
	CCollector* mpStickCollector;
};
