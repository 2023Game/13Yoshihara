#include "CGarbageTruck.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CModel.h"
#include "CNavNode.h"
#include "CTrashPlayer.h"
#include "CTrashEnemy.h"
#include "Primitive.h"
#include "CVehicleManager.h"
#include "CCollector.h"


#define TRUCK_HEIGHT	13.0f	// �g���b�N�̍���
#define TRUCK_WIDTH		30.0f	// �g���b�N�̕�
#define TRUCK_RADIUS	15.0f	// �g���b�N�̔��a
// �{�̃R���C�_�̃I�t�Z�b�g���W
#define BODY_COL_OFFSET_POS CVector(0.0f,0.0f,-2.0f)

#define SEARCH_RADIUS	50.0f	// �T�m�͈�

// ����������̃I�t�Z�b�g���W
#define COLLECTOR_OFFSET_POS CVector(-10.0f,0.0f,0.0f)

// �ԗ����m�̋����̍Œ�l
// �O���R���C�_�\�ɑ��̎ԗ�������Ƃ��ɂ�����߂��Ȃ�Β�~����
#define VEHICLE_DIST 40.0f

// �m�[�h�̍��W
#define NODE_POS0	CVector( 20.0f,0.0f, 35.0f)
#define NODE_POS1	CVector(-20.0f,0.0f, 35.0f)
#define NODE_POS2	CVector(-20.0f,0.0f,-35.0f)
#define NODE_POS3	CVector( 20.0f,0.0f,-35.0f)

#define PATROL_NEAR_DIST 0.0f	// ����J�n���ɑI������鏄��|�C���g�̍ŒZ����
#define ROTATE_SPEED 3.0f	// ��]���x

// �R���X�g���N�^
CGarbageTruck::CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation,
	ERoadType road, std::vector<CNavNode*> patrolPoints, bool punisher)
	: CVehicleBase(model, pos, rotation, road, patrolPoints)
	, CGarbageTruckStatus(punisher)
	, mState(EState::eMove)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsReturn(false)
{
	// �{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eVehicle,
		CVector(0.0f,TRUCK_HEIGHT, TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f,TRUCK_HEIGHT,-TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS, true
	);
	// ���W��ݒ�
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// �v���C���[�A�G�A������A�����ꏊ�A�ԗ��A�ԗ��T�m�p�A�n�`
	// �ƏՓ˔��肷��
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,ETag::eSpawnZone,
		ETag::eVehicle,ETag::eField, });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,ELayer::eCollector,ELayer::eAttackCol,
		ELayer::eSpawnZone,ELayer::eVehicle,ELayer::eVehicleSearch,
		ELayer::eGround,ELayer::eWall,ELayer::eObject });

	// �O�����R���C�_�\
	mpFrontCol = new CColliderCapsule
	(
		this, ELayer::eVehicleSearch,
		CVector(0.0f, TRUCK_HEIGHT,  TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS, true
	);
	// ���W��ݒ�
	mpFrontCol->Position(FRONT_COL_OFFSET_POS);
	// �ԗ��ƏՓ˔��肷��
	mpFrontCol->SetCollisionTags({ ETag::eVehicle });
	mpFrontCol->SetCollisionLayers({ ELayer::eVehicle });

	// �������R���C�_�\
	// ����蒷���ݒ肵�Ă���
	mpSideCol = new CColliderCapsule
	(
		this, ELayer::eVehicleSearch,
		CVector(0.0f, TRUCK_HEIGHT,  TRUCK_WIDTH - TRUCK_RADIUS * 5.0f),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH + TRUCK_RADIUS * 5.0f),
		TRUCK_RADIUS, true
	);
	// ������̎ԓ������ɂ���ԓ�
	if (mRoadType == ERoadType::eLeft2 ||
		mRoadType == ERoadType::eRight1)
	{
		// ���W��ݒ�
		mpSideCol->Position(LEFT_COL_OFFSET_POS);
	}
	// ������̎ԓ����E�ɂ���ԓ�
	else
	{
		// ���W��ݒ�
		mpSideCol->Position(RIGHT_COL_OFFSET_POS);
	}
	// �ԗ��ƏՓ˔��肷��
	mpSideCol->SetCollisionTags({ ETag::eVehicle });
	mpSideCol->SetCollisionLayers({ ELayer::eVehicle });

	// �o�H�T���p�̃R���C�_�\�쐬
	mpNavCol = new CColliderCapsule
	(
		this, ELayer::eNone,
		CVector(0.0f, TRUCK_HEIGHT, TRUCK_WIDTH * 1.2f - TRUCK_RADIUS),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH * 1.2f + TRUCK_RADIUS),
		TRUCK_RADIUS, true
	);

	// �T�m�p�R���C�_�\�쐬
	mpSearchCol = new CColliderSphere
	(
		this,ELayer::eCharaSearch,
		SEARCH_RADIUS,
		true
	);
	// �v���C���[�ƏՓ˔���
	mpSearchCol->SetCollisionTags({ ETag::ePlayer});
	mpSearchCol->SetCollisionLayers({ ELayer::ePlayer});

	// �������S�Đ������A�����ɂ��Ă���
	for (int i = 0; i < GetCollectorsNum(); i++)
	{
		mpCollectors.push_back(new CCollector(false, this,
			{ mpNode0,mpNode1,mpNode2,mpNode3 }));
		// �����ɂ���
		mpCollectors[i]->SetOnOff(false);
	}
}

