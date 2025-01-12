#include "CCar.h"
#include "CColliderCapsule.h"
#include "CModel.h"
#include "CNavNode.h"
#include "CTrashPlayer.h"
#include "CTrashEnemy.h"

#define CAR_HEIGHT		9.0f	// �Ԃ̍���
#define CAR_WIDTH		25.0f	// �Ԃ̕�
#define CAR_RADIUS		10.0f	// �Ԃ̔��a

// �m�[�h�̍��W
#define NODE_POS0	CVector( 15.0f,0.0f, 30.0f)
#define NODE_POS1	CVector(-15.0f,0.0f, 30.0f)
#define NODE_POS2	CVector(-15.0f,0.0f,-30.0f)
#define NODE_POS3	CVector( 15.0f,0.0f,-30.0f)

#define PATROL_NEAR_DIST 0.0f	// ����J�n���ɑI������鏄��|�C���g�̍ŒZ����
#define ROTATE_SPEED 6.0f	// ��]���x

CCar::CCar(CModel* model, const CVector& pos, const CVector& rotation, 
	ERoadType road, std::vector<CNavNode*> patrolPoints)
	: CVehicleBase(model, pos, rotation, road, patrolPoints)
	, CVehicleStatus()
	, mState(EState::eMove)
	, mStateStep(0)
	, mElapsedTime(0.0f)
{
	// �v���C���[�A�G�A�����ꏊ�A�ԗ��A�n�`
	// �ƏՓ˔��肷��{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eVehicle,
		CVector(0.0f, CAR_HEIGHT, CAR_WIDTH - CAR_RADIUS),
		CVector(0.0f, CAR_HEIGHT, -CAR_WIDTH + CAR_RADIUS),
		CAR_RADIUS, true
	);
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,ETag::eSpawnZone,
		ETag::eVehicle,ETag::eField, });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eSpawnZone,ELayer::eVehicle,
		ELayer::eField,ELayer::eWall,ELayer::eObject });

	// �o�H�T���p�̃R���C�_�\�쐬
	mpNavCol = new CColliderCapsule
	(
		this, ELayer::eNone,
		CVector(0.0f, CAR_HEIGHT, CAR_WIDTH * 1.2f - CAR_RADIUS),
		CVector(0.0f, CAR_HEIGHT, -CAR_WIDTH * 1.2f + CAR_RADIUS),
		CAR_RADIUS, true
	);
}

CCar::~CCar()
{

}

void CCar::Update()
{
	switch (mState)
	{
	// �ړ�
	case EState::eMove:			UpdateMove();		break;
	// ��~
	case EState::eStop:			UpdateStop();		break;
	// ��ꂽ
	case EState::eBroken:		UpdateBroken();		break;
	// �Ԑ��ύX
	case EState::eChangeRoad:	UpdateChangeRoad(); break;
	}

	CVehicleBase::Update();

	// �ړ����Ă���������́A���g���L���łȂ��ꍇ
	if (mIsMove || !IsEnable())
	{
		// �m�[�h�𖳌��ɂ���
		mpNode0->SetEnable(false);
		mpNode1->SetEnable(false);
		mpNode2->SetEnable(false);
		mpNode3->SetEnable(false);
	}
	// ���g���L���ňړ����Ă��Ȃ��ꍇ
	else
	{
		// �m�[�h��L���ɂ���
		mpNode0->SetEnable(true);
		mpNode1->SetEnable(true);
		mpNode2->SetEnable(true);
		mpNode3->SetEnable(true);
		// �m�[�h���ԗ����~�܂��Ă�����W�̎���ɐݒ肷��
		mpNode0->SetPos(Position() + NODE_POS0);
		mpNode1->SetPos(Position() + NODE_POS1);
		mpNode2->SetPos(Position() + NODE_POS2);
		mpNode3->SetPos(Position() + NODE_POS3);
	}

#if _DEBUG
	CDebugPrint::Print("CarState:%s\n", GetStateStr(mState).c_str());
#endif
}

