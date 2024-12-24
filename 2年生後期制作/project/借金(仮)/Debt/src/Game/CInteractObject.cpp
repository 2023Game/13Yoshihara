#include "CInteractObject.h"
#include "CInput.h"
#include "CInteractUI3D.h"
#include "CObjectBase.h"
#include "CCamera.h"
#include "CPlayerBase.h"

//�R���X�g���N�^
CInteractObject::CInteractObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eInteractObject, prio, 0, pause)
	, mInteractStr("���ׂ�")
#if _DEBUG
	, mDebugName("InteractObj")
#endif
{

	// �C���^���N�g�̉摜��\��
	mpInteractUI3D = new CInteractUI3D(this);
	mpInteractUI3D->SetOwner(this);
	// �ŏ��͔�\��
	mpInteractUI3D->SetEnable(false);
	mpInteractUI3D->SetShow(false);
}

CInteractObject::~CInteractObject()
{
	SAFE_DELETE(mpInteractUI3D);

	// �C���^���N�gUI�����݂�����A�ꏏ�ɍ폜
	if (mpInteractUI3D != nullptr)
	{
		mpInteractUI3D->SetOwner(nullptr);
		mpInteractUI3D->Kill();
	}
}

// �I�u�W�F�N�g�폜��`����֐�
void CInteractObject::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�̂��C���^���N�gUI�ł���΁A
	// �C���^���N�gUI�̃|�C���^����ɂ���
	if (obj == mpInteractUI3D)
	{
		mpInteractUI3D = nullptr;
	}
}

// �X�V
void CInteractObject::Update()
{
	// �C���^���N�gUI���X�V
	mpInteractUI3D->Position(Position() + mUIOffsetPos);

	// �v���C���[�̈�ԋ߂��ɂ��钲�ׂ�I�u�W�F�N�g�Ȃ�
	if (this == CPlayerBase::Instance()->GetNearInteractObject())
	{
		// UI��\��
		mpInteractUI3D->SetShow(true);
	}
	else
	{
		// UI���\��
		mpInteractUI3D->SetShow(false);
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

// �Փˏ���
void CInteractObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
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