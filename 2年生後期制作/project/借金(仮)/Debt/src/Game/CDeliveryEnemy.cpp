#include "CDeliveryEnemy.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryField.h"

#define TRUCK_HEIGHT	13.0f	// �g���b�N�̍���
#define TRUCK_WIDTH		32.5f	// �g���b�N�̕�
#define TRUCK_RADIUS	12.5f	// �g���b�N�̔��a
// �{�̃R���C�_�\�̃I�t�Z�b�g���W
#define BODY_COL_OFFSET_POS CVector(0.0f,0.0f,-3.0f)

#define EYE_HEIGHT 5.0f		// ���_�̍���

#define ROTATE_SPEED 3.0f	// ��]���x

// �����̕���
#define ROTATION CVector(0.0f,180.0f,0.0f)

// �Ԑ��ύX���̎Ԑ��̃I�t�Z�b�g���W
#define CHANGE_ROAD_OFFSET_POS_L CVector(-40.0f,0.0f,-10.0f)
#define CHANGE_ROAD_OFFSET_POS_R CVector( 40.0f,0.0f,-10.0f)
// 臒l
#define CHANGE_ROAD_THRESHOLD 0.1f

// �e�ۂ̃I�t�Z�b�g���W
#define BULLET_OFFSET_POS_L  CVector(-20.0f,14.75f,10.0f)	// ��
#define BULLET_OFFSET_POS_R  CVector( 19.0f,14.75f,10.0f)	// �E
#define BULLET_OFFSET_POS_B1 CVector( 6.5f, 14.75f,41.0f)	// ���1
#define BULLET_OFFSET_POS_B2 CVector(-7.5f, 14.75f,41.0f)	// ���2
// �e�ۂ̕���
#define BULLET_ROT_LR	CVector(0.0f,90.0f,0.0f) // ���E

// �_���[�W��̖��G����
#define INVINCIBLE_TIME 1.0f
// �_�ŊԊu
#define HIT_FLASH_INTERVAL 0.25f

// ���ł���Z���W
#define DELETE_POSZ 300.0f

// �R���X�g���N�^
CDeliveryEnemy::CDeliveryEnemy()
	: CEnemyBase
	(
		0.0f,0.0f,
		{},
		EYE_HEIGHT
	)
	, CDeliveryEnemyStatus()
	, mInvincibleTime(0.0f)
	, mHitFlashTime(0.0f)

{
	// �ړ����
	ChangeState(EState::eMove);
	// ������ݒ�
	Rotation(ROTATION);
	// �ړ������������Ȃ�
	mIsMoveDir = false;
	// �d�͖���
	mIsGravity = false;
	mpModel = CResourceManager::Get<CModel>("DeliveryEnemy");

	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CDeliveryEnemy::~CDeliveryEnemy()
{
}

// �_���[�W���󂯂�
void CDeliveryEnemy::TakeDamage(int damage, CObjectBase* causer, bool isShot)
{
	// �_���[�W���󂯂Ă��鎞�͏������Ȃ�
	if (IsDamaging()) return;

	// ���S��ԂȂ�A�_���[�W���󂯂Ȃ�
	if (mState == EState::eDeath) return;

	// �����̎ˌ��ɂ�錸���ł͂Ȃ��ꍇ
	if (!isShot)
	{
		// �_���[�W���󂯂Ă���
		mIsDamage = true;
	}

	// Hp��0�̎��U�����󂯂��玀�S
	if (GetHp() == 0)
	{
		Death();
		return;
	}

	// Hp���_���[�W�����炷
	SetHp(-damage);
}

// �X�V
void CDeliveryEnemy::Update()
{
	switch (mState)
	{
	case EState::eMove:			UpdateMove();			break;
	case EState::eChangeRoad:	UpdateChangeRoad();		break;
	case EState::eDeath:		UpdateDeath();			break;
	}

	// �_���[�W���󂯂Ă�����_�ł���
	HitFlash();

	// ���N���X�̍X�V
	CEnemyBase::Update();

#if _DEBUG
	CDebugPrint::Print("EnemyState�F%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("EnemyHp�F%d\n", GetHp());
	CDebugPrint::Print("EnemyTargetPos�F%f,%f,%f\n", mTargetPos.X(), mTargetPos.Y(), mTargetPos.Z());
#endif
}

// �Փˏ���
void CDeliveryEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ���N���X�̏Փˏ���
	CEnemyBase::Collision(self, other, hit);
	// �{�̃R���C�_�\
	if (self == mpBodyCol)
	{
		// �v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(other->Owner());
			player->TakeDamage(GetAttackPower(), this);
		}
	}
}

// �`��
void CDeliveryEnemy::Render()
{
	mpModel->Render(Matrix());
}

// �L��������؂�ւ���
void CDeliveryEnemy::SetOnOff(bool setOnOff)
{
	// �L��������ݒ�
	SetEnable(setOnOff);
	SetShow(setOnOff);
}

// ���݂̎ԓ���ݒ�
void CDeliveryEnemy::SetRoadType(ERoadType roadType)
{
	mRoadType = roadType;
}

// ���݂̎ԓ����擾
ERoadType CDeliveryEnemy::GetRoadType() const
{
	return ERoadType();
}

// ��Ԑ؂�ւ�
void CDeliveryEnemy::ChangeState(EState state)
{
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

#if _DEBUG
// ��Ԃ̕�������擾
std::string CDeliveryEnemy::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eMove:			return "�ړ�";
	case EState::eChangeRoad:	return "�Ԑ��ύX";
	case EState::eDeath:		return "���S";
	}
	return "";
}
#endif

