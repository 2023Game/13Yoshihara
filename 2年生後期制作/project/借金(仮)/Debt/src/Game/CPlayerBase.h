#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CSound.h"

class CFlamethrower;

/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayerBase : public CXCharacter
{
public:
	// インスタンスのポインタの取得
	static CPlayerBase* Instance();

	// コンストラクタ
	CPlayerBase(float capsuleRadius, float playerHeight);
	// デストラクタ
	~CPlayerBase();

	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	virtual void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

	// 描画
	void Render();

protected:
	// アニメーション切り替え
	void ChangeAnimation(int type, bool restart = false);

	// キーの入力情報から移動ベクトルを求める
	CVector CalcMoveVec();

	// モーションブラーの更新処理
	void UpdateMotionBlur();

	// プレイヤーのインスタンス
	static CPlayerBase* spInstance;

	CVector mMoveSpeed;	// 前後左右の移動速度
	float mMoveSpeedY;	// 重力やジャンプによる上下の移動速度

	bool mIsGrounded;	// 接地しているかどうか
	bool mIsWall;		// 壁に接触しているかどうか

	CVector mGroundNormal;	// 接地している地面の法線
	CVector mWallNormal;	// 壁に接触している時の法線

	CColliderCapsule* mpColliderCapsule;

	CTransform* mpRideObject;

	// モーションブラーを掛ける残り時間
	float mMotionBlurRemainTime;

	// 攻撃を受けているか
	bool mIsDamage;
};
