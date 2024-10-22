#pragma once
#include "CRideableObject.h"
#include "CColliderSphere.h"

class CInteractObject : public CRideableObject
{
public:
	CInteractObject(float radius, std::string objectName);
	virtual ~CInteractObject();

	// インタラクト判定
	void Interact();
	
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自分のコライダー</param>
	/// <param name="other">衝突した相手のコライダ―</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

protected:
	CColliderSphere* mpColliderSphere;
	bool mIsInteract;		// インタラクトしているかどうか
	bool mIsInteractArea;	// インタラクトエリア内かどうか
};