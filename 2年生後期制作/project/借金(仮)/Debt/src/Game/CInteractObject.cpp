#include "CInteractObject.h"
#include "CInput.h"
#include "CBillBoardUI.h"
#include "CObjectBase.h"
#include "CPlayerBase.h"
#include "CCamera.h"

#define INTERACT_IMAGE "UI\\interact.png"
#define IMAGE_SIZE CVector2(6.0f,2.0f)

//�R���X�g���N�^
CInteractObject::CInteractObject(float radius, std::string interactName)
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mIsInteract(false)
	, mIsInteractArea(false)
{
	// �v���C���[�Ƃ����Փ˔��������
	mpColliderSphere = new CColliderSphere(this, ELayer::eInteract, radius, true);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });

	// �C���^���N�g�̉摜��\��
	mpBillBoardUI = new CBillBoardUI(INTERACT_IMAGE, ETag::eInteractUI, ETaskPauseType::eGame);
	mpBillBoardUI->SetSize(IMAGE_SIZE);
	mpBillBoardUI->SetShow(false);
}

CInteractObject::~CInteractObject()
{
	if (mpColliderSphere != nullptr)
	{
		delete mpColliderSphere;
		mpColliderSphere = nullptr;
	}
	if (mpBillBoardUI != nullptr)
	{
		mpBillBoardUI->Kill();
	}
}

// �C���^���N�g����
void CInteractObject::Interact()
{
	// �v���C���[�̍����Ƀr���{�[�h��\��
	mpBillBoardUI->
		Position(Position().X(),
			CPlayerBase::Instance()->Position().Y() + 10.0f,
			Position().Z());

	// �C���^���N�g�G���A����F�L�[��������mIsInteract��true
	if (mIsInteractArea == true)
	{
		mpBillBoardUI->SetShow(true);
		if(CInput::PushKey('F'))
		{
			mIsInteract = true;
		}
	}
	else
	{
		mpBillBoardUI->SetShow(false);
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
		mpPlayer = other->Owner();
		// �v���C���[�ƏՓ˂��Ă���Ƃ��̓C���^���N�g�G���A��
		mIsInteractArea = true;
	}
}
