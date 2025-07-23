#include "CSwitchObject.h"
#include "CModel.h"

// �R���X�g���N�^
CSwitchObject::CSwitchObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eSwitchObject, prio, 0, pause)
	, mState(EState::eOff)
	, mOnSwitchNum(0)
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
	// �I���Ȃ�I���̃X�C�b�`���𑝂₷
	if (isOnOff)
	{
		mOnSwitchNum++;
	}
	// �I�t�Ȃ�I���̃X�C�b�`�������炷
	else
	{
		mOnSwitchNum--;
	}

	// �I���ɂȂ����X�C�b�`�̐��ƃX�C�b�`�̑�������v������
	if (mSwitchs.size() == mOnSwitchNum)
	{
		// �I���ɂ���
		ChangeState(EState::eOn);
	}
	// ��v���ĂȂ��Ȃ�
	else
	{
		// �I�t�ɂ���
		ChangeState(EState::eOff);
	}
}

// ��p����X�C�b�`��ݒ肷��
void CSwitchObject::SetSwitchs(std::vector<CSwitch*> switchs)
{
	mSwitchs = switchs;
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
