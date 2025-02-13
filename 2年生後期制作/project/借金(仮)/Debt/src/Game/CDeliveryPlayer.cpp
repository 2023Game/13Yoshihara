#include "CDeliveryPlayer.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CInput.h"
#include "CDeliveryItem.h"
#include "CDeliveryField.h"
#include "CDeliveryHpUI2D.h"
#include "CDeliveryEnemy.h"

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
#define CHANGE_ROAD_OFFSET_POS_L CVector(-60.0f,0.0f,0.0f)
#define CHANGE_ROAD_OFFSET_POS_R CVector( 60.0f,0.0f,0.0f)
// 臒l
#define CHANGE_ROAD_THRESHOLD 0.1f

// �e�ۂ̃I�t�Z�b�g���W
#define BULLET_OFFSET_POS_L  CVector(-20.0f,14.75f,10.0f)	// ��
#define BULLET_OFFSET_POS_R  CVector( 19.0f,14.75f,10.0f)	// �E
#define BULLET_OFFSET_POS_B  CVector( 0.0f, 14.75f,41.0f)	// ���
// �e�ۂ̕���
#define BULLET_ROT_LR	CVector(0.0f,90.0f,0.0f) // ���E

// �_���[�W��̖��G����
#define INVINCIBLE_TIME 1.0f
// �_�ŊԊu
#define HIT_FLASH_INTERVAL 0.25f

// �R���X�g���N�^
CDeliveryPlayer::CDeliveryPlayer()
	: CPlayerBase()
	, CDeliveryPlayerStatus()
	, mInvincibleTime(0.0f)
	, mHitFlashTime(0.0f)
	, mTargetPos(CVector::zero)
	, mDeliveryNum(0)
	, mDestroyEnemyNum(0)
	, mShotNum(0)
	, mHitNum(0)
	, mIsLeftMove(false)
{
	// �ړ����
	ChangeState(EState::eMove);
	// ������ݒ�
	Rotation(ROTATION);
	// �ړ������������Ȃ�
	mIsMoveDir = false;
	// �d�͖���
	mIsGravity = false;
	mpModel = CResourceManager::Get<CModel>("DeliveryPlayer");

	// HpUI�𐶐�
	mpHpUI = new CDeliveryHpUI2D(this);
	// ���݂�HP��ݒ�
	mpHpUI->SetCurrPoint(GetHp());

	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CDeliveryPlayer::~CDeliveryPlayer()
{
	// HpUI�����݂�����A�ꏏ�ɍ폜����
	if (mpHpUI != nullptr)
	{
		mpHpUI->SetOwner(nullptr);
		mpHpUI->Kill();
	}
}

// �I�u�W�F�N�g�폜����
void CDeliveryPlayer::DeleteObject(CObjectBase* obj)
{
	CPlayerBase::DeleteObject(obj);
	if (obj == mpHpUI)
	{
		mpHpUI = nullptr;
	}
}

// �X�V
void CDeliveryPlayer::Update()
{
	switch (mState)
	{
	case EState::eMove:			UpdateMove();			break;
	case EState::eChangeRoad:	UpdateChangeRoad();		break;
	case EState::eDeath:		UpdateDeath();			break;
	}

	// �ړ����Ԑ��ύX��ԂȂ�
	if (mState == EState::eMove ||
		mState == EState::eChangeRoad)
	{
		// �L�[���͉\
		ActionInput();
	}

	// ��e�̓_��
	HitFlash();
	
	// ���N���X�̍X�V
	CPlayerBase::Update();

	// ���݂�HP��ݒ�
	mpHpUI->SetCurrPoint(GetHp());

#if _DEBUG
	CDebugPrint::Print("PlayerState�F%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("PlayerHP�F%d\n", GetHp());
	CDebugPrint::Print("PlayerTargetPos�F%f,%f,%f\n", mTargetPos.X(), mTargetPos.Y(), mTargetPos.Z());
#endif
}

// �Փˏ���
void CDeliveryPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ���N���X�̏Փˏ���
	CPlayerBase::Collision(self, other, hit);
	// �{�̃R���C�_�\
	if (self == mpBodyCol)
	{
		// �G�̏ꍇ
		if (other->Layer() == ELayer::eEnemy)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);

			// �G�Ƀ_���[�W��^����
			CDeliveryEnemy* enemy = dynamic_cast<CDeliveryEnemy*>(other->Owner());
			enemy->TakeDamage(GetAttackPower(), this);

			// �Ԑ��ύX��Ԃ̏ꍇ
			if (mState == EState::eChangeRoad)
			{
				// �v���C���[��X���W
				float playerPosX = Position().X();
				// �G��X���W
				float enemyPosX = enemy->Position().X();
				// ���ړ��Ńv���C���[�̕����E�ɂ���Ȃ�
				if (mIsLeftMove &&
					playerPosX > enemyPosX)
				{
					// �����̂������֖߂�
					mTargetRoadType = mRoadType;
				}
				// �E�ړ��Ńv���C���[�̕������ɂ���Ȃ�
				else if (!mIsLeftMove &&
					playerPosX < enemyPosX)
				{
					// �����̂������֖߂�
					mTargetRoadType = mRoadType;
				}
			}
		}
	}
}

