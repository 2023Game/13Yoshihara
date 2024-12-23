#include "CInteractObject.h"
#include "CInput.h"
#include "CBillBoardUI.h"
#include "CObjectBase.h"
#include "CPlayerBase.h"
#include "CCamera.h"

#define INTERACT_IMAGE "UI\\interact.png"
#define IMAGE_SIZE CVector2(6.0f,2.0f)

//�R���X�g���N�^
CInteractObject::CInteractObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eField, prio, 0, pause)
	, mInteractStr("���ׂ�")
	, mIsInteract(false)
	, mIsInteractArea(false)
#if _DEBUG
	, mDebugName("InteractObj")
#endif
{
	// �C���^���N�g�̉摜��\��
	mpBillBoardUI = new CBillBoardUI(INTERACT_IMAGE, ETag::eInteractUI, ETaskPauseType::eGame);
	mpBillBoardUI->SetSize(IMAGE_SIZE);
	mpBillBoardUI->SetShow(false);
}

CInteractObject::~CInteractObject()
{
	if (mpBillBoardUI != nullptr)
	{
		mpBillBoardUI->Kill();
	}
}

// ���ׂ���e�̃e�L�X�g��Ԃ�
std::string CInteractObject::GetInteractStr() const
{
	return mInteractStr;
}

// ���ׂ����Ԃ��ǂ���
bool CInteractObject::CanInteract() const
{
	return true;
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

#if _DEBUG
// �f�o�b�O�\���p�̖��O���擾
std::string CInteractObject::GetDebugName() const
{
	return mDebugName;
}

// �f�o�b�O�\���p�̖��O��ݒ�
void CInteractObject::SetDebugName(std::string name)
{
	mDebugName = name;
}
#endif