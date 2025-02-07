#pragma once
#include "CEnemyBase.h"

/*
配達ゲームの敵クラス
敵基底クラスを継承
*/
class CDeliveryEnemy : public CEnemyBase
{
public:
	// コンストラクタ
	CDeliveryEnemy();
	// デストラクタ
	~CDeliveryEnemy();

	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render();

private:
	// コライダ―を生成
	void CreateCol();

	// 3dモデル
	CModel* mpModel;
};