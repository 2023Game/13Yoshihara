#include "CInteractObject.h"
#include "CPlayer.h"

//�R���X�g���N�^
CInteractObject::CInteractObject()
{
	mpColliderSphere = new CColliderSphere(this, ELayer::eInteract, 15, true);
}

CInteractObject::~CInteractObject()
{
}

//�Փˏ���
void CInteractObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	//�v���C���[�ƏՓ˂����Ƃ�
	if (other->Layer() == ELayer::ePlayer)
	{
		CPlayer::Instance()->SetInteract(true);
	}
}