// �f�X�g���N�^
CGarbageTruck::~CGarbageTruck()
{
	// �R���C�_�[�̍폜
	SAFE_DELETE(mpSearchCol);

	// ���X�g�̃N���A
	mpCollectors.clear();
}

// �X�V
void CGarbageTruck::Update()
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
	case EState::eChangeRoad:	UpdateChangeRoad();	break;
	// ���
	case EState::eCollect:		UpdateCollect();	break;
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

	std::string roadType;
	if (mRoadType == ERoadType::eLeft1)
		roadType = "L1";
	else if (mRoadType == ERoadType::eLeft2)
		roadType = "L2";
	else if (mRoadType == ERoadType::eRight1)
		roadType = "R1";
	else
		roadType = "R2";
	CDebugPrint::Print("TruckState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("TruckRoadType:%s\n" ,roadType.c_str());
#endif
}

// �Փˏ���
void CGarbageTruck::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CVehicleBase::Collision(self, other, hit);

	// �{�̃R���C�_�\
	if (self == mpBodyCol)
	{
		// �Փ˂������肪�v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			// �������ړ����Ă�����
			if (IsMove())
			{
				// �v���C���[�N���X���擾
				CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(other->Owner());

				// �U�����󂯂Ă���Ȃ珈�����Ȃ�
				if (player->IsDamaging()) return;
				// �������瑊��̕���
				CVector direction = player->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// ���肪�󂯂�m�b�N�o�b�N���x�ɁA
				// �������^����m�b�N�o�b�N���x���������瑊��̕����ɐݒ�
				player->SetKnockbackReceived(direction * GetKnockbackDealt());
				// �U���͕��̃_���[�W��^����
				player->TakeDamage(GetAttackPower(), this);
			}
		}
		// �Փ˂������肪�G�̏ꍇ
		else if (other->Layer() == ELayer::eEnemy)
		{
			// �������ړ����Ă�����
			if (IsMove())
			{
				// �G�N���X���擾
				CTrashEnemy* enemy = dynamic_cast<CTrashEnemy*>(other->Owner());

				// �U�����󂯂Ă���Ȃ珈�����Ȃ�
				if (enemy->IsDamaging()) return;
				// �������瑊��̕���
				CVector direction = enemy->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// ���肪�󂯂�m�b�N�o�b�N���x�ɁA
				// �������^����m�b�N�o�b�N���x���������瑊��̕����ɐݒ�
				enemy->SetKnockbackReceived(direction * enemy->GetKnockbackDealt());
				// �U���͕��̃_���[�W��^����
				enemy->TakeDamage(GetAttackPower(), this);
			}
		}
		// �Փ˂������肪������̏ꍇ
		else if (other->Layer() == ELayer::eCollector)
		{
			// �������ړ����Ă�����
			if (IsMove())
			{
				// ������N���X���擾
				CCollector* collector = dynamic_cast<CCollector*>(other->Owner());

				if (collector != nullptr &&
					!IsAttackHitObj(collector))
				{
					AddAttackHitObj(collector);
					// �U���͕��̃_���[�W��^����
					collector->TakeDamage(GetAttackPower(), this);
				}
			}
		}
		// �ԗ��ƂԂ�������~�܂�
		else if (other->Layer() == ELayer::eVehicle)
		{
			// �������ړ����Ă�����
			if (IsMove())
			{
				// �~�܂��Ԃֈڍs
				ChangeState(EState::eStop);
			}
		}
		//TODO�F�e�X�g�p
		else if (other->Layer() == ELayer::eAttackCol)
		{
			// ��ꂽ��Ԃֈڍs
			ChangeState(EState::eBroken);
		}
	}
	// �O���R���C�_�\
	else if (self == mpFrontCol)
	{
		// ���肪�ԗ��̏ꍇ
		if (other->Layer() == ELayer::eVehicle)
		{
			// �ԗ��N���X���擾
			CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());

			// ����Ǝ����̍��W�̋��������߂�
			float dist = CVector::Distance(vehicle->Position(), Position());

			// ���肪�����Ă��Ȃ������Ă��Ȃ��ꍇ
			if (!vehicle->IsMove()&&
				!vehicle->IsBroken())
			{
				mIsFrontVehicle = true;
				// �������Ɏԗ������Ȃ�����
				// �O���̎Ԃ̂���ɑO�ɒ�~���Ă���ԗ����Ȃ��ꍇ
				if (!mIsSideVehicle&&
					!vehicle->GetIsFrontVehicle())
				{
					// �Ԑ��ύX�̈ړ����ݒ�
					SetChangeRoadPoint(vehicle);
					// �Ԑ��ύX��Ԃֈڍs
					ChangeState(EState::eChangeRoad);
				}
				// �������Ɏԗ�������Ȃ�
				else
				{
					// ��~��Ԃֈڍs
					ChangeState(EState::eStop);
				}
			}
			// ���肪�����Ă��Ȃ������Ă���ꍇ
			else if (!vehicle->IsMove() &&
				vehicle->IsBroken())
			{
				mIsFrontVehicle = true;
				// ���Ȃ��Ȃ�ΐi�߂�̂ł���܂Œ�~
				ChangeState(EState::eStop);
			}
			// ���肪�����Ă��Ă��߂��ꍇ�͒�~����
			else if (vehicle->IsMove() &&
				dist < VEHICLE_DIST)
			{
				mIsFrontVehicle = true;
				// ��~��Ԃֈڍs
				ChangeState(EState::eStop);
			}
		}
	}
	// �������R���C�_�\
	else if (self == mpSideCol)
	{
		// ���肪�ԗ��̏ꍇ
		if (other->Layer() == ELayer::eVehicle)
		{
			mIsSideVehicle = true;
		}
	}
	// �T�m�p�R���C�_�\
	else if (self == mpSearchCol)
	{
		// �P�ޒ��łȂ����
		if (!mIsReturn)
		{
			// ���肪�v���C���[�̏ꍇ
			if (other->Layer() == ELayer::ePlayer)
			{
				// �����Ԃ�
				ChangeState(EState::eCollect);
			}
		}
	}
}

