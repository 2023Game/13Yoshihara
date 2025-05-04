#include "CSwitchObject.h"

// コンストラクタ
CSwitchObject::CSwitchObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eSwitchObject, prio, 0, pause)
	, mState(EState::eOff)
{
}

// デストラクタ
CSwitchObject::~CSwitchObject()
{
}

// 更新
void CSwitchObject::Update()
{
	switch (mState)
	{
	case EState::eOff:	UpdateOff();	break;
	case EState::eOn:	UpdateOn();		break;
	}

	std::string state = "Off";
	if (mState == EState::eOn) state = "On";
	CDebugPrint::Print("SwitchObjState:%s\n", state.c_str());
	CDebugPrint::Print("RotY:%f\n", EulerAngles().Y());
}

// 衝突処理
void CSwitchObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// オンオフを切り替える
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

// 作用していない時の処理
void CSwitchObject::UpdateOff()
{
}

// 作用している時の処理
void CSwitchObject::UpdateOn()
{
}

// 状態の変更
void CSwitchObject::ChangeState(EState state)
{
	if (mState != state) mState = state;
}
