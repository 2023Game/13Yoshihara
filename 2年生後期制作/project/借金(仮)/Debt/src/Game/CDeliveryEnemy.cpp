#include "CDeliveryEnemy.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryField.h"
#include "CDeliveryObstruction.h"
#include "CDeliveryItem.h"
#include "CDeliveryFieldItem.h"
#include "CDeliveryHpUI3D.h"

#define TRUCK_HEIGHT	13.0f	// �g���b�N�̍���
#define TRUCK_WIDTH		32.5f	// �g���b�N�̕�
#define TRUCK_RADIUS	12.5f	// �g���b�N�̔��a
// �{�̃R���C�_�\�̃I�t�Z�b�g���W
#define BODY_COL_OFFSET_POS CVector(0.0f,0.0f,-3.0f)

// �T�m�R���C�_�[
#define SEARCH_HEIGHT	13.0f	// ����
#define SEARCH_WIDTH	32.5f*5.0f	// ��
#define SEARCH_RADIUS	12.5f*2.0f	// ���a
// �T�m�R���C�_�[�̃I�t�Z�b�g���W
#define SEARCH_OFFSET_POS CVector(0.0f,0.0f,50.0f)

#define EYE_HEIGHT 5.0f		// ���_�̍���

#define ROTATE_SPEED 3.0f	// ��]���x

// �����̕���
#define ROTATION CVector(0.0f,180.0f,0.0f)

// �Ԑ��ύX���̎Ԑ��̃I�t�Z�b�g���W
#define CHANGE_ROAD_OFFSET_POS_L CVector(-40.0f,0.0f,-10.0f)
#define CHANGE_ROAD_OFFSET_POS_R CVector( 40.0f,0.0f,-10.0f)
// �Ԑ��ύX����臒l
#define CHANGE_ROAD_THRESHOLD 0.1f

// �e�ۂ̃I�t�Z�b�g���W
#define BULLET_OFFSET_POS_L  CVector(-20.0f,14.75f,10.0f)	// ��
#define BULLET_OFFSET_POS_R  CVector( 19.0f,14.75f,10.0f)	// �E
#define BULLET_OFFSET_POS_B  CVector( 0.0f, 14.75f,41.0f)	// ���
// �e�ۂ̕���
#define BULLET_ROT_LR	CVector(0.0f,90.0f,0.0f) // ���E

// �_���[�W��̖��G����
#define INVINCIBLE_TIME 0.5f
// �_�ŊԊu
#define HIT_BLINK_INTERVAL 0.1f
// ���Ă�Ԋu
#define SHOOT_INTERVAL 0.5f

// ���ł���Z���W
#define DELETE_POSZ 300.0f

// �ړ��̊Ԋu�̎���
#define MOVE_INTERVAL_TIME 0.4f
// �v���C���[�����ɂ��邩��臒l
#define SIDE_PLAYER_THRESHOLD 50.0f

// �G�̒�ʒu��Z���W
#define ENEMY_POSZ -50.0f

// Hp�Q�[�W�̃I�t�Z�b�g���W
#define HP_UI_OFFSET_POS CVector(0.0f,30.0f*2.0f,0.0f)

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
	, mHitBlinkTime(0.0f)
	, mLeftShootTime(0.0f)
	, mRightShootTime(0.0f)
	, mBackShootTime(0.0f)
	, mIsLeft1Danger(false)
	, mIsLeft2Danger(false)
	, mIsRight1Danger(false)
	, mIsRight2Danger(false)
	, mIsLeft1Item(false)
	, mIsLeft2Item(false)
	, mIsRight1Item(false)
	, mIsRight2Item(false)

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

	// HpUI�𐶐�
	mpHpUI = new CDeliveryHpUI3D(this);
	// ���݂�HP��ݒ�
	mpHpUI->SetCurrPoint(GetHp());
	// ����
	mpHpUI->SetEnable(false);
	mpHpUI->SetShow(false);

	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CDeliveryEnemy::~CDeliveryEnemy()
{
	SAFE_DELETE(mpSearchCol);

	// HpUI�����݂�����A�ꏏ�ɍ폜����
	if (mpHpUI != nullptr)
	{
		mpHpUI->SetOwner(nullptr);
		mpHpUI->Kill();
	}
}

