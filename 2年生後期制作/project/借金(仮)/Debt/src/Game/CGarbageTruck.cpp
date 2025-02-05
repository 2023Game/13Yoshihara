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
#include "CGaugeUI3D.h"
#include "CFlamethrower.h"
#include "CTrashBag.h"
#include "Maths.h"
#include "CSound.h"


#define TRUCK_HEIGHT	13.0f	// �g���b�N�̍���
#define TRUCK_WIDTH		30.0f	// �g���b�N�̕�
#define TRUCK_RADIUS	15.0f	// �g���b�N�̔��a
// �{�̃R���C�_�̃I�t�Z�b�g���W
#define BODY_COL_OFFSET_POS CVector(0.0f,0.0f,-2.0f)

// �T�m�͈�
#define SEARCH_RADIUS	50.0f

// ����������̃I�t�Z�b�g���W
#define COLLECTOR_OFFSET_POS VectorX() * 10.0f

// �ԗ����m�̋����̍Œ�l
// �O���R���C�_�\�ɑ��̎ԗ�������Ƃ��ɂ�����߂��Ȃ�Β�~����
#define VEHICLE_DIST 40.0f

// �m�[�h�̍��W
#define NODE_POS0	CVector( 20.0f,0.0f, 35.0f)
#define NODE_POS1	CVector(-20.0f,0.0f, 35.0f)
#define NODE_POS2	CVector(-20.0f,0.0f,-35.0f)
#define NODE_POS3	CVector( 20.0f,0.0f,-35.0f)

#define ROTATE_SPEED 6.0f	// ��]���x

// ������J�n�ł���Z�̍��W�͈̔�
#define COLLECT_MAX_POSZ  200.0f
#define COLLECT_MIN_POSZ -200.0f

// Hp�Q�[�W�̃I�t�Z�b�g���W
#define GAUGE_OFFSET_POS CVector(0.0f,30.0f,0.0f)
// �ʏ��Hp�Q�[�W�̉摜�̃p�X
#define HP_GAUGE_PATH "UI\\garbageTruck_hp_gauge.png"

// �S�~�܂𗎂Ƃ��I�t�Z�b�g���W
#define TRASH_BAG_OFFSET_POS CVector(20.0f,5.0f,-30.0f)
// �S�~�܂𗎂Ƃ�X���W�̃����_���͈�
#define TRASH_BAG_RAND_POSX 20.0f
// �S�~�܂𗎂Ƃ��Ԋu
#define TRASH_BAG_DROP_TIME 0.5f

// ���ʉ��̉���
#define SE_VOLUME 0.5f


// �R���X�g���N�^
CGarbageTruck::CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation,
	ERoadType road, std::vector<CNavNode*> patrolPoints, bool punisher)
	: CVehicleBase(model, pos, rotation, road, patrolPoints)
	, CGarbageTruckStatus(punisher)
	, mState(EState::eMove)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsReturn(false)
	, mpSearchCol(nullptr)
{
	// �ʏ�̏ꍇ
	if(!punisher)
	{
		// �ʏ��Hp�Q�[�W��ݒ�
		mpHpGauge = new CGaugeUI3D(this, HP_GAUGE_PATH);
		mpHpGauge->SetMaxPoint(GetMaxHp());
		mpHpGauge->SetCurrPoint(GetHp());
		// �ŏ��͖���
		mpHpGauge->SetEnable(false);
		mpHpGauge->SetShow(false);

		// �v���C���[�T�m�p�R���C�_�\�쐬
		mpSearchCol = new CColliderSphere
		(
			this, ELayer::eCharaSearch,
			SEARCH_RADIUS,
			true
		);
		// �v���C���[�ƏՓ˔���
		mpSearchCol->SetCollisionTags({ ETag::ePlayer });
		mpSearchCol->SetCollisionLayers({ ELayer::ePlayer });

		int num = GetCollectorsNum();
		// �������S�Đ������A�����ɂ��Ă���
		for (int i = 0; i < num; i++)
		{
			mpCollectors.push_back(new CCollector(punisher, this,
				{ mpNode0,mpNode1,mpNode2,mpNode3 }));
			// �����ɂ���
			mpCollectors[i]->SetOnOff(false);
		}
	}

	// �{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eVehicle,
		CVector(0.0f,TRUCK_HEIGHT, TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f,TRUCK_HEIGHT,-TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS
	);
	// ���W��ݒ�
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// �v���C���[�A�G�A������A�U���A�ԗ��A�S�~���W�ԁA�ԗ��T�m�p�A�n�`�A�S�~��
	// �ƏՓ˔��肷��
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,
		ETag::eVehicle,ETag::eField,ETag::eTrashBag });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,ELayer::eCollector,ELayer::eAttackCol,
		ELayer::eSpawnZone,ELayer::eVehicle,ELayer::eVehicleSearch,
		ELayer::eGround,ELayer::eWall,ELayer::eObject,ELayer::eTrashBag });

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

	// �S�~�܂̐��̏����l��ݒ�
	SetTrashBag(GetDefaultBagNum());
	SetGoldTrashBag(GetDefaultGoldBagNum());
}

