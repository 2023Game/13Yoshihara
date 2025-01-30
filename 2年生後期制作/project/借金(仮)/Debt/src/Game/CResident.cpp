#include "CResident.h"
#include "CDebugFieldOfView.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "Primitive.h"
#include "Maths.h"
#include "CTrashBag.h"

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
#define BODY_HEIGHT 2.5f	// �{�̂̃R���C�_�\�̍���

#define ROTATE_SPEED 9.0f	// ��]���x

#define IDLE_TIME 1.0f			// �ҋ@��Ԃ̎���

#define SCALE 5.0f	// �X�P�[��

// ������Ƃ��Ɍ�����W
#define LOOK_POS0  CVector(0.0f,0.0f,-100.0f)
#define LOOK_POS1  CVector(0.0f,0.0f, -80.0f)
#define LOOK_POS2  CVector(0.0f,0.0f, -60.0f)
#define LOOK_POS3  CVector(0.0f,0.0f, -40.0f)
#define LOOK_POS4  CVector(0.0f,0.0f, -20.0f)
#define LOOK_POS5  CVector(0.0f,0.0f,   0.0f)
#define LOOK_POS6  CVector(0.0f,0.0f,  20.0f)
#define LOOK_POS7  CVector(0.0f,0.0f,  40.0f)
#define LOOK_POS8  CVector(0.0f,0.0f,  60.0f)
#define LOOK_POS9  CVector(0.0f,0.0f,  80.0f)
#define LOOK_POS10 CVector(0.0f,0.0f, 100.0f)

// �S�~�𓊂���Ƃ��̃I�t�Z�b�g���W
#define TRASH_BAG_OFFSET_POSY CVector(0.0f,2.0f,0.0f)
#define TRASH_BAG_OFFSET_POSZ 5.0f

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
	, mLookPos(CVector::zero)
	, mTrashBagNum(-1)
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

	// �S�~�܂̐���
	CreateTrashBag();

	// �{�̂̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eResident,
		CVector(0.0f, BODY_RADIUS / SCALE, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS / SCALE, 0.0f),
		BODY_RADIUS
	);
	// �n�`�A�v���C���[�A�G�A������A�S�~��
	// �ƏՓ˔��������
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy,ETag::eTrashBag});
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,
		ELayer::ePlayer, ELayer::eEnemy, ELayer::eCollector,ELayer::eTrashBag });

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);
}