// �I�u�W�F�N�g�폜����
void CDeliveryEnemy::DeleteObject(CObjectBase* obj)
{
	CEnemyBase::DeleteObject(obj);
	if (obj == mpHpUI)
	{
		mpHpUI = nullptr;
	}
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
	HitBlink();

	// �ړ����Ԑ��ύX��ԂȂ�
	if (mState == EState::eMove ||
		mState == EState::eChangeRoad)
	{
		// �����𖞂������Ƃ��ˌ�
		Shoot();
	}

	// �����댯�������Z�b�g
	SetRoadDanger(ERoadType::eLeft1, false);
	SetRoadDanger(ERoadType::eLeft2, false);
	SetRoadDanger(ERoadType::eRight1, false);
	SetRoadDanger(ERoadType::eRight2, false);
	// ���ɃA�C�e�������邩�����Z�b�g
	SetRoadItem(ERoadType::eLeft1, false);
	SetRoadItem(ERoadType::eLeft2, false);
	SetRoadItem(ERoadType::eRight1, false);
	SetRoadItem(ERoadType::eRight2, false);

	// ���N���X�̍X�V
	CEnemyBase::Update();

	// HPUI���X�V
	mpHpUI->Position(Position() + HP_UI_OFFSET_POS);
	mpHpUI->SetCurrPoint(GetHp());

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
	// �T�m�R���C�_�\
	else if (self == mpSearchCol)
	{
		// ��Q���̏ꍇ
		if (other->Layer() == ELayer::eObstruction)
		{
			// ��Q���̎擾
			CDeliveryObstruction* obstruction = dynamic_cast<CDeliveryObstruction*>(other->Owner());
			// ���݂��铹
			ERoadType roadType = obstruction->GetRoadType();
			// ���݂��铹���댯
			SetRoadDanger(roadType, true);
		}
		// �A�C�e���̏ꍇ
		else if (other->Layer() == ELayer::eItem)
		{
			// �t�B�[���h�A�C�e���N���X�̎擾
			CDeliveryFieldItem* fieldItem = dynamic_cast<CDeliveryFieldItem*>(other->Owner());
			// ���݂��铹
			ERoadType roadType = fieldItem->GetRoadType();
			// ���ɃA�C�e�������邱�Ƃ�ݒ�
			SetRoadItem(roadType, true);
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
	// �ړ���Ԃɖ߂�
	ChangeState(EState::eMove);
	// Hp�����Z�b�g
	ResetHp();
	// HpUI�̈ʒu��ݒ�
	mpHpUI->Position(Position() + HP_UI_OFFSET_POS);
	// HpUI�̗L��������ݒ�
	mpHpUI->SetEnable(setOnOff);
	mpHpUI->SetShow(setOnOff);
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

	// �v���C���[�擾
	CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(CDeliveryPlayer::Instance());
	// �v���C���[�̍��W
	CVector playerPos = player->Position();
	// �v���C���[�̂��铹
	ERoadType playerRoadType = player->GetRoadType();
	// ���������A�v���C���[�̕������ɂ��邩�A
	// �G���A�����A�_���[�W���󂯂Ă��Ȃ��ꍇ
	if (mRoadType == playerRoadType &&
		Position().Z() > playerPos.Z() &&
		playerPos.Z() >= ROAD_Z_AREA_M &&
		!player->IsDamaging())
	{
		// �v���C���[��ړI�n�ɂ���
		mTargetPos = CVector(targetPosX, Position().Y(), playerPos.Z());
	}
	// ����ȊO��
	else
	{
		// �ړI�n�͒�ʒu
		mTargetPos = CVector(targetPosX, Position().Y(), ENEMY_POSZ);
	}
	// �ړI�n�ֈړ�
	// �ړ������͌����Ȃ�
	if (MoveTo(mTargetPos, GetBaseMoveSpeed(), 0.0f))
	{

	}

	CVector forward = CVector::Slerp(VectorZ(), CVector::back, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// �o�ߎ��Ԃ��v�Z
	mElapsedTime += Times::DeltaTime();

	// �����铹���댯�Ȃ�Ԑ��ύX����
	if (GetNowRoadDanger())
	{
		switch (mRoadType)
		{
		case ERoadType::eLeft1:
			// �E�ֈړ�
			GetTargetPos(false);
			break;
		case ERoadType::eLeft2:
			// ���̓����댯�łȂ��ꍇ
			if (!GetRoadDanger(ERoadType::eLeft1))
			{
				// ���ֈړ�
				GetTargetPos(true);
			}
			// ����ȊO�E�ֈړ�
			else
			{
				// �E�ֈړ�
				GetTargetPos(false);
			}
			break;
		case ERoadType::eRight1:
			// ���ֈړ�
			GetTargetPos(true);
			break;
		case ERoadType::eRight2:
			// �E�̓����댯�łȂ��ꍇ
			if (!GetRoadDanger(ERoadType::eRight1))
			{
				// �E�ֈړ�
				GetTargetPos(false);
			}
			// ����ȊO�E�ֈړ�
			else
			{
				// ���ֈړ�
				GetTargetPos(true);
			}
			break;
		}
		// �Ԑ��ύX��Ԃ�
		ChangeState(EState::eChangeRoad);
		return;
	}

	// ���E�ɃA�C�e��������A���S�̏ꍇ�͎Ԑ���ύX����
	ChangeRoadToItem();
}

// �Ԑ��ύX�̍X�V����
void CDeliveryEnemy::UpdateChangeRoad()
{
	mMoveSpeed = CVector::zero;
	// �ړI�n��X���W
	float targetPosX = 0.0f;
	switch (mTargetRoadType)
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
	// �ړI�n��ݒ�
	mTargetPos = CVector(targetPosX, Position().Y(), Position().Z());

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
	// ��������������
	CVector forward = CVector::Slerp(VectorZ(), CVector::back, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));
	// ��O�ֈړ�
	float moveSpeed = GetBaseMoveSpeed() * 0.2f * Times::DeltaTime();
	mMoveSpeed = CVector::forward * moveSpeed;
	// �폜���W�𒴂�����
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

// ����ł��邩�ǂ���
bool CDeliveryEnemy::IsDeath() const
{
	// ���S��Ԃ̏ꍇ
	if (mState == EState::eDeath)
	{
		return true;
	}
	return false;
}

// �����댯����ݒ�
void CDeliveryEnemy::SetRoadDanger(ERoadType roadType, bool danger)
{
	switch (roadType)
	{
	case ERoadType::eLeft1:
		mIsLeft1Danger = danger;
		break;
	case ERoadType::eLeft2:
		mIsLeft2Danger = danger;
		break;
	case ERoadType::eRight1:
		mIsRight1Danger = danger;
		break;
	case ERoadType::eRight2:
		mIsRight2Danger = danger;
		break;
	}
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
	// �n�`�A�v���C���[�A��Q���A���˕��̍U������A�A�C�e��
	// �ƏՓ˔��������
	mpBodyCol->SetCollisionTags({ ETag::eField,ETag::ePlayer,
		ETag::eObstruction,ETag::eBullet,ETag::eItem });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,ELayer::ePlayer,
		ELayer::eObstruction,ELayer::eAttackCol,ELayer::eItem });

	// �T�m�R���C�_�[
	mpSearchCol = new CColliderCapsule
	(
		this, ELayer::eSearch,
		CVector(SEARCH_WIDTH - SEARCH_RADIUS, SEARCH_HEIGHT, 0.0f),
		CVector(-SEARCH_WIDTH + SEARCH_RADIUS, SEARCH_HEIGHT, 0.0f),
		SEARCH_RADIUS,
		true
	);
	mpSearchCol->Position(SEARCH_OFFSET_POS);
	// ��Q���A�A�C�e���ƏՓ˔���
	mpSearchCol->SetCollisionTags({ ETag::eObstruction,ETag::eItem });
	mpSearchCol->SetCollisionLayers({ ELayer::eObstruction,ELayer::eItem });
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
void CDeliveryEnemy::HitBlink()
{
	if (IsDamaging())
	{
		// �_�ŊԊu���o�߂�����
		if (mHitBlinkTime > HIT_BLINK_INTERVAL)
		{
			mHitBlinkTime -= HIT_BLINK_INTERVAL;
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
			mHitBlinkTime = 0.0f;
			mInvincibleTime = 0.0f;
		}
		mHitBlinkTime += Times::DeltaTime();
		mInvincibleTime += Times::DeltaTime();
	}
}

// �����𖞂������Ƃ��Ɏˌ�����
void CDeliveryEnemy::Shoot()
{
	// Hp��5�ȏ゠��ꍇ���������Ȃ�
	if (GetHp() <= 5) return;

	// �v���C���[�擾
	CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(CDeliveryPlayer::Instance());
	// �v���C���[���W
	CVector playerPos = player->Position();
	// �v���C���[�̂��铹
	ERoadType playerRoadType = player->GetRoadType();
	// �C���^�[�o�����o�߂�����
	mLeftShootTime -= Times::DeltaTime();
	mRightShootTime -= Times::DeltaTime();
	mBackShootTime -= Times::DeltaTime();
	// 臒l���ŉ��ɂ���
	if (Position().Z() + SIDE_PLAYER_THRESHOLD >= playerPos.Z()&&
		Position().Z() - SIDE_PLAYER_THRESHOLD <= playerPos.Z())
	{
		// ���E�ǂ���Ɍ���
		bool isLeftShoot = false;
		// �����̓��ɂ���ď���
		switch (mRoadType)
		{
		case ERoadType::eLeft1:
			// �E�Ɍ���
			isLeftShoot = false;
			break;
		case ERoadType::eLeft2:
			// �v���C���[����1�ɂ���Ȃ�
			if (playerRoadType == ERoadType::eLeft1)
			{
				// ���Ɍ���
				isLeftShoot = true;
			}
			// ����ȊO
			else
			{
				// �E�Ɍ���
				isLeftShoot = false;
			}
			break;
		case ERoadType::eRight1:
			// ���Ɍ���
			isLeftShoot = true;
			break;
		case ERoadType::eRight2:
			// �v���C���[���E1�ɂ���Ȃ�
			if (playerRoadType == ERoadType::eRight1)
			{
				// �E�Ɍ���
				isLeftShoot = false;
			}
			// ����ȊO
			else
			{
				// ���Ɍ���
				isLeftShoot = true;
			}
			break;
		}
		// ���Ɍ����C���^�[�o�����I����Ă�����
		if (isLeftShoot && mLeftShootTime <= 0.0f)
		{
			// �C���^�[�o����ݒ�
			mLeftShootTime = SHOOT_INTERVAL;
			// �z�B���𐶐�
			CDeliveryItem* item = new CDeliveryItem(this);
			// ���W��ݒ�
			item->Position(Position() + BULLET_OFFSET_POS_L);
			// ��]��ݒ�
			item->Rotation(BULLET_ROT_LR);
			// �ړ����x
			float moveSpeedZ = GetThrowSpeed() * Times::DeltaTime();
			// �ړ���ݒ�
			item->SetMoveSpeed(-VectorX() * moveSpeedZ);

			// Hp�����炷
			TakeDamage(1, nullptr, true);
		}
		// �E�Ɍ����C���^�[�o�����I����Ă�����
		else if (!isLeftShoot && mRightShootTime <= 0.0f)
		{
			// �C���^�[�o����ݒ�
			mRightShootTime = SHOOT_INTERVAL;
			// �z�B���𐶐�
			CDeliveryItem* item = new CDeliveryItem(this);
			// ���W��ݒ�
			item->Position(Position() + BULLET_OFFSET_POS_R);
			// ��]��ݒ�
			item->Rotation(BULLET_ROT_LR);
			// �ړ����x
			float moveSpeedX = GetThrowSpeed() * Times::DeltaTime();
			// �ړ���ݒ�
			item->SetMoveSpeed(VectorX() * moveSpeedX);

			// Hp�����炷
			TakeDamage(1, nullptr, true);
		}
	}

	// ���������A���ɂ��邩�A
	// ���C���^�[�o�����I����Ă���ꍇ
	if (mRoadType == playerRoadType &&
		Position().Z() < playerPos.Z() &&
		mBackShootTime <= 0.0f)
	{
		// �C���^�[�o����ݒ�
		mBackShootTime = SHOOT_INTERVAL;
		// �z�B��1�𐶐�
		CDeliveryItem* item1 = new CDeliveryItem(this);
		// ���W��ݒ�
		item1->Position(Position() + BULLET_OFFSET_POS_B);
		// �ړ����x
		float moveSpeedZ = GetThrowSpeed() * Times::DeltaTime();
		// �ړ���ݒ�
		item1->SetMoveSpeed(-VectorZ() * moveSpeedZ);

		// Hp�����炷
		TakeDamage(1, nullptr, true);
	}
}

// �Ԑ��ύX��̍��W�����߂�
CVector CDeliveryEnemy::GetTargetPos(bool isLeft)
{
	// �����̍��W���擾
	CVector targetPos = Position();
	// �Ԑ��ύX���̏ꍇ
	if (mState == EState::eChangeRoad)
	{
		// �ړ��̕������t�Ȃ�
		if (mIsLeftMove != isLeft)
		{
			// �����̓��ɖ߂�
			mTargetRoadType = mRoadType;
			// ���W��ݒ�
			switch (mTargetRoadType)
			{
			case ERoadType::eLeft1:
				targetPos.X(ROAD_LEFT1_POSX);
				break;
			case ERoadType::eLeft2:
				targetPos.X(ROAD_LEFT2_POSX);
				break;
			case ERoadType::eRight1:
				targetPos.X(ROAD_RIGHT1_POSX);
				break;
			case ERoadType::eRight2:
				targetPos.X(ROAD_RIGHT2_POSX);
				break;
			}
			// �ړ�������ݒ�
			mIsLeftMove = isLeft;
			// �ڕW���W��Ԃ�
			return targetPos;
		}
	}
	// ���E�ǂ���ֈړ����邩��ݒ�
	mIsLeftMove = isLeft;
	// �����������铹�ƍ��E�ǂ��炩�̈ړ��Ȃ̂�
	// ����ړI�n������
	switch (mRoadType)
	{
	case ERoadType::eLeft1:
		// ���ړ�
		if (isLeft)
		{
			targetPos.X(ROAD_LEFT1_POSX);
			mTargetRoadType = ERoadType::eLeft1;
		}
		// �E�ړ�
		else
		{
			targetPos.X(ROAD_LEFT2_POSX);
			mTargetRoadType = ERoadType::eLeft2;
		}
		break;
	case ERoadType::eLeft2:
		// ���ړ�
		if (isLeft)
		{
			targetPos.X(ROAD_LEFT1_POSX);
			mTargetRoadType = ERoadType::eLeft1;
		}
		// �E�ړ�
		else
		{
			targetPos.X(ROAD_RIGHT2_POSX);
			mTargetRoadType = ERoadType::eRight2;
		}
		break;
	case ERoadType::eRight1:
		// ���ړ�
		if (isLeft)
		{
			targetPos.X(ROAD_RIGHT2_POSX);
			mTargetRoadType = ERoadType::eRight2;
		}
		// �E�ړ�
		else
		{
			targetPos.X(ROAD_RIGHT1_POSX);
			mTargetRoadType = ERoadType::eRight1;
		}
		break;
	case ERoadType::eRight2:
		// ���ړ�
		if (isLeft)
		{
			targetPos.X(ROAD_LEFT2_POSX);
			mTargetRoadType = ERoadType::eLeft2;
		}
		// �E�ړ�
		else
		{
			targetPos.X(ROAD_RIGHT1_POSX);
			mTargetRoadType = ERoadType::eRight1;
		}
		break;
	}
	return targetPos;
}

// �����铹���댯��
bool CDeliveryEnemy::GetNowRoadDanger() const
{
	// ���������铹���댯��
	switch (mRoadType)
	{
	case ERoadType::eLeft1:
		return mIsLeft1Danger;
		break;
	case ERoadType::eLeft2:
		return mIsLeft2Danger;
		break;
	case ERoadType::eRight1:
		return mIsRight1Danger;
		break;
	case ERoadType::eRight2:
		return mIsRight2Danger;
		break;
	}
}

// �w�肵�������댯��
bool CDeliveryEnemy::GetRoadDanger(ERoadType roadType) const
{
	switch (roadType)
	{
	case ERoadType::eLeft1:
		return mIsLeft1Danger;
		break;
	case ERoadType::eLeft2:
		return mIsLeft2Danger;
		break;
	case ERoadType::eRight1:
		return mIsRight1Danger;
		break;
	case ERoadType::eRight2:
		return mIsRight2Danger;
		break;
	}
}

// �w�肵�����ɃA�C�e�������邩��ݒ肷��
void CDeliveryEnemy::SetRoadItem(ERoadType roadType, bool isItem)
{
	switch (roadType)
	{
	case ERoadType::eLeft1:
		mIsLeft1Item = isItem;
		break;
	case ERoadType::eLeft2:
		mIsLeft2Item = isItem;
		break;
	case ERoadType::eRight1:
		mIsRight1Item = isItem;
		break;
	case ERoadType::eRight2:
		mIsRight2Item = isItem;
		break;
	}
}

// �w�肵�����ɃA�C�e�������邩
bool CDeliveryEnemy::GetRoadItem(ERoadType roadType) const
{
	switch (roadType)
	{
	case ERoadType::eLeft1:
		return mIsLeft1Item;
		break;
	case ERoadType::eLeft2:
		return mIsLeft2Item;
		break;
	case ERoadType::eRight1:
		return mIsRight1Item;
		break;
	case ERoadType::eRight2:
		return mIsRight2Item;
		break;
	}
}

// ���E�ɃA�C�e��������ꍇ�ɎԐ���ύX����
void CDeliveryEnemy::ChangeRoadToItem()
{
	// �Ԑ��ύX�����邩
	bool isChangeRoad = false;
	switch (mRoadType)
	{
	case ERoadType::eLeft1:
		// �E�̓��ɃA�C�e��������A���S�ȏꍇ
		if (GetRoadItem(ERoadType::eLeft2) &&
			!GetRoadDanger(ERoadType::eLeft2))
		{
			// �E�ֈړ�
			GetTargetPos(false);
			isChangeRoad = true;
		}
		break;
	case ERoadType::eLeft2:
		// �E�̓��ɃA�C�e��������A���S�ȏꍇ
		if (GetRoadItem(ERoadType::eRight2) &&
			!GetRoadDanger(ERoadType::eRight2))
		{
			// �E�ֈړ�
			GetTargetPos(false);
			isChangeRoad = true;
		}
		// ���̓��ɃA�C�e��������A���S�ȏꍇ
		else if (GetRoadItem(ERoadType::eLeft1) &&
			!GetRoadDanger(ERoadType::eLeft1))
		{
			// ���ֈړ�
			GetTargetPos(true);
			isChangeRoad = true;
		}
		break;
	case ERoadType::eRight1:
		// ���̓��ɃA�C�e��������A���S�ȏꍇ
		if (GetRoadItem(ERoadType::eRight2) &&
			!GetRoadDanger(ERoadType::eRight2))
		{
			// ���ֈړ�
			GetTargetPos(true);
			isChangeRoad = true;
		}
		break;
	case ERoadType::eRight2:
		// �E�̓��ɃA�C�e��������A���S�ȏꍇ
		if (GetRoadItem(ERoadType::eRight1) &&
			!GetRoadDanger(ERoadType::eRight1))
		{
			// �E�ֈړ�
			GetTargetPos(false);
			isChangeRoad = true;
		}
		// ���̓��ɃA�C�e��������A���S�ȏꍇ
		else if (GetRoadItem(ERoadType::eLeft2) &&
			!GetRoadDanger(ERoadType::eLeft2))
		{
			// ���ֈړ�
			GetTargetPos(true);
			isChangeRoad = true;
		}
		break;
	}

	// �Ԑ��ύX����Ȃ�
	if (isChangeRoad)
	{
		// �Ԑ��ύX��Ԃ�
		ChangeState(EState::eChangeRoad);
	}
}