// �f�X�g���N�^
CGarbageTruck::~CGarbageTruck()
{
	// �R���C�_�[�̍폜
	SAFE_DELETE(mpSearchCol);

	// ���X�g�̃N���A
	mpCollectors.clear();

	// HP�Q�[�W�����݂�����A�ꏏ�ɍ폜����
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
	}
}

// �_���[�W���󂯂�
void CGarbageTruck::TakeDamage(int damage, CObjectBase* causer)
{
	// �_���[�W���󂯂�
	CCharaStatusBase::TakeDamage(damage, causer);
}

// �N���e�B�J���_���[�W���󂯂�
void CGarbageTruck::TakeCritical(int damage, CObjectBase* causer)
{
	// �U���͂�2�{�̃_���[�W
	int criticalDamage = damage * 2;
	// �_���[�W���󂯂�
	CCharaStatusBase::TakeDamage(criticalDamage, causer);
}

// �I�u�W�F�N�g�폜����
void CGarbageTruck::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�I�u�W�F�N�g��Hp�Q�[�W�ł����
	// �|�C���^����ɂ���
	if (obj == mpHpGauge)
	{
		mpHpGauge = nullptr;
	}
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

	// HP�Q�[�W���X�V
	mpHpGauge->Position(Position() + GAUGE_OFFSET_POS);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());

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

				if (player != nullptr)
				{
					// �U�����󂯂Ă���Ȃ珈�����Ȃ�
					if (player->IsDamaging()) return;
					// �������瑊��̕���
					CVector direction = player->Position() - Position();
					direction = direction.Normalized();
					direction.Y(0.0f);
					// ���肪�󂯂�m�b�N�o�b�N���x�ɁA
					// �������^����m�b�N�o�b�N���x���������瑊��̕����ɐݒ�
					player->SetKnockbackReceived(direction * GetKnockbackDealt());
					// �N���e�B�J���_���[�W��^����
					player->TakeCritical(GetAttackPower(), this, GetPower());
					// �N���e�B�J�������Đ�
					mpCriticalSE->Play(SE_VOLUME, true);
				}
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

				if (enemy != nullptr)
				{
					// �U�����󂯂Ă���Ȃ珈�����Ȃ�
					if (enemy->IsDamaging()) return;
					// �������瑊��̕���
					CVector direction = enemy->Position() - Position();
					direction = direction.Normalized();
					direction.Y(0.0f);
					// ���肪�󂯂�m�b�N�o�b�N���x�ɁA
					// �������^����m�b�N�o�b�N���x���������瑊��̕����ɐݒ�
					enemy->SetKnockbackReceived(direction * enemy->GetKnockbackDealt());
					// �N���e�B�J���_���[�W��^����
					enemy->TakeCritical(GetAttackPower(), this, GetPower());
					// �N���e�B�J�������Đ�
					mpCriticalSE->Play(SE_VOLUME, true);
				}
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

				if (collector != nullptr)
				{
					// �U���͕��̃_���[�W��^����
					collector->TakeDamage(GetAttackPower(), this);
				}
			}
		}
		// �Փ˂������肪�ԗ��̏ꍇ
		else if (other->Layer() == ELayer::eVehicle)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);
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
		// ���肪�v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			// �P�ޒ��łȂ����
			if (!mIsReturn)
			{
				CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
				// ������J�n�ł���ꏊ�ɂ��邩��
				// ��ꂽ��ԂłȂ�����
				// �v���C���[���G���A���Ȃ�
				if (CanCollectPosZ() &&
					mState != EState::eBroken&&
					!player->AreaOutX())
				{
					// �����Ԃ�
					ChangeState(EState::eCollect);
				}
			}
		}
	}
}

