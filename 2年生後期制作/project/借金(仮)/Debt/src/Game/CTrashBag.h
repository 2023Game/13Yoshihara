#pragma once
#include "CObjectBase.h"
#include "CTrashBagStatus.h"

class CModel;

class CTrashBag : public CObjectBase , public CTrashBagStatus
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="gold">ゴールドかどうか</param>
	CTrashBag(bool gold);
	// デストラクタ
	~CTrashBag();

	// 更新
	void Update();
	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	// 描画
	void Render() override;

	// 有効無効にする
	void SetOnOff(bool isOnOff);

	// ゴールド化する
	void SetGold(bool gold);

	// 飛んでいく速度を設定
	void SetThrowSpeed(CVector speed, float speedY);
	// 重力をかけるかを設定
	void SetGravity(bool gravity);

private:
	// 本体コライダ―
	CCollider* mpBodyCol;
	// 3Dモデル
	CModel* mpModel;

	// 前後左右の移動速度
	CVector mMoveSpeed;
	// 上下の移動速度
	float mMoveSpeedY;

	CTransform* mpRideObject;

	CVector mGroundNormal;	// 接地している地面の法線
	bool mIsGrounded;		// 接地しているかどうか

	bool mIsGravity;	// 重力を掛けるかどうか
};