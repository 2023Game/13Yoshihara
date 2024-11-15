#pragma once
#include "CCharaBase.h"
#include "CColliderCapsule.h"
#include "CModel.h"

class CCar : public CCharaBase
{
public:
	CCar(CModel* model, const CVector& pos, const CVector& scale, const CVector& rotation);
	~CCar();

	void Update();
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	void Render();
private:
	CModel* mpModel;
	CColliderCapsule* mpColliderCapsule;

	int mHp;
};