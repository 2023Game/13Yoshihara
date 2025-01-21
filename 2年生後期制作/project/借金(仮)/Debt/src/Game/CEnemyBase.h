#pragma once
// キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CRideableObject.h"

// 視野範囲のデバッグ表示クラスの前宣言
class CDebugFieldOfView;
class CGaugeUI3D;
class CNavNode;
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
	/// <param name="patrolPoints">巡回ポイントのリスト</param>
	/// <param name="eyeHeight">視点の高さ</param>
	CEnemyBase(float fovAngle, float fovLength, 
		std::vector<CVector> patrolPoints,
		float eyeHeight);
	// デストラクタ
	~CEnemyBase();

	// 本体コライダ―を取得
	CCollider* GetBodyCol() const;

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

	float mGravity;	// 重力

	bool mIsGrounded;	// 接地しているかどうか
	bool mIsGravity;	// 重力を掛けるかどうか

	CVector mGroundNormal;	// 接地している地面の法線

	CCollider* mpBodyCol;	// 本体のコライダー
	CCollider* mpAttackCol;	// 攻撃コライダー

	CTransform* mpRideObject;

	float mFovAngle;	// 視野範囲の角度
	float mFovLength;	// 視野範囲の距離
	CDebugFieldOfView* mpDebugFov;	// 視野範囲のデバッグ表示

	float mEyeHeight;	// 視点の高さ

	CNavNode* mpLostPlayerNode;	// プレイヤーを見失った位置のノード

	// プレイヤーが視野範囲内に入ったかどうか
	bool IsFoundPlayer() const;
	// 現在位置からプレイヤーが見えているかどうか
	bool IsLookPlayer() const;
	// プレイヤーを攻撃できるかどうか
	bool CanAttackPlayer(float attackRange) const;

	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed, float rotateSpeed);
	/// <summary>
	/// 次に巡回するポイントを変更
	/// </summary>
	/// <param name="patrolNearDist">設定できる最短距離</param>
	void ChangePatrolPoint(float patrolNearDist);
	
	// 巡回ポイントのリスト
	std::vector<CNavNode*> mpPatrolPoints;
	int mNextPatrolIndex;	// 次に巡回するポイントの番号
	
	// 求めた最短経路記憶用
	std::vector<CNavNode*> mpMoveRoute;
	int mNextMoveIndex;		// 次に移動するノードのインデックス値

	// 攻撃してきた相手の記憶用
	CObjectBase* mpDamageCauser;
};