// �`��
void CDeliveryPlayer::Render()
{
	mpModel->Render(Matrix());
}

// ��Ԑ؂�ւ�
void CDeliveryPlayer::ChangeState(EState state)
{
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// �R���C�_�\�𐶐�
void CDeliveryPlayer::CreateCol()
{
	// �{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, TRUCK_HEIGHT, TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS
	);
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// �n�`�A�G�A��Q���A���˕��̍U������A�A�C�e��
	// �ƏՓ˔��������
	mpBodyCol->SetCollisionTags({ ETag::eField,ETag::eEnemy,
		ETag::eObstruction,ETag::eBullet,ETag::eItem });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,ELayer::eEnemy,
		ELayer::eObstruction,ELayer::eAttackCol,ELayer::eItem });
}

#if _DEBUG
// ��Ԃ̕�������擾
std::string CDeliveryPlayer::GetStateStr(EState state) const
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
void CDeliveryPlayer::UpdateMove()
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

	mMoveSpeed = CVector::zero;
	CVector forward = CVector::Slerp(VectorZ(), CVector::back, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// �v���C���[�̈ړ��x�N�g�������߂�
	CVector move = CalcMoveVec();
	// ���ړ���0�ɂ���
	move.X(0.0f);
	// �ړ����v���X�����Ȃ�
	if (move.Z() > 0.0f)
	{
		// �v���X�����͈̔͊O�Ȃ�
		if (Position().Z() >= ROAD_Z_AREA_P)
		{
			// Z���W��͈͂ɐݒ肵��
			Position(Position().X(), Position().Y(), ROAD_Z_AREA_P);
			// Z�ړ���0
			move.Z(0.0f);
		}
	}
	// �ړ����}�C�i�X�����Ȃ�
	else if (move.Z() < 0.0f)
	{
		// �}�C�i�X�����͈̔͊O�Ȃ�
		if (Position().Z() <= ROAD_Z_AREA_M)
		{
			// Z���W��͈͂ɐݒ肵��
			Position(Position().X(), Position().Y(), ROAD_Z_AREA_M);
			// Z�ړ���0
			move.Z(0.0f);
		}
	}
	// ���߂��ړ��x�N�g���̒����œ��͂���Ă��邩����
	if (move.LengthSqr() > 0.0f)
	{
		// ��b�ړ����x�ňړ�
		mMoveSpeed += move * GetBaseMoveSpeed() * Times::DeltaTime();
	}
}

// �Ԑ��ύX�̍X�V����
void CDeliveryPlayer::UpdateChangeRoad()
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
void CDeliveryPlayer::UpdateDeath()
{
	// �ړ����[��
	mMoveSpeedY = 0.0f;
	mMoveSpeed = CVector::zero;
	// �Փ˖���
	SetEnableCol(false);
	// �Q�[���V�[���Ńv���C���[�����S�������Ƃɂ��
	// �Q�[���I�����擾���邽�߂̕ϐ��ݒ�
	SetGameEnd(true);
}

// ���S
void CDeliveryPlayer::Death()
{
	// ���S��Ԃ�
	ChangeState(EState::eDeath);
}

// �w�肵���ʒu�܂ňړ�����
bool CDeliveryPlayer::MoveTo(const CVector& targetPos, float speed, float rotateSpeed)
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

