#include "CInteractObject.h"
#include "CInput.h"

//�R���X�g���N�^
CInteractObject::CInteractObject(float radius, std::string interactName)
	: mIsInteract(false)
	, mIsInteractArea(false)
{
	mpColliderSphere = new CColliderSphere(this, ELayer::eInteract, radius, true);
	mInteractName = interactName;
}

CInteractObject::~CInteractObject()
{
}

// �C���^���N�g����
void CInteractObject::Interact()
{
	// �C���^���N�g�G���A����F�L�[��������mIsInteract��true
	if (mIsInteractArea == true)
	{
		if(CInput::PushKey('F'))
		{
			mIsInteract = true;
		}
	}

	// �v���C���[�ƏՓ˂��ĂȂ��Ƃ���mIsInteractArea��false
	mIsInteractArea = false;
}

// �Փˏ���
void CInteractObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	//�v���C���[�ƏՓ˂����Ƃ�
	if (other->Layer() == ELayer::eInteract)
	{
		// �v���C���[�ƏՓ˂��Ă���Ƃ��̓C���^���N�g�G���A��
		mIsInteractArea = true;
	}
}
