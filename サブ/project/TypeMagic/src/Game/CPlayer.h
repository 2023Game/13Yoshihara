#pragma once
//プレイヤー基底クラスのインクルード
#include "CPlayerBase.h"
// プレイヤーのステータスクラスのインクルード
#include "CPlayerStatus.h"
#include "CSpellCaster.h"

class CSound;
class CImage;

/*
ゲームのプレイヤークラス
プレイヤー基底クラスと
プレイヤーのステータスクラスと
呪文詠唱クラスを継承
*/
class CPlayer : public CPlayerBase ,  public CPlayerStatus, public CSpellCaster
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
	// コライダ―を生成
	void CreateCol() override;
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

	// 攻撃中か
	bool mIsAttacking;
};
