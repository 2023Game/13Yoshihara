#include "CDeliveryPlayer.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CInput.h"
#include "CDeliveryItem.h"
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
#define CHANGE_ROAD_OFFSET_POS_L CVector(-60.0f,0.0f,0.0f)
#define CHANGE_ROAD_OFFSET_POS_R CVector( 60.0f,0.0f,0.0f)
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

// �R���X�g���N�^
CDeliveryPlayer::CDeliveryPlayer()
	: CPlayerBase()
	, CDeliveryPlayerStatus()
	, mInvincibleTime(0.0f)
	, mHitFlashTime(0.0f)
	, mTargetPos(CVector::zero)
	, mDeliveryNum(0)
	, mDestroyEnemyNum(0)
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

	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CDeliveryPlayer::~CDeliveryPlayer()
{
}

// �_���[�W���󂯂�
void CDeliveryPlayer::TakeDamage(int damage, CObjectBase* causer)
{
	// �_���[�W���󂯂Ă��鎞�͏������Ȃ�
	if (IsDamaging()) return;
	// �_���[�W���󂯂Ă���
	mIsDamage = true;
	// �_���[�W���󂯂�
	CCharaStatusBase::TakeDamage(damage, causer);
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

	// �L�[���͉\
	ActionInput();

	// �_���[�W���󂯂Ă�����_�ł���
	HitFlash();
	
	// ���N���X�̍X�V
	CPlayerBase::Update();

#if _DEBUG
	CDebugPrint::Print("PlayerState�F%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("PlayerHP�F%d\n", GetHp());
#endif
}

// �Փˏ���
void CDeliveryPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ���N���X�̏Փˏ���
	CPlayerBase::Collision(self, other, hit);
}

// �`��
void CDeliveryPlayer::Render()
{
	mpModel->Render(Matrix());
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
	// �n�`�A�G�A��Q���A���˕��̍U������
	// �ƏՓ˔��������
	mpBodyCol->SetCollisionTags({ ETag::eField,ETag::eEnemy,
		ETag::eObstruction,ETag::eBullet });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,ELayer::eEnemy,
		ELayer::eObstruction,ELayer::eAttackCol });
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
	// �g�p���Ȃ��̂�
	// �ړI�n�������ɐݒ肵�Ă���
	mTargetPos = Position();

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
	if (CInput::PushKey('1'))
	{
		TakeDamage(1, nullptr);
	}
	// A�L�[�ŁA���ֈړ�
	if (CInput::PushKey('A'))
	{
		// ��ԍ��̎ԓ��Ȃ珈�����Ȃ�
		if (mTargetPos.X() <= -ROAD_X_AREA + 30.0f) return;
		// �ړI�n��ݒ�
		mTargetPos = mTargetPos + CHANGE_ROAD_OFFSET_POS_L;
		// �Ԑ��ύX��Ԃ�
		ChangeState(EState::eChangeRoad);
		return;
	}
	// D�L�[�ŁA�E�ֈړ�
	else if (CInput::PushKey('D'))
	{
		// ��ԉE�̎ԓ��Ȃ珈�����Ȃ�
		if (mTargetPos.X() >= ROAD_X_AREA - 30.0f) return;
		// �ړI�n��ݒ�
		mTargetPos = mTargetPos + CHANGE_ROAD_OFFSET_POS_R;
		// �Ԑ��ύX��Ԃ�
		ChangeState(EState::eChangeRoad);
		return;
	}

	//// �Ԑ��ύX���͎ˌ��ł��Ȃ�
	//if (mState == EState::eChangeRoad) return;

	// ���N���b�N�ŁA�������֎ˌ�
	if (CInput::PushKey(VK_LBUTTON))
	{
		// �z�B���𐶐�
		CDeliveryItem* item = new CDeliveryItem(this);
		// ���W��ݒ�
		item->Position(Position() + BULLET_OFFSET_POS_L);
		// ��]��ݒ�
		item->Rotation(BULLET_ROT_LR);
		// �ړ����x
		float moveSpeedX = GetBaseMoveSpeed() * Times::DeltaTime();
		// �ړ���ݒ�
		item->SetMoveSpeed(-VectorX() * moveSpeedX);
	}
	// �E�N���b�N�ŁA�E�����֎ˌ�
	if (CInput::PushKey(VK_RBUTTON))
	{
		// �z�B���𐶐�
		CDeliveryItem* item = new CDeliveryItem(this);
		// ���W��ݒ�
		item->Position(Position() + BULLET_OFFSET_POS_R);
		// ��]��ݒ�
		item->Rotation(BULLET_ROT_LR);
		// �ړ����x
		float moveSpeedX = GetBaseMoveSpeed() * Times::DeltaTime();
		// �ړ���ݒ�
		item->SetMoveSpeed(VectorX() * moveSpeedX);
	}
	// �X�y�[�X�L�[�ŁA����֎ˌ�
	if (CInput::PushKey(VK_SPACE))
	{
		// �z�B��1�𐶐�
		CDeliveryItem* item1 = new CDeliveryItem(this);
		// ���W��ݒ�
		item1->Position(Position() + BULLET_OFFSET_POS_B1);
		// �ړ���ݒ�
		item1->SetMoveSpeed(-VectorZ() * GetBaseMoveSpeed() * Times::DeltaTime());

		// �z�B��2�𐶐�
		CDeliveryItem* item2 = new CDeliveryItem(this);
		// ���W��ݒ�
		item2->Position(Position() + BULLET_OFFSET_POS_B2);
		// �ړ���ݒ�
		item2->SetMoveSpeed(-VectorZ() * GetBaseMoveSpeed() * Times::DeltaTime());
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
