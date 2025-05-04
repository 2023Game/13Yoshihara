#pragma once
#include "CColBase.h"

class CColliderBox;

class CColBox : public CColBase
{
public:
	CColBox(CVector min, CVector max, const CColor defaultColor,
		bool isKinematic = false, float weight = 1.0f);
	~CColBox();

	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	void Render() override;
private:
	// ボックスコライダ―
	CColliderBox* mpCollider;
	// ボックスを構成する四角形のリスト
	std::list<SRVertexData> mVertices;
};