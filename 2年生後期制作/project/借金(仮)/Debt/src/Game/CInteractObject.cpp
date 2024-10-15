#include "CInteractObject.h"

CInteractObject::CInteractObject()
{
	mpColliderSphere = new CColliderSphere(GetParent(), ELayer::eInteract, 5, true);
}

CInteractObject::~CInteractObject()
{
}

void CInteractObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::ePlayer)
	{
		
	}
}