// �f�X�g���N�^
CResident::~CResident()
{
	// ���X�g���N���A
	mpTrashBags.clear();
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

// ���ɏ��񂷂�|�C���g��ύX����
void CResident::ChangePatrolPoint()
{
	// ����|�C���g���ݒ肳��Ă��Ȃ��ꍇ�́A�������Ȃ�
	int size = mpPatrolPoints.size();
	if (size == 0) return;

	// ����J�n���ł���΁A��ԍŏ��̏���|�C���g��I��
	if (mNextPatrolIndex == -1)
	{
		// ��ԍŏ��̏���|�C���g�ɐݒ肷��
		mNextPatrolIndex = 0;
	}
	// �ړ������f����Ă�����A
	// ����|�C���g�̔ԍ���ύX���Ȃ�
	else if (mIsMovePause)
	{
		mIsMovePause = false;
		// �Ō�̃m�[�h�i�ړI�n�̃m�[�h�j�������ꍇ�A���̏���|�C���g���w��
		if (mNextMoveIndex >= mpMoveRoute.size())
		{
			mNextPatrolIndex++;
			if (mNextPatrolIndex >= size) mNextPatrolIndex -= size;
		}
	}
	// ���񒆂������ꍇ�A���̏���|�C���g���w��
	else
	{
		mNextPatrolIndex++;
		if (mNextPatrolIndex >= size) mNextPatrolIndex -= size;
	}

	// ���ɏ��񂷂�|�C���g�����܂����ꍇ
	if (mNextPatrolIndex >= 0)
	{
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			// �o�H�T���p�̃m�[�h�̍��W���X�V
			mpNavNode->SetPos(Position());

			// ���ɏ��񂷂�|�C���g�̌o�H��ݒ�
			mpMoveRoute.clear();
			mpMoveRoute.push_back(mpNavNode);
			mpMoveRoute.push_back(mpPatrolPoints[mNextPatrolIndex]);
			// ���̖ړI�n�C���f�b�N�X��ݒ�
			mNextMoveIndex = 1;
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
		//����|�C���g�����߂�
		ChangePatrolPoint();
		mStateStep++;
		break;

		// �X�e�b�v1�F����|�C���g�܂ňړ�
	case 1:
	{
		// �ړ��A�j���[�V�������Đ�
		ChangeAnimation((int)EAnimType::eMove);

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
	// �������W�ւ̃x�N�g��
	CVector lookVec = mLookPos - Position();
	lookVec.Y(0.0f);
	// ��������
	CVector lookDir = lookVec.Normalized();
	// ���X�Ɍ��������֌�����
	CVector forward = CVector::Slerp
	(
		VectorZ(),	// ���݂̐��ʕ���
		lookDir,	// ��������
		ROTATE_SPEED * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

	// TODO�F�ړ���~�A�S�~�܂���ɐ����A�����_���ȕ����ɓ�����
	switch (mStateStep)
	{
		// �X�e�b�v0�F�S�~�܂𓊂���A�j���[�V�����Đ�
	case 0:
	{
		// �������W�������_���Ō���
		SetLookPosRandom();
		ChangeAnimation((int)EAnimType::eThrowBag);
		// �S�~�܂�L��
		if (!SpawnTrashBag())
		{
			// ������܂ł̎��Ԃ����Z�b�g
			SetThrowTime();
			// �����ł��Ȃ���Ώ����Ԃ�
			ChangeState(EState::ePatrol);
			return;
		}
		mpTrashBags[mTrashBagNum]->Position(Position() + TRASH_BAG_OFFSET_POSY +
			VectorZ() * TRASH_BAG_OFFSET_POSZ);
		mpTrashBags[mTrashBagNum]->SetGravity(false);
		mStateStep++;
		break;
	}
		// �X�e�b�v1�F�A�^�b�`�������Č����Ă�������ɓ�����
	case 1:
		mpTrashBags[mTrashBagNum]->Position(Position() + TRASH_BAG_OFFSET_POSY +
			VectorZ() * TRASH_BAG_OFFSET_POSZ);
		// �A�j���[�V������50%�i�s������
		if (GetAnimationFrameRatio() >= 0.2f)
		{
			// �d�͂��I��
			mpTrashBags[mTrashBagNum]->SetGravity(true);
			// �S�~�𓊂��鑬�x��ݒ�
			SetThrowSpeed();
			// �����̐��ʕ����ɓ����鑬�x��ݒ�
			CVector throwSpeed = VectorZ() * GetThrowSpeed();
			// ���ł������x��ݒ�
			mpTrashBags[mTrashBagNum]->SetThrowSpeed(throwSpeed, GetThrowSpeed());
			mStateStep++;
		}
		break;
		// �X�e�b�v2�F�A�j���[�V�������I��������
	case 2:
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;

		// �X�e�b�v3�F������܂ł̎��Ԃ����Z�b�g���ď����Ԃ�
	case 3:
		// ������܂ł̎��Ԃ����Z�b�g
		SetThrowTime();
		// �������̃S�~�܂̔ԍ������Z�b�g
		mTrashBagNum = -1;
		// �ړ��𒆒f���Ă���Ȃ�
		if (mIsMovePause)
		{
			// �����Ԃ�
			ChangeState(EState::ePatrol);
		}
		// ���Ă��Ȃ��Ȃ�
		else
		{
			// �ҋ@��Ԃ�
			ChangeState(EState::eIdle);
		}
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

// �������W�������_���Ɍ��肷��
void CResident::SetLookPosRandom()
{
	// �������擾
	int random = Math::Rand(0, 10);

	// �������W��ݒ�
	switch (random)
	{
	case 0:
		mLookPos = LOOK_POS0;
		break;
	case 1:
		mLookPos = LOOK_POS1;
		break;
	case 2:
		mLookPos = LOOK_POS2;
		break;
	case 3:
		mLookPos = LOOK_POS3;
		break;
	case 4:
		mLookPos = LOOK_POS4;
		break;
	case 5:
		mLookPos = LOOK_POS5;
		break;
	case 6:
		mLookPos = LOOK_POS6;
		break;
	case 7:
		mLookPos = LOOK_POS7;
		break;
	case 8:
		mLookPos = LOOK_POS8;
		break;
	case 9:
		mLookPos = LOOK_POS9;
		break;
	case 10:
		mLookPos = LOOK_POS10;
		break;
	}
}

// �S�~�܂̐���
void CResident::CreateTrashBag()
{
	// �������ő吔���쐬
	for (int i = 0; i < GetMaxThrowNum(); i++)
	{
		mpTrashBags.push_back(new CTrashBag(false));
		// �����ɂ��Ă���
		mpTrashBags.back()->SetOnOff(false);
	}
}

// �����ł���S�~�܂�L���ɂ���
bool CResident::SpawnTrashBag()
{
	// �S�~�܂̔ԍ��L���p
	int i = -1;
	// �S�ẴS�~�܂��m�F
	for (CTrashBag* trashBag : mpTrashBags)
	{
		i++;
		// �L���Ȃ玟��
		if (trashBag->IsEnable()) continue;
		// �S�[���h���Ȃ�S�[���h��
		if (GoldBag())
		{
			trashBag->SetGold(true);
		}
		// �����łȂ��Ȃ�ʏ�
		else
		{
			trashBag->SetGold(false);
		}
		// �L���ɂ���
		trashBag->SetOnOff(true);
		mTrashBagNum = i;
		return true;
	}
	return false;
}

// �S�~�܂��S�[���h���ۂ����v�Z
bool CResident::GoldBag()
{
	// 1����100�܂ł�100�̐����痐�����擾
	int random = Math::Rand(1, 100);

	// �S�[���h�m���ȉ��̒l�Ȃ�S�[���h
	if (random <= GetGoldChance())
	{
		return true;
	}
	// �����łȂ��Ȃ�ʏ�
	else
	{
		return false;
	}
}
