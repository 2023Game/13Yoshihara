#include "CSwitchButton.h"
#include "CColliderSphere.h"
#include "CSwitch.h"
#include "CModel.h"

// �����鋗��
#define PUSH_POS_Y 0.5f
// �㉺���鑬�x
#define MOVE_SPEED 0.1f
// �����Ă��邩�̃R���C�_�[�̔��a
#define COL_RADIUS 5.0f

// �R���X�g���N�^
CSwitchButton::CSwitchButton(CVector pos, CSwitch* owner)
	: CObjectBase(ETag::eSwitch, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mState(EState::eDefault)
	, mDefaultY(pos.Y())
	, mpOwner(owner)
{
	mpModel = CResourceManager::Get<CModel>("SwitchButton");
	// ���W��ݒ�
	Position(pos);
	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CSwitchButton::~CSwitchButton()
{
}

// �X�V
void CSwitchButton::Update()
{
	std::string state = "Default";
	if (mState == EState::ePush)state = "Push";
	CDebugPrint::Print("State:%s\n", state.c_str());
	switch (mState)
	{
	case EState::eDefault:	UpdateDefault();	break;
	case EState::ePush:		UpdatePush();		break;
	}

	ChangeState(EState::eDefault);
}

// �`��
void CSwitchButton::Render()
{
	mpModel->Render(Matrix());
}

// �R���C�_�[�𐶐�
void CSwitchButton::CreateCol()
{
	mpPushCol = new CColliderSphere
	(
		this, ELayer::eSwitch,
		COL_RADIUS,
		true
	);
	// �v���C���[�A�G�A�I�u�W�F�N�g�Ƃ����Փ�
	mpPushCol->SetCollisionLayers({ ELayer::ePlayer,
		ELayer::eEnemy,ELayer::eObject });
}

// �Փˏ���
void CSwitchButton::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �����Ă��邩�̔���R���C�_�[�Ƃ̏Փ˔���
	if (self == mpPushCol)
	{
		ChangeState(EState::ePush);
	}
}

// ��Ԃ�ύX
void CSwitchButton::ChangeState(EState state)
{
	if (mState != state) mState = state;
}

// ������Ă��Ȃ����̍X�V����
void CSwitchButton::UpdateDefault()
{
	// ������Ă��Ȃ�
	mpOwner->SetOnOff(false);

	// ������Y���W�܂ŏ㏸���Ă���
	if (Position().Y() < mDefaultY)
	{
		CVector pos = Position();
		pos.Y(pos.Y() + MOVE_SPEED);
		Position(pos);
	}
	// �ȏ�Ȃ珉����Y���W�ɐݒ�
	else
	{
		CVector pos = Position();
		pos.Y(mDefaultY);
		Position(pos);
	}
}

// ������Ă��鎞�̍X�V����
void CSwitchButton::UpdatePush()
{
	// ������Ă���
	mpOwner->SetOnOff(true);

	// ������Ă����Ԃ�Y���W�܂ŉ��~���Ă���
	if (Position().Y() > mDefaultY - PUSH_POS_Y)
	{
		CVector pos = Position();
		pos.Y(pos.Y() - MOVE_SPEED);
		Position(pos);
	}
	// �ȉ��Ȃ牟����Ă����Ԃ�Y���W�ɐݒ�
	else
	{
		CVector pos = Position();
		pos.Y(mDefaultY - PUSH_POS_Y);
		Position(pos);
	}
}
