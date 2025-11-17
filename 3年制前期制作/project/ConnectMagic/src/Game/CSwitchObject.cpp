#include "CSwitchObject.h"
#include "CModel.h"

// コンストラクタ
CSwitchObject::CSwitchObject(ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(ETag::eSwitchObject, prio, 0, pause)
	, mState(EState::eOff)
	, mOnSwitchNum(0)
	, mpModel(nullptr)
	, mpCol(nullptr)
{
}

// デストラクタ
CSwitchObject::~CSwitchObject()
{
	SAFE_DELETE(mpCol);
}

// 更新
void CSwitchObject::Update()
{
	switch (mState)
	{
	case EState::eOff:	UpdateOff();	break;
	case EState::eOn:	UpdateOn();		break;
	}
}

// 描画
void CSwitchObject::Render()
{
	if (mpModel != nullptr)
	{
		mpModel->Render(Matrix());
	}
}

// 衝突処理
void CSwitchObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// オンオフを切り替える
void CSwitchObject::SetOnOff(bool isOnOff)
{
	// スイッチが設定されていない時は引数に依存する
	if (mSwitchs.size() == 0)
	{
		if (isOnOff)
		{
			// オンにする
			ChangeState(EState::eOn);
		}
		else
		{
			// オフにする
			ChangeState(EState::eOff);
		}
		return;
	}

	// オンならオンのスイッチ数を増やす
	if (isOnOff)
	{
		mOnSwitchNum++;
	}
	// オフならオンのスイッチ数を減らす
	else
	{
		mOnSwitchNum--;
	}

	// オンになったスイッチの数とスイッチの総数が一致したら
	if (mSwitchs.size() == mOnSwitchNum)
	{
		// オンにする
		ChangeState(EState::eOn);
	}
	// 一致してないなら
	else
	{
		// オフにする
		ChangeState(EState::eOff);
	}
}

// 作用するスイッチを設定する
void CSwitchObject::SetSwitchs(std::vector<CSwitch*> switchs)
{
	mSwitchs = switchs;
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