// �ړ��̍X�V����
void CDeliveryEnemy::UpdateMove()
{
	// �����铹��X���W
	float targetPosX = 0.0f;
	switch (mRoadType)
	{
	case ERoadType::eLeft1:
		targetPosX = ROAD_LEFT1_POSX;
		break;
	case ERoadType::eLeft2:
		targetPosX = ROAD_LEFT2_POSX;
		break;
	case ERoadType::eRight1:
		targetPosX = ROAD_RIGHT1_POSX;
		break;
	case ERoadType::eRight2:
		targetPosX = ROAD_RIGHT2_POSX;
		break;
	}
	// �g�p���Ȃ��̂�
	// �ړI�n�������ɐݒ肵�Ă���
	mTargetPos = CVector(targetPosX, Position().Y(), Position().Z());
	Position(mTargetPos);
}

// �Ԑ��ύX�̍X�V����
void CDeliveryEnemy::UpdateChangeRoad()
{
	mMoveSpeed = CVector::zero;

	// �ړI�n�ֈړ�
	MoveTo(mTargetPos, GetBaseMoveSpeed(), ROTATE_SPEED);

	// ������X���W��臒l���ɓ�������
	if (Position().X() <= mTargetPos.X() + CHANGE_ROAD_THRESHOLD &&
		Position().X() >= mTargetPos.X() - CHANGE_ROAD_THRESHOLD)
	{
		// �ړI�n�̎ԓ��̃^�C�v�ɕύX
		mRoadType = mTargetRoadType;
		// �ړ���Ԃ�
		ChangeState(EState::eMove);
		return;
	}
}

// ���S�̍X�V����
void CDeliveryEnemy::UpdateDeath()
{
	float moveSpeed = GetBaseMoveSpeed() * 0.2f * Times::DeltaTime();
	mMoveSpeed = -VectorZ() * moveSpeed;
	if (Position().Z() > DELETE_POSZ)
	{
		// ����
		SetOnOff(false);
	}
}

// ���S
void CDeliveryEnemy::Death()
{
	// �󂵂��G�̐��𑝂₷
	CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(CDeliveryPlayer::Instance());
	player->IncreaseDestroyEnemyNum();
	// �U�����󂯂Ă��Ȃ�
	mIsDamage = false;
	// ���S��Ԃ�
	ChangeState(EState::eDeath);
}

// �R���C�_�\�𐶐�
void CDeliveryEnemy::CreateCol()
{
	// �{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eEnemy,
		CVector(0.0f, TRUCK_HEIGHT, TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS
	);
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// �n�`�A�v���C���[�A��Q���A���˕��̍U������
	// �ƏՓ˔��������
	mpBodyCol->SetCollisionTags({ ETag::eField,ETag::ePlayer,
		ETag::eObstruction,ETag::eBullet });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,ELayer::ePlayer,
		ELayer::eObstruction,ELayer::eAttackCol });
}

// �w�肵���ʒu�܂ňړ�����
bool CDeliveryEnemy::MoveTo(const CVector& targetPos, float speed, float rotateSpeed)
{
	// �ړI�n�܂ł̃x�N�g�������߂�
	CVector pos = Position();
	CVector vec = targetPos - pos;
	vec.Y(0.0f);
	// �ړ������x�N�g�������߂�
	CVector moveDir = vec.Normalized();

	// ���X�Ɉړ������֌�����
	CVector forward = CVector::Slerp
	(
		VectorZ(),	// ���݂̐��ʕ���
		moveDir,	// �ړ�����
		rotateSpeed * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

	// ����̈ړ����������߂�
	float moveDist = speed * Times::DeltaTime();
	// �ړI�n�܂ł̎c��̋��������߂�
	float remainDist = vec.Length();
	// �c��̋������ړ��������Z��
	if (remainDist <= moveDist)
	{
		pos = CVector(targetPos.X(), pos.Y(), targetPos.Z());
		Position(pos);
		return true;	// �ړI�n�ɓ��������̂ŁAtrue��Ԃ�
	}

	// �c��̋������ړ�������蒷���ꍇ��
	// �ړ��������ړI�n�ֈړ�
	pos += moveDir * moveDist;
	Position(pos);

	// �ړI�n�ɂ͓������Ȃ�����
	return false;
}

// �_���[�W�̓_�łƖ��G���Ԃ̏���
void CDeliveryEnemy::HitFlash()
{
	if (IsDamaging())
	{
		// �_�ŊԊu���o�߂�����
		if (mHitFlashTime > HIT_FLASH_INTERVAL)
		{
			mHitFlashTime -= HIT_FLASH_INTERVAL;
			// �`�悷�邩�𔽓]
			SetShow(!IsShow());
		}
		// ���G���Ԃ��o�߂�����
		if (mInvincibleTime > INVINCIBLE_TIME)
		{
			// �`�悷��
			SetShow(true);
			// �Փ˔���L��
			SetEnableCol(true);
			// �_���[�W���󂯂Ă��Ȃ�
			mIsDamage = false;
			// �v���p�̕ϐ������Z�b�g
			mHitFlashTime = 0.0f;
			mInvincibleTime = 0.0f;
		}
		mHitFlashTime += Times::DeltaTime();
		mInvincibleTime += Times::DeltaTime();
	}
}
