#include "CInteractObject.h"
#include "CPlayer.h"

//コンストラクタ
CInteractObject::CInteractObject()
{
	mpColliderSphere = new CColliderSphere(this, ELayer::eInteract, 15, true);
}

CInteractObject::~CInteractObject()
{
}

//衝突処理
void CInteractObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	//プレイヤーと衝突したとき
	if (other->Layer() == ELayer::ePlayer)
	{
		CPlayer::Instance()->SetInteract(true);
	}
}
