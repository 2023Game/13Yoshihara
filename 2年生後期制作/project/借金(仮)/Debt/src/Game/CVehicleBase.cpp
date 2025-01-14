#include "CVehicleBase.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CVehicleManager.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "Maths.h"
 
#define FRONT_HEIGHT	13.0f	// �O������̍���
#define FRONT_WIDTH		40.0f	// �O������̕�
#define FRONT_RADIUS	12.0f	// �O������̔��a
#define TURN_SPEED		CVector(0.0f,0.5f,0.0f)	// �ԗ��̕����]�����x
#define TURN_MAX		CVector(0.0f,22.5f,0.0f)// �ԗ��̕����]���̍ő�l

// �R���X�g���N�^
CVehicleBase::CVehicleBase(CModel* model, const CVector& pos, const CVector& rotation,
	ERoadType road, std::vector<CNavNode*> patrolPoints)
	: CCharaBase(ETag::eVehicle, ETaskPriority::eVehicle)
	, mpModel(model)
	, mpBodyCol(nullptr)
	, mpFrontCol(nullptr)
	, mpSideCol(nullptr)
	, mRoadType(road)
	, mCurrentRoadRotation(rotation)
	, mNextPatrolIndex(-1)
	, mNextMoveIndex(0)
	, mPatrolPoints(patrolPoints)
	, mIsMoveEnd(false)
	, mIsMovePause(false)
	, mIsChangeRoad(false)
{
	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);

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
}

// �f�X�g���N�^
CVehicleBase::~CVehicleBase()
{
	// �R���C�_�\�̍폜
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpNavCol);
}

// �X�V
void CVehicleBase::Update()
{
	CVector moveSpeed = mMoveSpeed;

	Position(Position() + moveSpeed);

	// �o�H�T���p�̃m�[�h�����݂���΁A���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}
}

// �Փˏ���
void CVehicleBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// �`��
void CVehicleBase::Render()
{
	mpModel->Render(Matrix());
}

// �ړ������ǂ���
bool CVehicleBase::IsMove() const
{
	return mIsMove;
}

// �Ō�̏���|�C���g�܂ł̈ړ����I���������ǂ���
bool CVehicleBase::GetMoveEnd() const
{
	return mIsMoveEnd;
}

// �Ԑ���ύX����
void CVehicleBase::ChangeRoad(bool& isEnd)
{

}

// �ǂ̓��ɂ����Ԃ���ύX����
void CVehicleBase::ChangeRoadType(ERoadType roadType)
{
	// �����Ȃ珈�����Ȃ�
	if (roadType == mRoadType) return;

	mRoadType = roadType;
}

// ���ǂ̓��ɂ��邩�擾����
CVehicleBase::ERoadType CVehicleBase::GetRoadType() const
{
	return mRoadType;
}

// �{�̃R���C�_�\���擾����
CCollider* CVehicleBase::GetBodyCol() const
{
	return mpBodyCol;
}

// �o�H�T���p�R���C�_�\���擾����
CCollider* CVehicleBase::GetNavCol() const
{
	return mpNavCol;
}

// ����|�C���g�̃��X�g��ݒ肷��
void CVehicleBase::SetPatrolPoints(std::vector<CNavNode*> patrolPoints)
{
	mPatrolPoints = patrolPoints;
}

// �ԗ��̗L��������؂�ւ���
void CVehicleBase::SetOnOff(bool setOnOff)
{
	SetEnable(setOnOff);
	SetShow(setOnOff);
}

// �ϐ������Z�b�g
void CVehicleBase::Reset()
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
bool CVehicleBase::MoveTo(const CVector& targetPos, float speed, float rotateSpeed)
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
void CVehicleBase::ChangePatrolPoint()
{
	// ����|�C���g���ݒ肳��Ă��Ȃ��ꍇ�́A�������Ȃ�
	int size = mPatrolPoints.size();
	if (size == 0) return;

	// ����J�n���ł���΁A��ԍŏ��̏���|�C���g��I��
	if (mNextPatrolIndex == -1)
	{
		// ��ԍŏ��̏���|�C���g�ɐݒ肷��
		mNextPatrolIndex = 0;
	}
	// ���񂷂铹���ύX����Ă�����A
	// ����|�C���g�̔ԍ���ύX���Ȃ�
	else if (mIsChangeRoad)
	{

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
			// ����|�C���g�̌o�H�T���m�[�h�̈ʒu��ݒ肵�Ȃ������ƂŁA
			// �e�m�[�h�ւ̐ڑ������X�V
			for (CNavNode* node : mPatrolPoints)
			{
				node->SetPos(node->GetPos());
			}
			// ����|�C���g�܂ł̍ŒZ�o�H�����߂�
			if (navMgr->Navigate(mpNavNode, mPatrolPoints[mNextPatrolIndex], mMoveRoute))
			{
				// ���̖ړI�n�̃C���f�b�N�X��ݒ�
				mNextMoveIndex = 1;
			}
		}
	}
}

// �Ԑ��ύX�ňړ�����m�[�h�̍��W��ݒ肷��
void CVehicleBase::SetChangeRoadPoint(CVehicleBase* frontVehicle)
{
	// �O�̎Ԃ̍��W
	CVehicleManager* vehicleMgr = CVehicleManager::Instance();
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
		roadPosX = vehicleMgr->GetRoadPosX("L2");
		pos = CVector(roadPosX, 0.0f, pos.Z());
		// �Ԑ��ύX�p�̃m�[�h�̍��W��ݒ�
		mpChangeRoadPoint->SetPos(pos);
		break;

	// ������2�Ԗڂ̓�
	case ERoadType::eLeft2:
		// �����ƑO�̎Ԃ̒��Ԓn�_���v�Z
		pos = CVector::Lerp(Position(), frontVehiclePos, 0.5f);

		// �ύX��̓���X���W
		roadPosX = vehicleMgr->GetRoadPosX("L1");
		pos = CVector(roadPosX, 0.0f, pos.Z());
		// �Ԑ��ύX�p�̃m�[�h�̍��W��ݒ�
		mpChangeRoadPoint->SetPos(pos);
		break;

	// �E����1�Ԗڂ̓�
	case ERoadType::eRight1:
		// �����ƑO�̎Ԃ̒��Ԓn�_���v�Z
		pos = CVector::Lerp(Position(), frontVehiclePos, 0.5f);

		// �ύX��̓���X���W
		roadPosX = vehicleMgr->GetRoadPosX("R1");
		pos = CVector(roadPosX, 0.0f, pos.Z());
		// �Ԑ��ύX�p�̃m�[�h�̍��W��ݒ�
		mpChangeRoadPoint->SetPos(pos);
		break;

	// �E����2�Ԗڂ̓�
	case ERoadType::eRight2:
		// �����ƑO�̎Ԃ̒��Ԓn�_���v�Z
		pos = CVector::Lerp(Position(), frontVehiclePos, 0.5f);

		// �ύX��̓���X���W
		roadPosX = vehicleMgr->GetRoadPosX("R2");
		pos = CVector(roadPosX, 0.0f, pos.Z());
		// �Ԑ��ύX�p�̃m�[�h�̍��W��ݒ�
		mpChangeRoadPoint->SetPos(pos);
		break;
	}
}
