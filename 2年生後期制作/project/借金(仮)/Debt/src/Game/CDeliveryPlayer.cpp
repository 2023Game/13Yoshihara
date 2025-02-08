#include "CDeliveryPlayer.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CInput.h"
#include "CDeliveryItem.h"

#define TRUCK_HEIGHT	13.0f	// �g���b�N�̍���
#define TRUCK_WIDTH		32.5f	// �g���b�N�̕�
#define TRUCK_RADIUS	12.5f	// �g���b�N�̔��a
// �{�̃R���C�_�\�̃I�t�Z�b�g���W
#define BODY_COL_OFFSET_POS CVector(0.0f,0.0f,-3.0f)

#define EYE_HEIGHT 5.0f		// ���_�̍���

#define ROTATE_SPEED 6.0f	// ��]���x

#define MAX_HP 10						// �ő�HP
#define BASE_MOVE_SPEED 7.5f * 30.0f	// �ړ����x
#define JUMP_SPEED 1.5f					// �W�����v���x
#define ATTACK_POWER 1					// �U����

// �����̕���
#define ROTATION CVector(0.0f,180.0f,0.0f)

// �Ԑ��ύX���̎Ԑ��̃I�t�Z�b�g���W
#define CHANGE_ROAD_OFFSET_POS_L CVector(-40.0f,0.0f,-50.0f)
#define CHANGE_ROAD_OFFSET_POS_R CVector( 40.0f,0.0f,-50.0f)
// Z�����̃I�t�Z�b�g���W
#define CHANGE_ROAD_OFFSET_POS_Z -10.0f
// 臒l
#define CHANGE_ROAD_THRESHOLD 0.1f

// �e�ۂ̃I�t�Z�b�g���W
#define BULLET_OFFSET_POS_L  CVector(-20.0f,14.75f,10.0f)	// ��
#define BULLET_OFFSET_POS_R  CVector( 19.0f,14.75f,10.0f)	// �E
#define BULLET_OFFSET_POS_B1 CVector( 6.5f, 14.75f,41.0f)	// ���1
#define BULLET_OFFSET_POS_B2 CVector(-7.5f, 14.75f,41.0f)	// ���2
// �e�ۂ̕���
#define BULLET_ROT_LR	CVector(0.0f,90.0f,0.0f) // ���E

