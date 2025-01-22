#include "CResident.h"
#include "CDebugFieldOfView.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "Primitive.h"

// �R���C�_�̃C���N���[�h
#include "CColliderCapsule.h"

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\Resident\\anim\\"

// �Z�l�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CEnemyBase::AnimData> ANIM_DATA =
{
	{"",					 true, 0.0f,1.0f},	// T�|�[�Y
	{ ANIM_PATH"Idle.x",	 true,80.0f,1.0f},	// �ҋ@
	{ ANIM_PATH"Move.x",	 true,80.0f,1.0f},	// �ړ�
	{ ANIM_PATH"ThrowBag.x",false,80.0f,1.0f},	// �S�~�܂𓊂���
};

#define BODY_RADIUS 2.5f	// �{�̂̃R���C�_�\�̔��a
#define BODY_HEIGHT 25.0f	// �{�̂̃R���C�_�\�̍���

#define ROTATE_SPEED 9.0f	// ��]���x

#define PATROL_NEAR_DIST 10.0f	// ����J�n���ɑI������鏄��|�C���g�̍ŒZ����
#define IDLE_TIME 3.0f			// �ҋ@��Ԃ̎���

#define SCALE 5.0f	// �X�P�[��

// �R���X�g���N�^
CResident::CResident(CModelX* model, CVector startPos, 
	std::vector<CNavNode*> patrolPoints)
	: CEnemyBase
	(
		0.0f,
		0.0f,
		{},
		0.0f
	)
	, CResidentStatus()
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsMovePause(false)
{
	// ���f����ݒ�
	mpModel = model;
	// ���������W��ݒ�
	Position(startPos);

	// ����|�C���g��ݒ�
	mpPatrolPoints = patrolPoints;

	// �傫���̒���
	Scale(SCALE, SCALE, SCALE);
	// �A�j���[�V�����ƃ��f���̏�����
	InitAnimationModel("Resident", &ANIM_DATA);

	// �{�̂̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eResident,
		CVector(0.0f, BODY_RADIUS / SCALE, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS / SCALE, 0.0f),
		BODY_RADIUS
	);
	// �n�`�A�v���C���[�A�G�A�����
	// �ƏՓ˔��������
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy});
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,
		ELayer::ePlayer, ELayer::eEnemy, ELayer::eCollector });

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);
}

// �f�X�g���N�^
CResident::~CResident()
{
}

// �X�V
void CResident::Update()
{
	// ���݂̏�Ԃɍ��킹�čX�V������؂�ւ�
	switch (mState)
	{
	case EState::eIdle:				UpdateIdle();			break;
	case EState::ePatrol:			UpdatePatrol();			break;
	case EState::eThrowBag:			UpdateThrowBag();		break;
	}

	// �S�~�𓊂���܂ł��J�E���g�_�E������
	CountThrowTime();

	// �L�����N�^�[�̍X�V
	CEnemyBase::Update();

#if _DEBUG
	// ���݂̏�Ԃɍ��킹�Ď���͈͂̐F��ύX
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("ResidentState:%s\n", GetStateStr(mState).c_str());
#endif
}

// �Փˏ���
void CResident::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ���N���X�̏Փˏ���
	CEnemyBase::Collision(self, other, hit);
}

// �`��
void CResident::Render()
{
	CEnemyBase::Render();

	// �����Ԃł���΁A
	if (mState == EState::ePatrol)
	{
		// ����|�C���g��S�ĕ`��
		int size = mpPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawWireBox
			(
				mpPatrolPoints[i]->GetPos() + CVector(0.0f, 1.0f, 0.0f),
				CVector::one,
				c
			);
		}
	}
}

// �ҋ@���
void CResident::UpdateIdle()
{
	// �S�~�܂𓊂���܂ł̎��Ԃ��o�߂�����
	if (IsElapsedThrowTime())
	{
		// �S�~�܂𓊂����Ԃ�
		ChangeState(EState::eThrowBag);
		return;
	}

	// �ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);


	if (mElapsedTime < IDLE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// �ҋ@���Ԃ��o�߂�����A�����Ԃֈڍs
		ChangeState(EState::ePatrol);
		mElapsedTime = 0.0f;
		mStateStep = 0;
	}
}

// ���񏈗�
void CResident::UpdatePatrol()
{
	// �S�~�܂𓊂���܂ł̎��Ԃ��o�߂�����
	if (IsElapsedThrowTime())
	{
		// �ړ��̒��f
		mIsMovePause = true;
		// �S�~�܂𓊂����Ԃ�
		ChangeState(EState::eThrowBag);
		return;
	}

	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
		// �X�e�b�v0�F����J�n���̏���|�C���g�����߂�
	case 0:
		// �ړ������f����Ă��Ȃ���Ώ���|�C���g��ύX
		if (!mIsMovePause)
		{
			mNextPatrolIndex = -1;
			ChangePatrolPoint(PATROL_NEAR_DIST);
		}
		else
		{
			mIsMovePause = false;
		}
		mStateStep++;
		break;

		// �X�e�b�v1�F����|�C���g�܂ňړ�
	case 1:
	{
		// �ړ��A�j���[�V�������Đ�
		ChangeAnimation((int)EAnimType::eMove);

		if (mpMoveRoute.size() == 1)
		{
			mNextMoveIndex = 0;
		}
		// �ŒZ�o�H�̎��̃m�[�h�܂ňړ�
		CNavNode* moveNode = mpMoveRoute[mNextMoveIndex];

		if (MoveTo(moveNode->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{
			// �ړ����I���΁A���̃m�[�h�֐؂�ւ�
			mNextMoveIndex++;
			// �Ō�̃m�[�h�i�ړI�n�̃m�[�h�j�������ꍇ�́A���̃X�e�b�v�֐i�߂�
			if (mNextMoveIndex >= mpMoveRoute.size())
			{
				mStateStep++;
			}
		}
		break;
	}
	// �X�e�b�v2�F�ړ���͑ҋ@��Ԃ�
	case 2:
		ChangeState(EState::eIdle);
		break;
	}
}

// �S�~�܂𓊂��鏈��
void CResident::UpdateThrowBag()
{
	// TODO�F�ړ���~�A�S�~�܂���ɐ����A�����_���ȕ����ɓ�����
	switch (mStateStep)
	{
		// �X�e�b�v0�F�S�~�܂𓊂���A�j���[�V�����Đ�
	case 0:
		ChangeAnimation((int)EAnimType::eThrowBag);
		mStateStep++;
		break;

		// �X�e�b�v1�F�A�j���[�V�������I��������
	case 1:
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;

		// �X�e�b�v2�F������܂ł̎��Ԃ����Z�b�g���ď����Ԃ�
	case 2:
		// ������܂ł̎��Ԃ����Z�b�g
		SetThrowTime();
		// �����Ԃ�
		ChangeState(EState::ePatrol);
		break;
	}
}

// ��Ԑ؂�ւ�
void CResident::ChangeState(EState state)
{
	// ���ɓ�����Ԃł���΁A�������Ȃ�
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

#if _DEBUG
// ��Ԃ̕�������擾
std::string CResident::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return "�ҋ@";
	case EState::ePatrol:			return "����";
	case EState::eThrowBag:			return "�S�~�܂𓊂���";
	}
	return "";
}

// ��Ԃ̐F���擾
CColor CResident::GetStateColor(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return CColor::white;
	case EState::ePatrol:			return CColor::green;
	case EState::eThrowBag:			return CColor::red;
	}
	return CColor::white;
}
#endif