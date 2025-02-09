#include "CDeliveryEnemy.h"
#include "CModel.h"
#include "CColliderCapsule.h"

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
void CDeliveryEnemy::TakeDamage(int damage, CObjectBase* causer)
{
	// �_���[�W���󂯂Ă��鎞�͏������Ȃ�
	if (IsDamaging()) return;
	// �_���[�W���󂯂Ă���
	mIsDamage = true;
	// �_���[�W���󂯂�
	CCharaStatusBase::TakeDamage(damage, causer);
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
	CDebugPrint::Print("EnemyHp�F%d\n", GetHp());
#endif
}

// �Փˏ���
void CDeliveryEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ���N���X�̏Փˏ���
	CEnemyBase::Collision(self, other, hit);
}

// �`��
void CDeliveryEnemy::Render()
{
	mpModel->Render(Matrix());
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
		// �ړ���Ԃ�
		ChangeState(EState::eMove);
		return;
	}
}

// ���S�̍X�V����
void CDeliveryEnemy::UpdateDeath()
{
	mMoveSpeed = -VectorZ() * GetBaseMoveSpeed() * Times::DeltaTime();
	if (Position().Z() > DELETE_POSZ)
	{
		// ����
		SetEnable(false);
		SetShow(false);
	}
}

// ���S
void CDeliveryEnemy::Death()
{
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
