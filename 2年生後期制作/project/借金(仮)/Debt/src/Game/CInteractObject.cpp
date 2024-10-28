#include "CInteractObject.h"
#include "CInput.h"
#include "CBillBoardImage.h"

#define INTERACT_IMAGE "res\\UI\\menu_item.png"

//�R���X�g���N�^
CInteractObject::CInteractObject(float radius, std::string interactName)
	: mIsInteract(false)
	, mIsInteractArea(false)
{
	// �v���C���[�Ƃ����Փ˔��������
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eInteract,
		radius,
		true
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });

	// TODO:�C���^���N�g�̉摜�\��
	// �C���^���N�g�̉摜��\��
	mpBillBoardImage=new CBillBoardImage
		(INTERACT_IMAGE,ETag::)
}

CInteractObject::~CInteractObject()
{
	if (mpColliderSphere != nullptr)
	{
		delete mpColliderSphere;
		mpColliderSphere = nullptr;
	}
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
	if (other->Layer() == ELayer::ePlayer)
	{
		// �v���C���[�ƏՓ˂��Ă���Ƃ��̓C���^���N�g�G���A��
		mIsInteractArea = true;
	}
}