// �`��
void CGarbageTruck::Render()
{
	CVehicleBase::Render();
	// �ړ���Ԃł����
	if (mState == EState::eMove)
	{
		// ����|�C���g��S�ĕ`��
		int size = mPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawWireBox
			(
				mPatrolPoints[i]->GetPos() + CVector(0.0f, 1.0f, 0.0f),
				CVector::one,
				c
			);
		}
		// ����|�C���g�܂ňړ��̃X�e�b�v�Ȃ�
		if (mStateStep == 1)
		{
			CVector offsetPos = CVector(0.0f, TRUCK_HEIGHT, 0.0f);
			CVector selfPos = Position() + offsetPos;
			CVector targetPos = mMoveRoute[mNextMoveIndex]->GetPos() + offsetPos;
			// �����񂷂�|�C���g�܂ŗΐ���`��
			Primitive::DrawLine
			(
				selfPos, targetPos,
				CColor::green,
				2.0f
			);
		}
	}
	// �Ԑ��ύX��Ԃ̏ꍇ
	else if (mState == EState::eChangeRoad)
	{
		// �Ԑ��ύX�ňړ�����|�C���g��`��
		Primitive::DrawWireBox
		(
			mpChangeRoadPoint->GetPos() + CVector(0.0f, 1.0f, 0.0f),
			CVector::one,
			CColor::gray
		);
		CVector offsetPos = CVector(0.0f, TRUCK_HEIGHT, 0.0f);
		CVector selfPos = Position() + offsetPos;
		CVector targetPos = mpChangeRoadPoint->GetPos() + offsetPos;
		// �Ԑ��ύX�̈ړ���|�C���g�܂Ő���`��
		Primitive::DrawLine
		(
			selfPos, targetPos,
			CColor::blue,
			2.0f
		);
	}
}

