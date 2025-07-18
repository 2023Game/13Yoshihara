#pragma once
//プレイヤー基底クラスのインクルード
#include "CPlayerBase.h"
// プレイヤーのステータスクラスのインクルード
#include "CPlayerStatus.h"

class CSound;
class CWand;
class CConnectPoint;
class CConnectObject;
class CConnectTarget;
class CImage;

/*
ゲームのプレイヤークラス
プレイヤー基底クラスと
プレイヤーのステータスクラスを継承
*/
class CPlayer : public CPlayerBase ,  public CPlayerStatus
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

	// 杖のポインタを取得
	CWand* GetWand();

	// リスポーン地点を設定
	void SetRespawnPos(CVector respawnPos);

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

		eIdle,		// 待機
		eIdle_Wand,	// 杖持ち待機
		eMove,		// 移動
		eMove_Wand,	// 杖持ち移動
		eJump_Start,// ジャンプ開始
		eJump,		// ジャンプ中
		eJump_End,	// ジャンプ終了
		eAttack,	// 攻撃中
		eSwing,		// スイング
		eSwing_End_Start,	// スイング終了の開始
		eSwing_End,			// スイング終了中
		eSwing_End_End,		// スイング終了の終了

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
		eAttackStart,	// 攻撃開始
		eAttack,		// 攻撃
		eAttackEnd,		// 攻撃終了
		eTarzanStart,	// ターザン開始
		eTarzan,		// ターザン中
		eTarzanEnd,		// ターザン終了
		eReturn,		// 帰還
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
	// ターザン開始
	void UpdateTarzanStart();
	// ターザン中
	void UpdateTarzan();
	// ターザン終了
	void UpdateTarzanEnd();
	// 場外から帰還する処理
	void UpdateReturn();
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

	// ジャンプしているか
	bool mIsJump;
	// 杖を持っているか
	bool mIsWand;
	// 攻撃中か
	bool mIsAttacking;

	CWand* mpWand;			// 杖
	CConnectPoint* mpWandPoint;	// 接続部

	// 射程内にあるコネクトオブジェクトのリスト
	std::list<CConnectObject*> mConnectObjs;
	// コネクトオブジェクトの探知用
	CCollider* mpSearchConnectObjCol;

	// 視点の中心に一番近いターゲットを求める
	void CenterTarget();
	// 視点の中心に一番近いターゲット
	CConnectTarget* mpCenterTarget;

	// ターゲット中の場所の画像
	CImage* mpTargetPointImg;
	// ターゲット中の画像の位置や有効無効の更新
	void UpdatePointImg();

	// リスポーン地点
	CVector mRespawnPos;
};
