#include "CHomePlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"

#define CAPSULE_RADIUS 2.5f	// �J�v�Z���R���C�_�̔��a
#define PLAYER_HEIGHT 16.0f	// �v���C���[�̍���

#define MOVE_SPEED 0.375f * 2.0f


// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CHomePlayer::AnimData CHomePlayer::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// T�|�[�Y
	{ "Character\\Player\\anim\\idle.x",		true,	153.0f	},	// �ҋ@
	{ "Character\\Player\\anim\\walk.x",		true,	66.0f	},	// ���s
};

CHomePlayer::CHomePlayer()
	: CPlayerBase(CAPSULE_RADIUS, PLAYER_HEIGHT)
	, mState(EState::eIdle)
{
	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("Player");

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);


	//�C���^���N�g�I�u�W�F�N�g�Ƃ����Փ˔���
	mpColliderLine = new CColliderLine
	(
		this, ELayer::ePlayer,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eInteract });

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);
}

CHomePlayer::~CHomePlayer()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}
	if (mpColliderCapsule != nullptr)
	{
		delete mpColliderCapsule;
		mpColliderCapsule = nullptr;
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

	// �ҋ@���ƃW�����v���́A�ړ��������s��
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
			ChangeAnimation(EAnimType::eMove);
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eIdle);
		}
	}
}

void CHomePlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

void CHomePlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);
}