#pragma once
#include "CPlayerBase.h"
#include "CPlayerStatus.h"

class CSound;
class CWand;
class CConnectPoint;
class CConnectObject;
class CConnectTarget;
class CImage;
struct PlayerData;

/*
ゲームのプレイヤークラス
プレイヤー基底クラスと
プレイヤーのステータスクラスを継承
*/
class CPlayer : public CPlayerBase ,  public CPlayerStatus
{
public:
	// セーブ・ロード処理
	PlayerData SaveState();
	void LoadState(const PlayerData& data);

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
		eEdgeJumpStart,	// エッジジャンプの開始
		eEdgeJump,		// エッジジャンプ中
		eEdgeJumpEnd,	// エッジジャンプの終了
		eDeath,			// 死亡
	};

	// コンストラクタ
	CPlayer(const CVector& pos);
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

	// 杖のポインタを取得
	CWand* GetWand();

	// 状態切り替え
	void ChangeState(EState state);

	// 衝突処理
	void OnCollision(const CollisionData& data) override;
	// センサーの接触処理
	void OnSensorEnter(const CollisionData& data) override;

private:
	// コライダ―を生成
	void CreateCol() override;

	// 射程内にあるコネクトオブジェクトのリスト
	std::list<CConnectObject*> mConnectObjs;

	// 地面に接触しているかを判断して設定
	void CheckGrounded();
	/*
	アクションのキー入力
	回収員がついていないときのみ入力可能
	*/
	void ActionInput();


	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,				// Tポーズ

		eIdle,				// 待機
		eIdle_Wand,			// 杖持ち待機
		eMove,				// 移動
		eMove_Wand,			// 杖持ち移動
		eJump_Start,		// ジャンプ開始
		eJump,				// ジャンプ中
		eJump_End,			// ジャンプ終了
		eAttack,			// 攻撃中
		eSwing,				// スイング
		eSwing_End_Start,	// スイング終了の開始
		eSwing_End,			// スイング終了中
		eSwing_End_End,		// スイング終了の終了
		eDeath,		// 死亡


		Num
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
	// エッジジャンプの開始
	void UpdateEdgeJumpStart();
	// エッジジャンプ中
	void UpdateEdgeJump();
	// エッジジャンプの終了
	void UpdateEdgeJumpEnd();
	// 死亡の更新処理
	void UpdateDeath();
	// 死亡
	void Death() override;

	EState mState;	// プレイヤーの状態
	int mStateStep;				// 状態内のステップ管理用
	float mElapsedTime;			// 経過時間計測用

#if _DEBUG
	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;
#endif

	// 1フレーム前の接地状態
	bool mWasGrounded;
	// ジャンプしているか
	bool mIsJump;
	// 杖を持っているか
	bool mIsWand;
	// 攻撃中か
	bool mIsAttacking;

	CWand* mpWand;			// 杖
	CConnectPoint* mpWandPoint;	// 接続部

	// 視点の中心に一番近いターゲットを求める
	void CenterTarget();
	// 視点の中心に一番近いターゲット
	CConnectTarget* mpCenterTarget;

	// ターゲット中の場所の画像
	CImage* mpTargetPointImg;
	// ターゲット中の画像の位置や有効無効の更新
	void UpdatePointImg();

	// ターザン中の移動速度
	CVector mTarzanMoveSpeed;
};
