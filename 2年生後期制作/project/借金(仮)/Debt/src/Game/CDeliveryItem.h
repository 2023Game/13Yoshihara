#pragma once
#include "CObjectBase.h"
#include "CDeliveryItemStatus.h"

class CModel;
class CSound;

// 発射する配達物
class CDeliveryItem : public CObjectBase, public CDeliveryItemStatus
{
public:
	// コンストラクタ
	CDeliveryItem(CObjectBase* owner);
	// デストラクタ
	~CDeliveryItem();

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
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;


	// 移動を設定
	void SetMoveSpeed(CVector moveSpeed);

	// 持ち主を取得する
	CObjectBase* GetOwner();

private:
	// コライダ―を生成
	void CreateCol();
	// 3dモデル
	CModel* mpModel;
	// 本体コライダ―
	CCollider* mpBodyCol;
	// 持ち主
	CObjectBase* mpOwner;
	// 持ち主の座標
	CVector mOwnerPos;
	// ゴール音声
	CSound* mpGoalSE;
	// ヒット音
	CSound* mpHitSE;
	// ダメージ音
	CSound* mpDamageSE;

	// 移動
	CVector mMoveSpeed;
	// 移動するか
	bool mIsMove;
	// 経過時間
	float mElapsedTime;
	// 持ち主がプレイヤーか
	bool mIsPlayer;
};