// �ϐ������Z�b�g
void CGarbageTruck::Reset()
{
	CVehicleBase::Reset();

	mStateStep = 0;
	mElapsedTime = 0.0f;
	mIsReturn = false;

	mState = EState::eMove;
}

// �ړ�����
void CGarbageTruck::UpdateMove()
{
	// �����Ă���
	mIsMove = true;
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
void CGarbageTruck::UpdateStop()
{
	switch (mStateStep)
	{
		// �ݒ��ύX����
	case 0:
		// �����Ă��Ȃ�
		mIsMove = false;
		// �ړ����x���[���ɂ���
		mMoveSpeed = CVector::zero;
		mStateStep++;
		break;

		// �O�Ɏԗ������Ȃ��Ȃ�����ړ���Ԃ�
	case 1:
		// �O�Ɏԗ������Ȃ��Ȃ�
		if (!mIsFrontVehicle)
		{
			// �ړ���Ԃ�
			ChangeState(EState::eMove);
		}
		break;
	}
}

// ��ꂽ����
void CGarbageTruck::UpdateBroken()
{
	switch (mStateStep)
	{
		// �ݒ��ύX����
	case 0:
		// �����Ă��Ȃ�
		mIsMove = false;
		// �ړ����x���[���ɂ���
		mMoveSpeed = CVector::zero;
		// ���ł܂ł̎��Ԃ������l�ɖ߂�
		SetDeleteTime();
		mStateStep++;
		break;

		// ���Ԃ��o�߂���܂ŃJ�E���g�_�E��
	case 1:
		// ���ł���܂ł̎��Ԃ��J�E���g�_�E��
		CountDeleteTime();

		// ���ł܂ł̎��Ԃ��o�߂�����
		if (IsElapsedDeleteTime())
		{
			// ���̃X�e�b�v��
			mStateStep++;
		}
		break;

		// ���ł���
	case 2:
		// ��\��
		SetEnable(false);
		SetShow(false);
		break;
	}
}

// �Ԑ��ύX����
void CGarbageTruck::UpdateChangeRoad()
{
	switch (mStateStep)
	{
	// �ړ����Ă��邱�Ƃ�ݒ�
	case 0:
		mIsMove = true;
		mStateStep++;
		break;

	// �Ԑ��ύX�̈ړ���̃m�[�h�܂ňړ�
	case 1:
		// �ړ����I�������
		if (MoveTo(mpChangeRoadPoint->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{
			CVehicleManager* vehicleMgr = CVehicleManager::Instance();
			if (vehicleMgr == nullptr) return;

			/*
			�ǂ̓��ɂ��邩�̏�Ԃ̕ύX
			*/
			// ������1�Ԗڂ̓��̏ꍇ
			if (mRoadType == ERoadType::eLeft1)
			{
				// ������2�Ԗڂ̓��Ɉړ������̂ŕύX
				ChangeRoadType(ERoadType::eLeft2);
			}
			// ������2�Ԗڂ̓��̏ꍇ
			else if (mRoadType == ERoadType::eLeft2)
			{
				// ������1�Ԗڂ̓��Ɉړ������̂ŕύX
				ChangeRoadType(ERoadType::eLeft1);
			}
			// �E����1�Ԗڂ̓��̏ꍇ
			else if (mRoadType == ERoadType::eRight1)
			{
				// ������2�Ԗڂ̓��Ɉړ������̂ŕύX
				ChangeRoadType(ERoadType::eRight2);
			}
			// �E����2�Ԗڂ̓��̏ꍇ
			else
			{
				// ������2�Ԗڂ̓��Ɉړ������̂ŕύX
				ChangeRoadType(ERoadType::eRight1);
			}

			// ����|�C���g�̃��X�g��ύX���ꂽ���̂��̂ɕύX����
			mPatrolPoints = vehicleMgr->GetPatrolPoints(mRoadType);
			mStateStep++;
		}
		break;

	// �Ԑ��ύX���I��
	case 2:
		// �I������̂ňړ���ԂɈڍs
		ChangeState(EState::eMove);
		break;
	}
}


// �������
void CGarbageTruck::UpdateCollect()
{
	switch (mStateStep)
	{
		// �X�e�b�v0�F�ݒ��ύX����
	case 0:
		// �����Ă��Ȃ�
		mIsMove = false;
		// �ړ����x���[���ɂ���
		mMoveSpeed = CVector::zero;
		// �P�ނ܂ł̎��Ԃ������l�ɖ߂�
		SetReturnTime();
		// ������̐l���������l�ɖ߂�
		SetCollectorsNum();

		// �������S�ėL���ɂ���
		for (int i = 0; i < mpCollectors.size(); i++)
		{
			// �L���ɂ���
			mpCollectors[i]->SetOnOff(true);
			// �����̍��W�{�I�t�Z�b�g���W��ݒ�
			mpCollectors[i]->Position(Position() + COLLECTOR_OFFSET_POS);
		}

		mStateStep++;
		break;

		// �X�e�b�v1�F���Ԃ��o�߂���܂ŃJ�E���g�_�E��
	case 1:
		// �P�ނ܂ł̎��Ԃ��J�E���g�_�E������
		CountReturnTime();

		// �P�ނ܂ł̎��Ԃ��o�߂����玟�̃X�e�b�v��
		if (IsElapsedReturnTime())
		{
			// �S�Ẳ�����̏�Ԃ�P�ޏ�Ԃ�
			for (int i = 0; i < mpCollectors.size(); i++)
			{
				// �����Ȃ�X���[
				if (!mpCollectors[i]->IsEnable()) continue;
				// �P����ԂɕύX
				mpCollectors[i]->ChangeStateReturn();
			}

			mStateStep++;
		}
		// �������́A��������S�ł����玟�̃X�e�b�v��
		else if (GetCollectorsNum() <= 0)
		{
			mStateStep++;
		}
		break;

		// �X�e�b�v2�F����������Ȃ��Ȃ�܂őҋ@
	case 2:

		// ������̐����[���Ȃ�
		if (GetCollectorsNum() <= 0)
		{
			// �����̓P�ނ��J�n
			// �P�ނ̈ړ��ł���
			mIsReturn = true;
			// �ړ���Ԃ�
			ChangeState(EState::eMove);
		}
		break;
	}
}

// ��Ԑ؂�ւ�
void CGarbageTruck::ChangeState(EState state)
{
	// �����Ȃ珈�����Ȃ�
	if (state == mState) return;
	// ��ꂽ��Ԃ���Ԑ��ύX�A�~�܂�ɂ͕ύX���Ȃ�
	if ((state == EState::eChangeRoad || state == EState::eStop) &&
		mState == EState::eBroken)
	{
		return;
	}

	// �ړ�����j��ȊO�ɕύX�����Ƃ�
	// �ړ��̒��f��
	if (mState == EState::eMove &&
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
std::string CGarbageTruck::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eMove:			return "�ړ���";
	case EState::eStop:			return "��~��";
	case EState::eBroken:		return "���Ă���";
	case EState::eChangeRoad:	return "�Ԑ��ύX";
	case EState::eCollect:		return "�����";
	default:					return "";
	}
}
#endif

