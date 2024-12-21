#include "CHomePlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"

#define CAPSULE_RADIUS 2.5f	// �J�v�Z���R���C�_�̔��a
#define PLAYER_HEIGHT 16.0f	// �v���C���[�̍���

#define MOVE_SPEED 0.375f * 2.0f

#define ANIM_PATH "Character\\Player\\anim\\"

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f},	// T�|�[�Y
	{ ANIM_PATH"idle.x",		true,	153.0f,	1.0f},	// �ҋ@
	{ ANIM_PATH"walk.x",		true,	66.0f,	1.0f},	// ���s
};

CHomePlayer::CHomePlayer()
	: CPlayerBase(CAPSULE_RADIUS, PLAYER_HEIGHT)
	, mState(EState::eIdle)
{
	// �A�j���[�V�����ƃ��f����������
	InitAnimationModel("Player", &ANIM_DATA);

	//�t�B�[���h�A�ǁA�I�u�W�F�N�g�Ƃ����Փ˔��������
	mpColliderCapsule = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, CAPSULE_RADIUS, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT - CAPSULE_RADIUS, 0.0f),
		CAPSULE_RADIUS
	);
	mpColliderCapsule->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::eObject });

	//�C���^���N�g�I�u�W�F�N�g�Ƃ����Փ˔���
	mpColliderLine = new CColliderLine
	(
		this, ELayer::ePlayer,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eInteract });

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);
}

CHomePlayer::~CHomePlayer()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}
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