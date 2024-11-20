#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CSound.h"

/*
プレイヤークラス
キャラクタクラスを継承
*/
class CTrashPlayer : public CXCharacter
{
public:
	// インスタンスのポインタの取得
	static CTrashPlayer* Instance();

	// コンストラクタ
	CTrashPlayer();
	// デストラクタ
	~CTrashPlayer();

	// 更新
	void Update();
	// 描画
	void Render();

protected:
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
	// クリティカル攻撃開始
	void UpdateCriticalStart();
	// クリティカル攻撃中
	void UpdateCritical();
	// クリティカル攻撃終了
	void UpdateCriticalEnd();
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
		eIdleClose,			// 待機					（と）
		eIdleOpen,			// 待機					（開）
		eMoveClose,			// 移動					（と）
		eMoveOpen,			// 移動					（開）
		eDamageStartClose,	// 被弾開始				（と）
		eDamageStartOpen,	// 被弾開始				（開）
		eDamageClose,		// 被弾ノックバック		（と）
		eDamageOpen,		// 被弾ノックバック		（開）
		eDamageEndClose,	// 被弾終了				（と）
		eDamageEndOpen,		// 被弾終了				（開）
		eJumpStartClose,	// ジャンプ開始			（と）
		eJumpStartOpen,		// ジャンプ開始			（開）
		eJumpClose,			// ジャンプ				（と）
		eJumpOpen,			// ジャンプ				（開）
		eJumpEndClose,		// ジャンプ終了			（と）
		eJumpEndOpen,		// ジャンプ終了			（開）
		eAttackStartClose,	// 攻撃開始				（と）
		eAttackStartOpen,	// 攻撃開始				（開）
		eAttack,			// 攻撃					（開閉）
		eAttackEnd,			// 攻撃終了				（開閉）
		eCriticalStartClose,// クリティカル攻撃開始	（と）
		eCriticalStartOpen,	// クリティカル攻撃開始	（開）
		eCritical,			// クリティカル攻撃		（開閉）
		eCriticalEnd,		// クリティカル攻撃終了	（開閉）


		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type);

	// プレイヤーのインスタンス
	static CTrashPlayer* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

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
	};
	EState mState;

	// 蓋が開いているか
	bool IsOpen;

	int mTest;
};
