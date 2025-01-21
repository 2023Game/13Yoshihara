#pragma once
#include "CEnemyBase.h"
#include "CCollectorStatus.h"

/*
ゴミ収集車から生成される
回収員のクラス
敵基底クラスと
*/
class CCollector : public CEnemyBase , public CCollectorStatus
{
public :
	/// <summary>
	///	コンストラクタ
	/// </summary>
	/// <param name="punisher">trueならば、お仕置き用</param>
	/// <param name="owner">回収員の持ち主</param>
	/// <param name="patrolPoints">巡回ポイントのリスト</param>
	CCollector(bool punisher, CObjectBase* owner,
		std::vector<CNavNode*> patrolPoints);
	// デストラクタ
	~CCollector();

	// 攻撃中か
	bool IsAttacking() const override;
	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;
	// 攻撃が成功したか
	bool GetAttackSuccess() const;

	// ダメージを受ける
	void TakeDamage(int damage, CObjectBase* causer) override;
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

	// 描画
	void Render() override;

	// 回収員の有効無効を切り替える
	void SetOnOff(bool setOnOff);

	// 撤収状態に変える
	void ChangeStateReturn();

	// 回収員の持ち主を設定
	void SetOwner(CObjectBase* owner);
	// 回収員の持ち主を取得
	CObjectBase* GetOwner() const;
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

		eTPose,					// Tポーズ
		eIdle,					// 待機				（無）
		eIdle_Bag,				// 待機				（有）
		eMove,					// 移動				（無）
		eMove_Bag,				// 移動				（有）
		eGetBag,				// ゴミ袋獲得		（無）
		eGetBag_End,			// ゴミ袋獲得終了	（有）
		eDeath,					// 死亡				（有無）
		eAttack_Start,			// 攻撃開始			（無）
		eAttack_True,			// 攻撃成功			（無）
		eAttack_False,			// 攻撃失敗			（無）
		eAttack_False_Fall,		// 攻撃失敗落下中	（無）
		eAttack_False_StandUp,	// 立ち上がる		（無）


		Num
	};

	// 敵の状態
	enum class EState
	{
		eIdle,			// 待機
		ePatrol,		// 巡回
		eChase,			// 追跡
		eLost,			// 見失う
		eReturn,		// ゴミ収集車に戻る
		eAttackStart,	// 攻撃開始
		eAttackTrue,	// 攻撃中（成功）
		eAttackFalse,	// 攻撃中（失敗）
		eAttackEnd,		// 攻撃終了
		eDeath,			// 死亡
	};

	// 待機状態
	void UpdateIdle();
	// 巡回処理
	void UpdatePatrol();
	// 追跡処理
	void UpdateChase();
	// 見失う処理
	void UpdateLost();
	// ゴミ収集車に戻る処理
	void UpdateReturn();
	// 攻撃開始
	void UpdateAttackStart();
	// 攻撃中（成功）
	void UpdateAttackTrue();
	// 攻撃中（失敗）
	void UpdateAttackFalse();
	// 攻撃終了
	void UpdateAttackEnd();
	// 死亡の更新処理
	void UpdateDeath();
	// 死亡処理
	void Death() override;

	// 状態切り替え
	void ChangeState(EState state);
	EState mState;	// 回収員の状態
	int mStateStep;	// 状態内のステップ数
	float mElapsedTime;	// 経過時間

#if _DEBUG
	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;
	// 状態の色を取得
	CColor GetStateColor(EState state) const;
#endif

	bool mIsBag;	// ゴミ袋を持っているかどうか
	// 攻撃が成功したかどうか
	bool mIsAttackSuccess;		
	// 攻撃中かどうか
	bool mIsAttacking;

	// 回収員の持ち主
	CObjectBase* mpOwner;
};