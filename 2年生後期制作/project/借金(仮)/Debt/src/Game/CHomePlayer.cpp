#include "CHomePlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CInteractObject.h"

#define CAPSULE_RADIUS 2.5f	// �J�v�Z���R���C�_�̔��a
#define PLAYER_HEIGHT 16.0f	// �v���C���[�̍���

#define SEARCH_RADIUS 10.0f	// ���ׂ�I�u�W�F�N�g�̒T�m�͈͂̔��a

#define MOVE_SPEED 0.375f * 2.0f		// �ړ����x

#define ANIM_PATH "Character\\Player\\anim\\"

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f},	// T�|�[�Y
	{ ANIM_PATH"idle.x",		true,	153.0f,	1.0f},	// �ҋ@
	{ ANIM_PATH"walk.x",		true,	66.0f,	1.0f},	// ���s
};

CHomePlayer::CHomePlayer()
	: CPlayerBase()
	, mState(EState::eIdle)
{
	// �A�j���[�V�����ƃ��f����������
	InitAnimationModel("Player", &ANIM_DATA);

	// �{�̂̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, CAPSULE_RADIUS, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT - CAPSULE_RADIUS, 0.0f),
		CAPSULE_RADIUS
	);
	//�t�B�[���h�A�ǁA�I�u�W�F�N�g�Ƃ����Փ�
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject });

	// ���ׂ�I�u�W�F�N�g��T�m����R���C�_�\
	mpSearchCol = new CColliderSphere
	(
		this, ELayer::eInteractSearch,
		SEARCH_RADIUS
	);
	mpSearchCol->Position(0.0f, SEARCH_RADIUS * 0.5f , 0.0f);
	// ���ׂ�I�u�W�F�N�g�Ƃ̂ݏՓ�
	mpSearchCol->SetCollisionTags({ ETag::eInteractObject });
	mpSearchCol->SetCollisionLayers({ ELayer::eInteractObj });

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);
}

CHomePlayer::~CHomePlayer()
{

}

void CHomePlayer::Update()
{
	switch (mState)
	{
	case EState::eIdle:
		UpdateIdle();
		break;
	}

	// �ҋ@���́A�ړ��������s��
	if (mState == EState::eIdle)
	{
		UpdateMove();
	}

	CPlayerBase::Update();

	CDebugPrint::Print("State:%d\n", mState);
}

void CHomePlayer::UpdateIdle()
{
	// �ڒn���Ă����
	if (mIsGrounded)
	{
		// �߂��̒��ׂ�I�u�W�F�N�g���擾
		CInteractObject* obj = GetNearInteractObject();
		if (obj != nullptr)
		{
			// F�L�[����������A�߂��̒��ׂ�I�u�W�F�N�g�𒲂ׂ�
			if (CInput::PushKey('F'))
			{
				obj->Interact();
			}
		}
	}
}

// �ړ��̍X�V����
void CHomePlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// �v���C���[�̈ړ��x�N�g�������߂�
	CVector move = CalcMoveVec();
	// ���߂��ړ��x�N�g���̒����œ��͂���Ă��邩����
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * MOVE_SPEED;

		// �ҋ@��Ԃł���΁A�ړ��A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			ChangeAnimation((int)EAnimType::eMove);
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			ChangeAnimation((int)EAnimType::eIdle);
		}
	}
}

void CHomePlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);
}