// �R���X�g���N�^
CDeliveryPlayer::CDeliveryPlayer()
	: CPlayerBase()
	, CCharaStatusBase(MAX_HP, BASE_MOVE_SPEED, JUMP_SPEED, ATTACK_POWER)
	, mTargetPos(CVector::zero)
	, mState(EState::eMove)
{
	// ������ݒ�
	Rotation(ROTATION);
	mpModel = CResourceManager::Get<CModel>("DeliveryPlayer");

	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CDeliveryPlayer::~CDeliveryPlayer()
{
}

// �X�V
void CDeliveryPlayer::Update()
{
	switch (mState)
	{
	case EState::eMove:			UpdateMove();			break;
	case EState::eChangeRoad:	UpdateChangeRoad();		break;
	}

	// �L�[���͉\
	ActionInput();

	
	// ���N���X�̍X�V
	CPlayerBase::Update();

#if _DEBUG
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
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

	// �����ړ��̑��x��ݒ�
	mMoveSpeed = VectorZ() * GetBaseMoveSpeed() * Times::DeltaTime();
	// �v���C���[�̈ړ��x�N�g�������߂�
	CVector move = CalcMoveVec();
	// ���ړ���0�ɂ���
	move.X(0.0f);
	// ���߂��ړ��x�N�g���̒����œ��͂���Ă��邩����
	if (move.LengthSqr() > 0.0f)
	{
		// ��b�ړ����x��4����1�̑��x�ŉ�����
		mMoveSpeed += move * GetBaseMoveSpeed() * 0.25f * Times::DeltaTime();
	}
}

// �Ԑ��ύX�̍X�V����
void CDeliveryPlayer::UpdateChangeRoad()
{
	mMoveSpeed = CVector::zero;

	// �ړI�n�������̍��W��菭���O�֐ݒ�
	mTargetPos.Z(Position().Z() + CHANGE_ROAD_OFFSET_POS_Z);

	// �ړI�n�ֈړ�
	MoveTo(mTargetPos, GetBaseMoveSpeed(), ROTATE_SPEED);

	// ������X���W��臒l���ɓ�������
	if (Position().X() <= mTargetPos.X() + CHANGE_ROAD_THRESHOLD &&
		Position().X() >= mTargetPos.X() - CHANGE_ROAD_THRESHOLD)
	{
		// �����̕����ɐݒ肵����
		Rotation(ROTATION);
		// �ړ���Ԃ�
		ChangeState(EState::eMove);
		return;
	}
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
		// �ړI�n��ݒ�
		mTargetPos = mTargetPos + CHANGE_ROAD_OFFSET_POS_L;
		// �Ԑ��ύX��Ԃ�
		ChangeState(EState::eChangeRoad);
		return;
	}
	// D�L�[�ŁA�E�ֈړ�
	else if (CInput::PushKey('D'))
	{
		// �ړI�n��ݒ�
		mTargetPos = mTargetPos + CHANGE_ROAD_OFFSET_POS_R;
		// �Ԑ��ύX��Ԃ�
		ChangeState(EState::eChangeRoad);
		return;
	}

	// ���N���b�N�ŁA�������֎ˌ�
	if (CInput::PushKey(VK_LBUTTON))
	{
		// �z�B���𐶐�
		CDeliveryItem* item = new CDeliveryItem();
		// ���W��ݒ�
		item->Position(Position() + BULLET_OFFSET_POS_L);
		// ��]��ݒ�
		item->Rotation(BULLET_ROT_LR);
		// �ړ����x
		float moveSpeedX = GetBaseMoveSpeed() * Times::DeltaTime();
		float moveSpeedZ = -mMoveSpeed.Z();
		// �ړ���ݒ�
		item->SetMoveSpeed(-VectorX() * moveSpeedX +
			VectorZ() * moveSpeedZ);
	}
	// �E�N���b�N�ŁA�E�����֎ˌ�
	if (CInput::PushKey(VK_RBUTTON))
	{
		// �z�B���𐶐�
		CDeliveryItem* item = new CDeliveryItem();
		// ���W��ݒ�
		item->Position(Position() + BULLET_OFFSET_POS_R);
		// ��]��ݒ�
		item->Rotation(BULLET_ROT_LR);
		// �ړ����x
		float moveSpeedX = GetBaseMoveSpeed() * Times::DeltaTime();
		float moveSpeedZ = -mMoveSpeed.Z();
		// �ړ���ݒ�
		item->SetMoveSpeed(VectorX() * moveSpeedX +
			VectorZ() * moveSpeedZ);
	}
	// �X�y�[�X�L�[�ŁA����֎ˌ�
	if (CInput::PushKey(VK_SPACE))
	{
		// �z�B��1�𐶐�
		CDeliveryItem* item1 = new CDeliveryItem();
		// ���W��ݒ�
		item1->Position(Position() + BULLET_OFFSET_POS_B1);
		// �ړ����x
		float moveSpeedZ = -mMoveSpeed.Z();
		// �ړ���ݒ�
		item1->SetMoveSpeed(-VectorZ() * moveSpeedZ * 0.5f);

		// �z�B��2�𐶐�
		CDeliveryItem* item2 = new CDeliveryItem();
		// ���W��ݒ�
		item2->Position(Position() + BULLET_OFFSET_POS_B2);
		// �ړ���ݒ�
		item2->SetMoveSpeed(-VectorZ() * moveSpeedZ * 0.5f);
	}
}