// �`��
void CGarbageTruck::Render()
{
	CVehicleBase::Render();
#if _DEBUG
	// �ړ���Ԃł����
	if (mState == EState::eMove)
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
		// ����|�C���g�܂ňړ��̃X�e�b�v�Ȃ�
		if (mStateStep == 1)
		{
			CVector offsetPos = CVector(0.0f, TRUCK_HEIGHT, 0.0f);
			CVector selfPos = Position() + offsetPos;
			CVector targetPos = mpMoveRoute[mNextMoveIndex]->GetPos() + offsetPos;
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
#endif
}

// �ԗ��̗L��������؂�ւ���
void CGarbageTruck::SetOnOff(bool setOnOff)
{
	CVehicleBase::SetOnOff(setOnOff);
	// �L���ɂ���ꍇ
	if (setOnOff)
	{
		// HP�Q�[�W���X�V
		mpHpGauge->Position(Position() + GAUGE_OFFSET_POS);
		mpHpGauge->SetMaxPoint(GetMaxHp());
		mpHpGauge->SetCurrPoint(GetHp());
	}

	mpHpGauge->SetEnable(setOnOff);
	mpHpGauge->SetShow(setOnOff);
	mpFlamethrower->Stop();
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

// ������ł���Z�͈͓̔��ɂ��邩�ǂ���
bool CGarbageTruck::CanCollectPosZ()
{
	// ������J�n�ł���Z�͈͓̔��Ȃ�true
	if (Position().Z() <= COLLECT_MAX_POSZ &&
		Position().Z() >= COLLECT_MIN_POSZ)
	{
		return true;
	}
	// �͈͊O�Ȃ�false
	return false;
}

// �ړ�����
void CGarbageTruck::UpdateMove()
{
	// ����ł���ꍇ
	if (IsDeath())
	{
		mElapsedTime -= Times::DeltaTime();
		// �S�~�܂𗎂Ƃ��Ԋu�̎��Ԃ��o�߂�����
		if (mElapsedTime <= TRASH_BAG_DROP_TIME)
		{
			// �S�~�܂𗎂Ƃ�
			DropTrashBag(1, Position(), VectorZ(), VectorX(), TRASH_BAG_OFFSET_POS);
			mElapsedTime = TRASH_BAG_DROP_TIME;
		}
	}
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
		// �X�e�b�v0�F�ݒ��ύX����
	case 0:
		// �����Ă��Ȃ�
		mIsMove = false;
		// �ړ����x���[���ɂ���
		mMoveSpeed = CVector::zero;
		// ���̃G�t�F�N�g�𔭐�������
		mpFlamethrower->Start();
		mStateStep++;
		break;

		// �X�e�b�v1�F�S�Ẳ������P��
	case 1:
	{
		int size = mpCollectors.size();
		// �S�Ẳ�����̏�Ԃ�P�ޏ�Ԃ�
		for (int i = 0; i < size; i++)
		{
			// �����Ȃ�X���[
			if (!mpCollectors[i]->IsEnable()) continue;
			// �P����ԂɕύX
			mpCollectors[i]->ChangeStateReturn();
		}
		// ���̃X�e�b�v��
		mStateStep++;

		break;
	}

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
			mpPatrolPoints = vehicleMgr->GetPatrolPoints(mRoadType);
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
	{
		// �����Ă��Ȃ�
		mIsMove = false;
		// �ړ����x���[���ɂ���
		mMoveSpeed = CVector::zero;
		// �P�ނ܂ł̎��Ԃ������l�ɖ߂�
		SetReturnTime();
		// ������̐l���������l�ɖ߂�
		SetCollectorsNum();

		int size = mpCollectors.size();
		// �������S�ėL���ɂ���
		for (int i = 0; i < size; i++)
		{
			// �L���ɂ���
			mpCollectors[i]->SetOnOff(true);
			// �����̍��W�{�I�t�Z�b�g���W��ݒ�
			mpCollectors[i]->Position(Position() + COLLECTOR_OFFSET_POS * (i + 1));
		}

		mStateStep++;
		break;
	}

	// �X�e�b�v1�F���Ԃ��o�߂���܂ŃJ�E���g�_�E��
	case 1:
		// �P�ނ܂ł̎��Ԃ��J�E���g�_�E������
		CountReturnTime();

		// �P�ނ܂ł̎��Ԃ��o�߂����玟�̃X�e�b�v��
		if (IsElapsedReturnTime())
		{
			int size = mpCollectors.size();
			// �S�Ẳ�����̏�Ԃ�P�ޏ�Ԃ�
			for (int i = 0; i < size; i++)
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

// ���S(��ꂽ)
void CGarbageTruck::Death()
{
	// ��ꂽ��Ԃ�
	ChangeState(EState::eBroken);
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

	// �ړ����瑼�̏�ԂɕύX���鎞
	// �ړ��̒��f��
	if (mState == EState::eMove)
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

