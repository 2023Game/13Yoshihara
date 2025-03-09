#include "CTrashVehicleBase.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CTrashVehicleManager.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "Maths.h"
#include "CFlamethrower.h"
#include "CSound.h"

// ���̃G�t�F�N�g�̃I�t�Z�b�g���W
#define FLAME_OFFSET_POS CVector(0.0f,10.0f,20.0f)

// ���̃X�P�[���l�̍ő�l
#define FLAME_SCALE 5.0f
// ���̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define FLAME_SCALE_ANIM_TIME 0.5f
// ���̈ړ����x
#define FLAME_MOVE_SPEED 35.0f

// �R���X�g���N�^
CTrashVehicleBase::CTrashVehicleBase(CModel* model, const CVector& pos, const CVector& rotation,
	ERoadType road, std::vector<CNavNode*> patrolPoints)
	: CCharaBase(ETag::eVehicle, ETaskPriority::eVehicle)
	, mpModel(model)
	, mpBodyCol(nullptr)
	, mpFrontCol(nullptr)
	, mpSideCol(nullptr)
	, mIsMove(false)
	, mIsBroken(false)
	, mRoadType(road)
	, mCurrentRoadRotation(rotation)
	, mNextPatrolIndex(-1)
	, mNextMoveIndex(0)
	, mpPatrolPoints(patrolPoints)
	, mIsMoveEnd(false)
	, mIsMovePause(false)
	, mIsFrontVehicle(false)
	, mIsSideVehicle(false)
{
	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);
	mpNavNode->SetEnable(false);

	// �ԗ��̎���p�̃m�[�h�𐶐�
	mpNode0 = new CNavNode(Position());
	mpNode1 = new CNavNode(Position());
	mpNode2 = new CNavNode(Position());
	mpNode3 = new CNavNode(Position());
	// �ŏ��̓m�[�h�𖳌�
	mpNode0->SetEnable(false);
	mpNode1->SetEnable(false);
	mpNode2->SetEnable(false);
	mpNode3->SetEnable(false);

	// �Ԑ��ύX�p�̃m�[�h�𐶐�
	mpChangeRoadPoint = new CNavNode(Position(), true);
	// �ŏ��̓m�[�h�𖳌�
	mpChangeRoadPoint->SetEnable(false);

	// �ŏ��͕`��A�X�V���Ȃ�
	SetEnable(false);
	SetShow(false);

	Position(pos);
	Rotation(rotation);

	// ���̃G�t�F�N�g�����p
	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		FLAME_OFFSET_POS,
		CVector::up
	);
	mpFlamethrower->SetAddBlend(false);
	mpFlamethrower->SetFlameColor(CColor(0.0f,0.0f,0.0f));
	mpFlamethrower->SetFlameScale(FLAME_SCALE);
	mpFlamethrower->SetFlameScaleAnimTime(FLAME_SCALE_ANIM_TIME);
	mpFlamethrower->SetFlameMoveSpeed(FLAME_MOVE_SPEED);

	// ���ʉ���ݒ�
	mpCriticalSE = CResourceManager::Get<CSound>("CriticalSE");
}

// �f�X�g���N�^
CTrashVehicleBase::~CTrashVehicleBase()
{
	// �R���C�_�\�̍폜
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpFrontCol);
	SAFE_DELETE(mpSideCol);
}

// �X�V
void CTrashVehicleBase::Update()
{
	CVector moveSpeed = mMoveSpeed;

	Position(Position() + moveSpeed);

	mIsFrontVehicle = false;
	mIsSideVehicle = false;
}

// �Փˏ���
void CTrashVehicleBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// �`��
void CTrashVehicleBase::Render()
{
	mpModel->Render(Matrix());
}

// �ړ������ǂ���
bool CTrashVehicleBase::IsMove() const
{
	return mIsMove;
}

// �Ō�̏���|�C���g�܂ł̈ړ����I���������ǂ���
bool CTrashVehicleBase::GetMoveEnd() const
{
	return mIsMoveEnd;
}

// ���Ă��邩
bool CTrashVehicleBase::IsBroken() const
{
	return mIsBroken;
}

// �O���ɒ�~���Ă���ԗ������邩�ǂ���
bool CTrashVehicleBase::GetIsFrontVehicle() const
{
	return mIsFrontVehicle;
}

// �Ԑ���ύX����
void CTrashVehicleBase::ChangeRoad(bool& isEnd)
{

}

// �ǂ̓��ɂ����Ԃ���ύX����
void CTrashVehicleBase::ChangeRoadType(ERoadType roadType)
{
	// �����Ȃ珈�����Ȃ�
	if (roadType == mRoadType) return;

	mRoadType = roadType;
	// �������R���C�_�\�̈ʒu���ݒ�
	// ������̎ԓ������ɂ���ԓ�
	if (mRoadType == ERoadType::eLeft1 ||
		mRoadType == ERoadType::eRight1)
	{
		mpSideCol->Position(LEFT_COL_OFFSET_POS);
	}
	// ������̎ԓ����E�ɂ���ԓ�
	else
	{
		mpSideCol->Position(RIGHT_COL_OFFSET_POS);
	}
}