// �A�N�V�����̃L�[����
void CDeliveryPlayer::ActionInput()
{
	// A�L�[�ŁA���ֈړ�
	if (CInput::PushKey('A'))
	{
		// ��ԍ��̎ԓ��Ȃ珈�����Ȃ�
		if (mTargetPos.X() <= ROAD_LEFT1_POSX + 30.0f) return;
		// �ړI�n��ݒ�
		mTargetPos = GetTargetPos(true);
		// �Ԑ��ύX��Ԃ�
		ChangeState(EState::eChangeRoad);
		return;
	}
	// D�L�[�ŁA�E�ֈړ�
	else if (CInput::PushKey('D'))
	{
		// ��ԉE�̎ԓ��Ȃ珈�����Ȃ�
		if (mTargetPos.X() >= ROAD_RIGHT1_POSX - 30.0f) return;
		// �ړI�n��ݒ�
		mTargetPos = GetTargetPos(false);
		// �Ԑ��ύX��Ԃ�
		ChangeState(EState::eChangeRoad);
		return;
	}

	// ���N���b�N�ŁA�������֎ˌ�
	if (CInput::PushKey(VK_LBUTTON))
	{
		// Hp��1�ȏ゠��ꍇ
		if (GetHp() >= 1)
		{
			// �z�B���𐶐�
			CDeliveryItem* item = new CDeliveryItem(this);
			// ���W��ݒ�
			item->Position(Position() + BULLET_OFFSET_POS_L);
			// ��]��ݒ�
			item->Rotation(BULLET_ROT_LR);
			// �ړ����x
			float moveSpeedX = GetThrowSpeed() * Times::DeltaTime();
			// �ړ���ݒ�
			item->SetMoveSpeed(-VectorX() * moveSpeedX);

			// Hp�����炷
			TakeDamage(1, nullptr, true);
		}
	}
	// �E�N���b�N�ŁA�E�����֎ˌ�
	if (CInput::PushKey(VK_RBUTTON))
	{
		// Hp��1�ȏ゠��ꍇ
		if (GetHp() >= 1)
		{
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
	// �X�y�[�X�L�[�ŁA����֎ˌ�
	if (CInput::PushKey(VK_SPACE))
	{
		// Hp��1�ȏ゠��ꍇ
		if (GetHp() >= 1)
		{
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
}

// �_���[�W�̓_�łƖ��G���Ԃ̏���
void CDeliveryPlayer::HitFlash()
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

// �Ԑ��ύX��̍��W�����߂�
CVector CDeliveryPlayer::GetTargetPos(bool isLeft)
{
	// ���E�ǂ���ֈړ����邩��ݒ�
	mIsLeftMove = isLeft;
	// �����̍��W���擾
	CVector targetPos = Position();
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

// �z�B��������1���₷
void CDeliveryPlayer::IncreaseDeliveryNum()
{
	mDeliveryNum++;
}

// �z�B���������擾����
int CDeliveryPlayer::GetDeliveryNum() const
{
	return mDeliveryNum;
}

// �󂵂��g���b�N�̐���1���₷
void CDeliveryPlayer::IncreaseDestroyEnemyNum()
{
	mDestroyEnemyNum++;
}

// �󂵂��g���b�N�̐����擾����
int CDeliveryPlayer::GetDestroyEnemyNum() const
{
	return mDestroyEnemyNum;
}

// ���˂�������1���₷
void CDeliveryPlayer::IncreaseShotNum()
{
	mShotNum++;
}

// ���˂��������擾����
int CDeliveryPlayer::GetShotNum() const
{
	return mShotNum;
}

// ������������1���₷
void CDeliveryPlayer::IncreaseHitNum()
{
	mHitNum++;
}

// �������������擾����
int CDeliveryPlayer::GetHitNum() const
{
	return mHitNum;
}

// ���݂̎ԓ���ݒ�
void CDeliveryPlayer::SetRoadType(ERoadType roadType)
{
	mRoadType = roadType;
}

// ���݂̎ԓ����擾
ERoadType CDeliveryPlayer::GetRoadType() const
{
	return mRoadType;
}

// �_���[�W���󂯂�
void CDeliveryPlayer::TakeDamage(int damage, CObjectBase* causer, bool isShot)
{
	// �_���[�W���󂯂Ă��鎞�̓_���[�W���󂯂Ȃ�
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