#include "CInteractObject.h"
#include "CInput.h"
#include "CBillBoardImage.h"
#include "CObjectBase.h"
#include "CPlayer.h"

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
	mpBillBoardImage = new CBillBoardImage(INTERACT_IMAGE, ETag::eInteractUI, ETaskPauseType::eGame);
	mpBillBoardImage->SetSize(IMAGE_SIZE);
	mpBillBoardImage->SetShow(false);
}

CInteractObject::~CInteractObject()
{
	if (mpColliderSphere != nullptr)
	{
		delete mpColliderSphere;
		mpColliderSphere = nullptr;
	}
	if (mpBillBoardImage != nullptr)
	{
		mpBillBoardImage->Kill();
	}
}

// �C���^���N�g����
void CInteractObject::Interact()
{
	CVector direction = CPlayer::Instance()->Position() - mpBillBoardImage->Position();
	direction.Normalize();
	direction = direction * 10.0f;
	// �v���C���[���Ƀr���{�[�h��\��
	mpBillBoardImage->
		Position(CVector(Position().X() + direction.X(),
			CPlayer::Instance()->Position().Y() + 10.0f,
			Position().Z() + direction.Z()));

	// �C���^���N�g�G���A����F�L�[��������mIsInteract��true
	if (mIsInteractArea == true)
	{
		mpBillBoardImage->SetShow(true);
		if(CInput::PushKey('F'))
		{
			mIsInteract = true;
		}
	}
	else
	{
		mpBillBoardImage->SetShow(false);
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
