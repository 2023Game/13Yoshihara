#pragma once
//プレイヤー基底クラスのインクルード
#include "CPlayerBase.h"
// プレイヤーのステータスクラスのインクルード
#include "CPlayerStatus.h"
#include "CCastSpellStr.h"

class CSound;
class CImage;
class CCastSpellStr;

/*
ゲームのプレイヤークラス
プレイヤー基底クラスと
プレイヤーのステータスクラスと
呪文詠唱クラスを継承
*/
class CPlayer : public CPlayerBase ,  public CPlayerStatus, public CCastSpellStr
{
public:
	// コンストラクタ
	CPlayer();
	// デストラクタ
	~CPlayer();

	// 更新
	void Update();

	// プレイヤーが詠唱状態か
	bool IsCastState() const;

private:
	// コライダ―を生成
	void CreateCol() override;
	/*
	アクションのキー入力
	回収員がついていないときのみ入力可能
	*/
	void ActionInput();
	// 詠唱のキー入力
	void CastInput();
	// 基本詠唱のキー入力
	void BasicCastInput();
	// 短縮詠唱のキー入力
	void QuickCastInput();

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
		eDamageStart,	// 被弾開始
		eDamage,		// 被弾ノックバック
		eDamageEnd,		// 被弾終了
		eCast,			// 詠唱中
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

	// MPゲージ
	CGaugeUI2D* mpMpGauge;
};
