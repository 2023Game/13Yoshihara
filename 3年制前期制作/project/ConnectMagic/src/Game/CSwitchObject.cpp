#include "CSwitchObject.h"
#include "CModel.h"

// �R���X�g���N�^
CSwitchObject::CSwitchObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eSwitchObject, prio, 0, pause)
	, mState(EState::eOff)
{
}

// �f�X�g���N�^
CSwitchObject::~CSwitchObject()
{
	SAFE_DELETE(mpCol);
}

// �X�V
void CSwitchObject::Update()
{
	switch (mState)
	{
	case EState::eOff:	UpdateOff();	break;
	case EState::eOn:	UpdateOn();		break;
	}

	std::string state = "Off";
	if (mState == EState::eOn) state = "On";

#if _DEBUG
	CDebugPrint::Print("SwitchObjState:%s\n", state.c_str());
	CDebugPrint::Print("RotY:%f\n", EulerAngles().Y());
#endif
}

// �`��
void CSwitchObject::Render()
{
	mpModel->Render(Matrix());
}

// �Փˏ���
void CSwitchObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// �I���I�t��؂�ւ���
void CSwitchObject::SetOnOff(bool isOnOff)
{
	if (isOnOff)
	{
		ChangeState(EState::eOn);
	}
	else
	{
		ChangeState(EState::eOff);
	}
}

// ��p���Ă��Ȃ����̏���
void CSwitchObject::UpdateOff()
{
}

// ��p���Ă��鎞�̏���
void CSwitchObject::UpdateOn()
{
}

// ��Ԃ̕ύX
void CSwitchObject::ChangeState(EState state)
{
	if (mState != state) mState = state;
}
