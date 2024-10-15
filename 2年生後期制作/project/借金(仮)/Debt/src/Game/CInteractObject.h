#pragma once
#include "CRideableObject.h"
#include "CColliderSphere.h"

class CInteractObject : public CRideableObject
{
public:
	CInteractObject();
	virtual ~CInteractObject();
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自分のコライダー</param>
	/// <param name="other">衝突した相手のコライダ―</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;
private:
	bool interact;//インタラクトされたかどうか
	CColliderSphere* mpColliderSphere;
};