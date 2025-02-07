#pragma once
#include "CPlayerBase.h"

#include "CObjectBase.h"
/*
配達ゲームのプレイヤークラス
*/
class CDeliveryPlayer : public CObjectBase
{
public:
	// コンストラクタ
	CDeliveryPlayer();
	// デストラクタ
	~CDeliveryPlayer();

	// 更新
	void Update();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);// override;

	// 描画
	void Render();

private:
	// コライダ―を生成
	void CreateCol();

	// 3dモデル
	CModel* mpModel;

	CCollider* mpBodyCol;
};