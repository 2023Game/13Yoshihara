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

	// ゴミ袋の持ち主を設定
	void SetOwner(CObjectBase* owner);
	// ゴミ袋の持ち主を取得
	CObjectBase* GetOwner() const;

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

	// ゴミ袋をアタッチする行列を設定
	void SetAttachMtx(const CMatrix* mtx);
	// 行列を取得
	CMatrix Matrix() const;
	// 行列を更新
	void UpdateMtx();

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

	CObjectBase* mpOwner;		// ゴミ袋の持ち主
	const CMatrix* mpAttachMtx;	// くっつける行列のポインター
	CMatrix mAttachMtx;			// くっつける行列の本体
};