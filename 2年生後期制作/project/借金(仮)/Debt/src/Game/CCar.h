#pragma once
#include "CCharaBase.h"
#include "CCarStatus.h"
#include "CColliderCapsule.h"
#include "CModel.h"

class CCar : public CCharaBase, public CCarStatus
{
public:
	CCar(CModel* model, const CVector& pos, const CVector& scale, const CVector& rotation);
	~CCar();

	void Update();
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	void Render();

	/// <summary>
	/// ³–Ê•ûŒü‚ÖˆÚ“®‚·‚é
	/// </summary>
	/// <param name="moveSpeed">ˆÚ“®‘¬“x</param>
	void Move(float moveSpeed);
private:
	CModel* mpModel;
	CColliderCapsule* mpColliderCapsule;
};