#include "CInteractObject.h"
#include "CPlayer.h"

//�R���X�g���N�^
CInteractObject::CInteractObject()
{
	mpColliderSphere = new CColliderSphere(this, ELayer::eInteract, 5, true);
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
		//mIsInteract�̒l���v���C���[�̃C���^���N�g�̒l�Ɠ����ɂ���
		SetInteract(CPlayer::Instance()->GetInteract());
	}
}

//mIsInteract�̒l��ݒ肷��
void CInteractObject::SetInteract(bool interact)
{
	mIsInteract = interact;
}

//mIsInteract�̒l��Ԃ�
bool CInteractObject::GetInteract()
{
	return mIsInteract;
}
