#pragma once
// キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CRideableObject.h"

// 視野範囲のデバッグ表示クラスの前宣言
class CDebugFieldOfView;
class CGaugeUI3D;
/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemyBase : public CXCharacter
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="fovAngle">視野範囲の角度</param>
	/// <param name="fovLength">視野範囲の距離</param>
	CEnemyBase(float fovAngle, float fovLength);
	// デストラクタ
	~CEnemyBase();

	// オブジェクト削除処理
	void DeleteObject(CObjectBase* obj) override;
	// 更新
	void Update() override;
	// 描画
	void Render() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

protected:
	// アニメーション切り替え
	void ChangeAnimation(int type, bool restart = false);

	CVector mMoveSpeed;	// 前後左右の移動速度
	float mMoveSpeedY;	// 重力やジャンプによる上下の移動速度

	bool mIsGrounded;	// 接地しているかどうか
	bool mIsWall;		// 壁に接触しているかどうか

	CVector mGroundNormal;	// 接地している地面の法線
	CVector mWallNormal;	// 壁に接触している時の法線

	CCollider* mpBodyCol;	// 本体のコライダー

	CTransform* mpRideObject;

	// 攻撃を受けているか
	bool mIsDamage;

	float mFovAngle;	// 視野範囲の角度
	float mFovLength;	// 視野範囲の距離
	CDebugFieldOfView* mpDebugFov;	// 視野範囲のデバッグ表示

	CVector mLostPlayerPos;	// プレイヤーを見失った座標

	// オブジェクトが視界範囲内に入ったかどうか
	bool IsFoundObject(CObjectBase* obj) const;
	// プレイヤーが視野範囲内に入ったかどうか
	bool IsFoundPlayer() const;

	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed);
};