// ���ǂ̓��ɂ��邩�擾����
ERoadType CTrashVehicleBase::GetRoadType() const
{
	return mRoadType;
}

// �{�̃R���C�_�\���擾����
CCollider* CTrashVehicleBase::GetBodyCol() const
{
	return mpBodyCol;
}

// ����|�C���g�̃��X�g��ݒ肷��
void CTrashVehicleBase::SetPatrolPoints(std::vector<CNavNode*> patrolPoints)
{
	mpPatrolPoints = patrolPoints;
}

// �ԗ��̗L��������؂�ւ���
void CTrashVehicleBase::SetOnOff(bool isOnOff)
{
	// �L��������ݒ�
	SetEnable(isOnOff);
	SetShow(isOnOff);
	// �o�H�T���p�̃m�[�h�̗L��������ݒ�
	mpNavNode->SetEnable(isOnOff);
}

// �ϐ������Z�b�g
void CTrashVehicleBase::Reset()
{
	mNextPatrolIndex = -1;
	mIsMoveEnd = false;

	// �o�H�T���p�̃m�[�h������΁A���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}
}

// �w�肵���ʒu�܂ňړ�����
bool CTrashVehicleBase::MoveTo(const CVector& targetPos, float speed, float rotateSpeed)
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

// ���ɏ��񂷂�|�C���g��ύX
void CTrashVehicleBase::ChangePatrolPoint()
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
	}
	// ���񒆂������ꍇ�A���̏���|�C���g���w��
	// �S�Ă̏���|�C���g��ʂ����ꍇ�ړ����I��
	else
	{
		mNextPatrolIndex++;
		if (mNextPatrolIndex >= size)
		{
			mIsMoveEnd = true;
			return;
		}
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

// �Ԑ��ύX�ňړ�����m�[�h�̍��W��ݒ肷��
void CTrashVehicleBase::SetChangeRoadPoint(CTrashVehicleBase* frontVehicle)
{
	// �O�̎Ԃ̍��W
	CTrashVehicleManager* vehicleMgr = CTrashVehicleManager::Instance();
	if (vehicleMgr == nullptr) return;
	CVector frontVehiclePos = frontVehicle->Position();
	CVector pos;
	float roadPosX;

	switch (mRoadType)
	{
	// ������1�Ԗڂ̓�
	case ERoadType::eLeft1:
		// �����ƑO�̎Ԃ̒��Ԓn�_���v�Z
		pos = CVector::Lerp(Position(), frontVehiclePos, 0.5f);

		// �ύX��̓���X���W
		roadPosX = vehicleMgr->GetRoadPosX(ERoadType::eLeft2);
		pos = CVector(roadPosX, 0.0f, pos.Z());
		// �Ԑ��ύX�p�̃m�[�h�̍��W��ݒ�
		mpChangeRoadPoint->SetPos(pos);
		break;

	// ������2�Ԗڂ̓�
	case ERoadType::eLeft2:
		// �����ƑO�̎Ԃ̒��Ԓn�_���v�Z
		pos = CVector::Lerp(Position(), frontVehiclePos, 0.5f);

		// �ύX��̓���X���W
		roadPosX = vehicleMgr->GetRoadPosX(ERoadType::eLeft1);
		pos = CVector(roadPosX, 0.0f, pos.Z());
		// �Ԑ��ύX�p�̃m�[�h�̍��W��ݒ�
		mpChangeRoadPoint->SetPos(pos);
		break;

	// �E����1�Ԗڂ̓�
	case ERoadType::eRight1:
		// �����ƑO�̎Ԃ̒��Ԓn�_���v�Z
		pos = CVector::Lerp(Position(), frontVehiclePos, 0.5f);

		// �ύX��̓���X���W
		roadPosX = vehicleMgr->GetRoadPosX(ERoadType::eRight2);
		pos = CVector(roadPosX, 0.0f, pos.Z());
		// �Ԑ��ύX�p�̃m�[�h�̍��W��ݒ�
		mpChangeRoadPoint->SetPos(pos);
		break;

	// �E����2�Ԗڂ̓�
	case ERoadType::eRight2:
		// �����ƑO�̎Ԃ̒��Ԓn�_���v�Z
		pos = CVector::Lerp(Position(), frontVehiclePos, 0.5f);

		// �ύX��̓���X���W
		roadPosX = vehicleMgr->GetRoadPosX(ERoadType::eRight1);
		pos = CVector(roadPosX, 0.0f, pos.Z());
		// �Ԑ��ύX�p�̃m�[�h�̍��W��ݒ�
		mpChangeRoadPoint->SetPos(pos);
		break;
	}
}
