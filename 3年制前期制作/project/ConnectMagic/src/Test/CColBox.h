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
	// �{�b�N�X�R���C�_�\
	CColliderBox* mpCollider;
	// �{�b�N�X���\������l�p�`�̃��X�g
	std::list<SRVertexData> mVertices;
};