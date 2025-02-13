#pragma once
#include "CObjectBase.h"

class CModel;

// 配達の障害物
class CDeliveryObstruction : public CObjectBase
{
public:
	// コンストラクタ
	CDeliveryObstruction();
	// デストラクタ
	~CDeliveryObstruction();

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
private:
	// コライダ―を生成
	void CreateCol();
	// 3Dモデル
	CModel* mpModel;
	// 本体コライダ―
	CCollider* mpBodyCol;
};