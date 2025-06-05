#include "CSpellBase.h"
#include "CModel.h"

// コンストラクタ
CSpellBase::CSpellBase(ESpellElementalType elemental, ESpellShapeType shape,
	CObjectBase* owner, CObjectBase* target)
	: CObjectBase(ETag::eSpell, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, CSpellStatus(elemental, shape)
	, mpModel(nullptr)
	, mpOwner(owner)
	, mpTarget(target)
{
	// 待機状態へ
	ChangeState(EState::eIdle);
}

// デストラクタ
CSpellBase::~CSpellBase()
{
	SAFE_DELETE(mpSpellCol);
}

// 更新
void CSpellBase::Update()
{
	switch (mState)
	{
	case CSpellBase::EState::eIdle:			UpdateIdle();		break;
	case CSpellBase::EState::eShooting:		UpdateShooting();	break;
	}

	// 移動
	Position(Position() + mMoveSpeed);
}

// 描画
void CSpellBase::Render()
{
	mpModel->Render(Matrix());
}

// 呪文のターゲットを設定
void CSpellBase::SetTarget(CObjectBase* target)
{
	mpTarget = target;
}

// 待機中の更新
void CSpellBase::UpdateIdle()
{
}

// 状態を切り替え
void CSpellBase::ChangeState(EState state)
{
	if (mState == state) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}