// �Փˏ���
void CCar::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CVehicleBase::Collision(self, other, hit);

	// �{�̃R���C�_�\
	if (self == mpBodyCol)
	{
		// �Փ˂������肪�v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			// �ړ����Ă�����
			if (IsMove())
			{
				// �v���C���[�N���X���擾
				CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(other->Owner());

				// �������瑊��̕���
				CVector direction = player->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// ���肪�󂯂�m�b�N�o�b�N���x�ɁA
				// �������^����m�b�N�o�b�N���x���������瑊��̕����ɐݒ�
				player->SetKnockbackReceived(direction * GetKnockbackDealt());
				// �U���͕��̃_���[�W��^����
				player->TakeDamage(GetAttackPower(), this);

				// ��ꂽ��ԂɕύX
				ChangeState(EState::eBroken);
			}
		}
		// �Փ˂������肪�G�̏ꍇ
		else if (other->Layer() == ELayer::eEnemy)
		{	
			// �ړ����Ă�����
			if (IsMove())
			{
				// �G�N���X���擾
				CTrashEnemy* enemy = dynamic_cast<CTrashEnemy*>(other->Owner());

				// �������瑊��̕���
				CVector direction = enemy->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// ���肪�󂯂�m�b�N�o�b�N���x�ɁA
				// �������^����m�b�N�o�b�N���x���������瑊��̕����ɐݒ�
				enemy->SetKnockbackReceived(direction * enemy->GetKnockbackDealt());
				// �U���͕��̃_���[�W��^����
				enemy->TakeDamage(GetAttackPower(), this);

				// ��ꂽ��ԂɕύX
				ChangeState(EState::eBroken);
			}
		}
	}
}

// �ړ�����
void CCar::UpdateMove()
{
	// �S�Ă̏���|�C���g�ւ̈ړ����I����Ă��Ȃ��Ȃ�
	if (!mIsMoveEnd)
	{
		// �X�e�b�v���Ƃɏ�����؂�ւ���
		switch (mStateStep)
		{
			// �X�e�b�v0�F�ړ��J�n���̏���|�C���g�����߂�
		case 0:
			// ����|�C���g�����߂�
			ChangePatrolPoint();
			mStateStep++;
			break;

			// �X�e�b�v1�F����|�C���g�܂ňړ�
		case 1:
		{
			// �ŒZ�o�H�̎��̃m�[�h�܂ňړ�
			CNavNode* moveNode = mMoveRoute[mNextMoveIndex];

			if (MoveTo(moveNode->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
			{
				// �ړ����I���΁A���̃m�[�h�֐؂�ւ�
				mNextMoveIndex++;
				// �Ō�̃m�[�h�i�ړI�n�̃m�[�h�j�������ꍇ�́A���̃X�e�b�v�֐i�߂�
				if (mNextMoveIndex >= mMoveRoute.size())
				{
					mStateStep++;
				}
			}
			break;
		}
		// ���̏���|�C���g�����߂�
		case 2:
			ChangePatrolPoint();
			mStateStep = 1;
			break;
		}
	}
}

// ��~����
void CCar::UpdateStop()
{
	// �����Ă��Ȃ�
	mIsMove = false;
	// �ړ����x���[���ɂ���
	mMoveSpeed = CVector::zero;
}

// ��ꂽ����
void CCar::UpdateBroken()
{
	// �����Ă��Ȃ�
	mIsMove = false;
	// �ړ����x���[���ɂ���
	mMoveSpeed = CVector::zero;

	// ���ł���܂ł̎��Ԃ��J�E���g�_�E��
	CountDeleteTime();

	// ���ł܂ł̎��Ԃ��o�߂�����
	if (IsElapsedDeleteTime())
	{
		// ���ł܂ł̎��Ԃ������l�ɖ߂�
		SetDeleteTime();
		// ��Ԃ��ړ��ɖ߂��Ă���
		ChangeState(EState::eMove);

		// ��\��
		SetEnable(false);
		SetShow(false);
	}
}

// �Ԑ��ύX����
void CCar::UpdateChangeRoad()
{
	// �����Ă���
	mIsMove = true;
	bool isEnd = false;
	// �Ԑ��ύX�ړ�
	ChangeRoad(isEnd);

	// true�Ȃ�΁A�Ԑ��ύX���I�����
	if (isEnd)
	{
		// �ړ���Ԃɖ߂�
		ChangeState(EState::eMove);
	}
}

// ��Ԑ؂�ւ�
void CCar::ChangeState(EState state)
{
	// �����Ȃ珈�����Ȃ�
	if (state == mState) return;

	// �ړ�����j��ȊO�ɕύX�����Ƃ�
	// �ړ��̒��f��
	if (mState == EState::eMove&&
		state != EState::eBroken)
	{
		mIsMovePause = true;
	}

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

#if _DEBUG
// ��Ԃ̕�������擾
std::string CCar::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eMove:			return "�ړ���";
	case EState::eStop:			return "��~��";
	case EState::eBroken:		return "���Ă���";
	case EState::eChangeRoad:	return "�Ԑ��ύX";
	default:					return "";
	}
}
#endif
