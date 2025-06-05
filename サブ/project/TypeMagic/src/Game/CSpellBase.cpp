#include "CSpellBase.h"
#include "CModel.h"

// �R���X�g���N�^
CSpellBase::CSpellBase(ESpellElementalType elemental, ESpellShapeType shape,
	CObjectBase* owner, CObjectBase* target)
	: CObjectBase(ETag::eSpell, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, CSpellStatus(elemental, shape)
	, mpModel(nullptr)
	, mpOwner(owner)
	, mpTarget(target)
{
	// �ҋ@��Ԃ�
	ChangeState(EState::eIdle);
}

// �f�X�g���N�^
CSpellBase::~CSpellBase()
{
	SAFE_DELETE(mpSpellCol);
}

// �X�V
void CSpellBase::Update()
{
	switch (mState)
	{
	case CSpellBase::EState::eIdle:			UpdateIdle();		break;
	case CSpellBase::EState::eShooting:		UpdateShooting();	break;
	}

	// �ړ�
	Position(Position() + mMoveSpeed);
}

// �`��
void CSpellBase::Render()
{
	mpModel->Render(Matrix());
}

// �����̃^�[�Q�b�g��ݒ�
void CSpellBase::SetTarget(CObjectBase* target)
{
	mpTarget = target;
}

// �ҋ@���̍X�V
void CSpellBase::UpdateIdle()
{
}

// ��Ԃ�؂�ւ�
void CSpellBase::ChangeState(EState state)
{
	if (mState == state) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}
