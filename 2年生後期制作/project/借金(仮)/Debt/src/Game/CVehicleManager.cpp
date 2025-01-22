#include "CVehicleManager.h"
#include "CCar.h"
#include "CGarbageTruck.h"
#include <random>
#include "Maths.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "Primitive.h"

CVehicleManager* CVehicleManager::spInstance = nullptr;

// �o���܂ł̎���
#define CAR_POP_TIME 3.0f		// ��
#define TRUCK_POP_TIME 1.0f		// �g���b�N
#define PUNISH_POP_TIME 4.0f		// ���d�u���g���b�N

// ������1�Ԗڂ̓��̏���|�C���g
#define PATROLPOINT_L1_1 CVector( -85.0f,0.0f, 500.0f)
#define PATROLPOINT_L1_2 CVector( -60.0f,0.0f, 470.0f)
#define PATROLPOINT_L1_3 CVector( -60.0f,0.0f,-470.0f)
#define PATROLPOINT_L1_4 CVector( -85.0f,0.0f,-500.0f)
#define PATROLPOINT_L1_5 CVector(-300.0f,0.0f,-500.0f)

// ������2�Ԗڂ̓��̏���|�C���g
#define PATROLPOINT_L2_1 CVector( 20.0f,0.0f, 580.0f)
#define PATROLPOINT_L2_2 CVector(-20.0f,0.0f, 540.0f)
#define PATROLPOINT_L2_3 CVector(-20.0f,0.0f,-540.0f)
#define PATROLPOINT_L2_4 CVector( 20.0f,0.0f,-580.0f)
#define PATROLPOINT_L2_5 CVector(300.0f,0.0f,-580.0f)

// �E����1�Ԗڂ̓��̏���|�C���g
#define PATROLPOINT_R1_1 CVector( 85.0f,0.0f,-500.0f)
#define PATROLPOINT_R1_2 CVector( 60.0f,0.0f,-470.0f)
#define PATROLPOINT_R1_3 CVector( 60.0f,0.0f, 470.0f)
#define PATROLPOINT_R1_4 CVector( 85.0f,0.0f, 500.0f)
#define PATROLPOINT_R1_5 CVector(300.0f,0.0f, 500.0f)

// �E����2�Ԗڂ̓��̏���|�C���g
#define PATROLPOINT_R2_1 CVector( -20.0f,0.0f,-580.0f)
#define PATROLPOINT_R2_2 CVector(  20.0f,0.0f,-540.0f)
#define PATROLPOINT_R2_3 CVector(  20.0f,0.0f, 540.0f)
#define PATROLPOINT_R2_4 CVector( -20.0f,0.0f, 580.0f)
#define PATROLPOINT_R2_5 CVector(-300.0f,0.0f, 580.0f)

// ���A���ɏo���\�ɂȂ�܂ł̎���
#define NEXT_CAN_POP_TIME 2.0f

// �Ԃ̍ő吔
#define CARS_MAX_NUM 2

// �ԗ��Ǘ��N���X�̃C���X�^���X���擾
CVehicleManager* CVehicleManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CVehicleManager::CVehicleManager()
	: mCarPopTime(CAR_POP_TIME)
	, mTruckPopTime(TRUCK_POP_TIME)
	, mPunishTruckPopTime(PUNISH_POP_TIME)
	, mpGarbageTruck(nullptr)
	, mpPunishTruck(nullptr)
{
	spInstance = this;

	// �ԗ��̃��f��
	mpCarModel = CResourceManager::Get<CModel>("Car");
	mpGarbageTruckModel = CResourceManager::Get<CModel>("GarbageTruck");
	mpPunishTruckModel = CResourceManager::Get<CModel>("BlackTruck");

	// �o�H�T���p�̃m�[�h���쐬
	CreateNavNodes();

	// �ԗ��̍쐬
	CreateVehicle(mpCarModel, mpGarbageTruckModel, mpPunishTruckModel);
}

// �f�X�g���N�^
CVehicleManager::~CVehicleManager()
{
	// �Ԃ̃��f����nullptr�łȂ��Ȃ�
	if (mpCarModel != nullptr)
	{
		// nullptr�ɂ���
		mpCarModel = nullptr;
	}
	// �ʏ�g���b�N�̃��f����nullptr�łȂ��Ȃ�
	if (mpGarbageTruckModel != nullptr)
	{
		// nullptr�ɂ���
		mpGarbageTruckModel = nullptr;
	}
	// ���d�u���g���b�N�̃��f����nullptr�łȂ��Ȃ�
	if (mpPunishTruckModel != nullptr)
	{
		// nullptr�ɂ���
		mpPunishTruckModel = nullptr;
	}

	// ���X�g����ɂ���
	{
		auto itr = mpCars.begin();
		auto end = mpCars.end();
		while (itr != end)
		{
			CCar* del = *itr;
			itr = mpCars.erase(itr);
			delete del;
		}
	}
	SAFE_DELETE(mpGarbageTruck);	// �ʏ�g���b�N
	SAFE_DELETE(mpPunishTruck);		// ���d�u���g���b�N

	// �C���X�^���X�ƍ폜���Ă���ԗ��Ǘ��N���X������Ȃ�폜
	if (spInstance == this)
	{
		spInstance = nullptr;
	}

	// �o�H�T���p�̃m�[�h���폜
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// ������1�Ԗڂ̓���
		// ����|�C���g�ɔz�u�����m�[�h��S�č폜
		auto itr = mpPatrolPointsL1.begin();
		auto end = mpPatrolPointsL1.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mpPatrolPointsL1.erase(itr);
			delete del;
		}

		// ������2�Ԗڂ̓���
		// ����|�C���g�ɔz�u�����m�[�h��S�č폜
		itr = mpPatrolPointsL2.begin();
		end = mpPatrolPointsL2.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mpPatrolPointsL2.erase(itr);
			delete del;
		}

		// �E����1�Ԗڂ̓���
		// ����|�C���g�ɔz�u�����m�[�h��S�č폜
		itr = mpPatrolPointsR1.begin();
		end = mpPatrolPointsR1.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mpPatrolPointsR1.erase(itr);
			delete del;
		}

		// �E����2�Ԗڂ̓���
		// ����|�C���g�ɔz�u�����m�[�h��S�č폜
		itr = mpPatrolPointsR2.begin();
		end = mpPatrolPointsR2.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mpPatrolPointsR2.erase(itr);
			delete del;
		}
	}
}

//���C�ƑS�Ă̎ԗ��̏Փ˔���
bool CVehicleManager::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit, bool alreadyHit)
{
	// �Փˏ��ۑ��p
	CHitInfo tHit;
	// �Փ˂������ǂ����t���O
	bool isHit = alreadyHit;
	// �S�Ă̎ԂƂ̏Փ˂��`�F�b�N
	for (CCar* car : mpCars)
	{
		// �����Ȃ玟��
		if (!car->IsEnable()) continue;

		// �ԂƂ̏Փ˔���
		if (CCollider::CollisionRay(car->GetBodyCol(), start, end, &tHit))
		{
			// �܂����ɏՓ˂��Ă��Ȃ��ꍇ���A
			// ���ɏՓ˂��Ă���R���C�_�\���߂��ꍇ�́A
			if (!isHit || tHit.dist < hit->dist)
			{
				// �Փˏ����X�V
				*hit = tHit;
				isHit = true;
			}
		}
	}
	// �g���b�N�����݂��邩�L���Ȃ�
	if (mpGarbageTruck != nullptr && mpGarbageTruck->IsEnable())
	{
		// �g���b�N�Ƃ̏Փ˔���
		if (CCollider::CollisionRay(mpGarbageTruck->GetBodyCol(), start, end, &tHit))
		{
			// �܂����ɏՓ˂��Ă��Ȃ��ꍇ���A
			// ���ɏՓ˂��Ă���R���C�_�\���߂��ꍇ�́A
			if (!isHit || tHit.dist < hit->dist)
			{
				// �Փˏ����X�V
				*hit = tHit;
				isHit = true;
			}
		}
	}
	// ���d�u���g���b�N�����݂��邩�L���Ȃ�
	if (mpPunishTruck != nullptr && mpPunishTruck->IsEnable())
	{
		// ���d�u���g���b�N�Ƃ̏Փ˔���
		if (CCollider::CollisionRay(mpPunishTruck->GetBodyCol(), start, end, &tHit))
		{
			// �܂����ɏՓ˂��Ă��Ȃ��ꍇ���A
			// ���ɏՓ˂��Ă���R���C�_�\���߂��ꍇ�́A
			if (!isHit || tHit.dist < hit->dist)
			{
				// �Փˏ����X�V
				*hit = tHit;
				isHit = true;
			}
		}
	}
	return isHit;
}

// ���C�ƑS�Ă̎ԗ��Ƃ̏Փ˔���i�o�H�T���p�j
bool CVehicleManager::NavCollisionRay(const CVector& start, const CVector& end, CHitInfo* hit, bool alreadyHit)
{
	// �Փˏ��ۑ��p
	CHitInfo tHit;
	// �Փ˂������ǂ����t���O
	bool isHit = alreadyHit;
	// �S�Ă̎ԂƂ̏Փ˂��`�F�b�N
	for (CCar* car : mpCars)
	{
		// �����Ȃ玟��
		if (!car->IsEnable()) continue;
		// �ړ����Ȃ玟��
		if (car->IsMove()) continue;

		// �ԂƂ̏Փ˔���
		if (CCollider::CollisionRay(car->GetNavCol(), start, end, &tHit))
		{
			// �܂����ɏՓ˂��Ă��Ȃ��ꍇ���A
			// ���ɏՓ˂��Ă���R���C�_�\���߂��ꍇ�́A
			if (!isHit || tHit.dist < hit->dist)
			{
				// �Փˏ����X�V
				*hit = tHit;
				isHit = true;
			}
		}
	}
	// �g���b�N�����݂��A
	// �L�����ړ����Ă��Ȃ��Ȃ�
	if (mpGarbageTruck != nullptr &&
		mpGarbageTruck->IsEnable() &&
		!mpGarbageTruck->IsMove())
	{
		// �g���b�N�Ƃ̏Փ˔���
		if (CCollider::CollisionRay(mpGarbageTruck->GetNavCol(), start, end, &tHit))
		{
			// �܂����ɏՓ˂��Ă��Ȃ��ꍇ���A
			// ���ɏՓ˂��Ă���R���C�_�\���߂��ꍇ�́A
			if (!isHit || tHit.dist < hit->dist)
			{
				// �Փˏ����X�V
				*hit = tHit;
				isHit = true;
			}
		}
	}
	// ���d�u���g���b�N�����݂��A
	// �L�����ړ����Ă��Ȃ��Ȃ�
	if (mpPunishTruck != nullptr &&
		mpPunishTruck->IsEnable()&&
		!mpPunishTruck->IsMove())
	{
		// ���d�u���g���b�N�Ƃ̏Փ˔���
		if (CCollider::CollisionRay(mpPunishTruck->GetNavCol(), start, end, &tHit))
		{
			// �܂����ɏՓ˂��Ă��Ȃ��ꍇ���A
			// ���ɏՓ˂��Ă���R���C�_�\���߂��ꍇ�́A
			if (!isHit || tHit.dist < hit->dist)
			{
				// �Փˏ����X�V
				*hit = tHit;
				isHit = true;
			}
		}
	}
	return isHit;
}

// �X�V
void CVehicleManager::Update()
{
	HideVehicle();

	// �o���܂ł̎��Ԃ��J�E���g
	CountCarPopTime();
	CountTruckPopTime();
	CountBlackTruckPopTime();
	// ���ꂼ��̓��̐����\�܂ł̎��Ԃ��J�E���g
	CountLeft1CanPopTime();
	CountLeft2CanPopTime();
	CountRight1CanPopTime();
	CountRight2CanPopTime();


	// �o���܂ł̎��Ԃ�0�ȉ��Ȃ�o��
	SpawnVehicle();
}

// ����X���W���擾
float CVehicleManager::GetRoadPosX(CVehicleBase::ERoadType road)
{
	// ������1�Ԗڂ̓�
	if (road == CVehicleBase::ERoadType::eLeft1)
	{
		return PATROLPOINT_L1_2.X();
	}
	// ������2�Ԗڂ̓�
	else if (road == CVehicleBase::ERoadType::eLeft2)
	{
		return PATROLPOINT_L2_2.X();
	}
	// �E����1�Ԗڂ̓�
	else if (road == CVehicleBase::ERoadType::eRight1)
	{
		return PATROLPOINT_R1_2.X();
	}
	// �E����2�Ԗڂ̓�
	else
	{
		return PATROLPOINT_R2_2.X();
	}
}

// �w�肵�����̏���|�C���g���擾
std::vector<CNavNode*> CVehicleManager::GetPatrolPoints(CVehicleBase::ERoadType road)
{
	// ������1�Ԗڂ̓�
	if (road == CVehicleBase::ERoadType::eLeft1)
	{
		return mpPatrolPointsL1;
	}
	// ������2�Ԗڂ̓�
	else if (road == CVehicleBase::ERoadType::eLeft2)
	{
		return mpPatrolPointsL2;
	}
	// �E����1�Ԗڂ̓�
	else if (road == CVehicleBase::ERoadType::eRight1)
	{
		return mpPatrolPointsR1;
	}
	// �E����2�Ԗڂ̓�
	else
	{
		return mpPatrolPointsR2;
	}
}

// �g�p����g���b�N��S�Đ���
void CVehicleManager::CreateVehicle(CModel* car, CModel* garbageTruck, CModel* blackTruck)
{
	for (int i = 0; i < CARS_MAX_NUM; i++)
	{
		mpCars.push_back(new CCar(car, VEHICLE_LEFT_POS1, VEHICLE_RIGHT_ROTATION, CVehicleBase::ERoadType::eLeft1, mpPatrolPointsL1));
	}

	mpGarbageTruck = new CGarbageTruck(garbageTruck, VEHICLE_LEFT_POS1, VEHICLE_RIGHT_ROTATION, CVehicleBase::ERoadType::eLeft1, mpPatrolPointsR1, false);
	mpPunishTruck = new CGarbageTruck(blackTruck, VEHICLE_LEFT_POS1, VEHICLE_RIGHT_ROTATION, CVehicleBase::ERoadType::eLeft1, mpPatrolPointsR2, true);
}

// �o�H�T���p�̃m�[�h���쐬
void CVehicleManager::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// ������1�Ԗڂ̓�
		mpPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_1, true));
		mpPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_2, true));
		mpPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_3, true));
		mpPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_4, true));
		mpPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_5, true));

		// ������2�Ԗڂ̓�
		mpPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_1, true));
		mpPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_2, true));
		mpPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_3, true));
		mpPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_4, true));
		mpPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_5, true));

		// �E����1�Ԗڂ̓�
		mpPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_1, true));
		mpPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_2, true));
		mpPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_3, true));
		mpPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_4, true));
		mpPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_5, true));

		// �E����2�Ԗڂ̓�
		mpPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_1, true));
		mpPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_2, true));
		mpPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_3, true));
		mpPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_4, true));
		mpPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_5, true));
	}
}

// �ړ����I�������ԁA�g���b�N�̍X�V�A�`����~�߂�
void CVehicleManager::HideVehicle()
{
	// ��
	for (CCar* car : mpCars)
	{
		// �ړ��I���Ȃ�Ȃ�X�V�A�`����~�߂�
		if (car->GetMoveEnd())
		{
			car->SetOnOff(false);
		}
	}
	// �g���b�N
	if (mpGarbageTruck->GetMoveEnd())
	{
		mpGarbageTruck->SetOnOff(false);
	}
	// ���d�u���g���b�N
	if (mpPunishTruck->GetMoveEnd())
	{
		mpPunishTruck->SetOnOff(false);
	}
}

// ��蕨���o��������
void CVehicleManager::SpawnVehicle()
{	
	// TODO�F���d�u�������Ⴄ����
	// ���d�u���g���b�N�̏o�����Ԃ�0�ȉ��Ȃ�o��
	if (mPunishTruckPopTime <= 0.0f)
	{
		// ���d�u���g���b�N�������Ȃ�o��������
		if (!mpPunishTruck->IsEnable())
		{
			// ���d�u���g���b�N�̐����ʒu
			CVector punishPopPos;
			// �������̉�]
			CVector popRotation;
			// �ǂ̓��ɂ��邩
			CVehicleBase::ERoadType punishRoadType;
			// ����|�C���g
			std::vector<CNavNode*> patrolPoints;

			// �����_���ɏꏊ������
			// ����o�����璆�g������
			if (RandomDecidePopPosition(punishRoadType, punishPopPos))
			{

				// ���̓�1�Ȃ�
				if (punishRoadType == CVehicleBase::ERoadType::eLeft1)
				{
					// ������1�Ԗڂ̓��̏���|�C���g
					patrolPoints = mpPatrolPointsL1;
					// �E�����̉�]��ݒ�
					popRotation = VEHICLE_RIGHT_ROTATION;
				}
				// ���̓�2�Ȃ�
				else if (punishRoadType == CVehicleBase::ERoadType::eLeft2)
				{
					// ������2�Ԗڂ̓��̏���|�C���g
					patrolPoints = mpPatrolPointsL2;
					// �������̉�]��ݒ�
					popRotation = VEHICLE_LEFT_ROTATION;
				}
				// �E�̓�1�Ȃ�
				else if (punishRoadType == CVehicleBase::ERoadType::eRight1)
				{
					// �E����1�Ԗڂ̓��̏���|�C���g
					patrolPoints = mpPatrolPointsR1;
					// �������̉�]��ݒ�
					popRotation = VEHICLE_LEFT_ROTATION;
				}
				// �E�̓�2�Ȃ�
				else
				{
					// �E����2�Ԗڂ̓��̏���|�C���g
					patrolPoints = mpPatrolPointsR2;
					// �E�����̉�]��ݒ�
					popRotation = VEHICLE_RIGHT_ROTATION;
				}

				// �ʒu��]�ݒ�
				mpPunishTruck->Position(punishPopPos);
				mpPunishTruck->Rotation(popRotation);

				// �ǂ̓��ɂ��邩��ݒ�
				mpPunishTruck->ChangeRoadType(punishRoadType);
				// ����|�C���g�̃��X�g��ݒ�
				mpPunishTruck->SetPatrolPoints(patrolPoints);
				// �ϐ������Z�b�g
				mpPunishTruck->Reset();

				// �`��X�V�J�n
				mpPunishTruck->SetOnOff(true);

				// �o���܂ł̎��Ԃ�ݒ肵�Ȃ���
				mPunishTruckPopTime = PUNISH_POP_TIME;
			}
		}
	}

	// �Ԃ̏o�����Ԃ�0�ȉ��Ȃ�o��
	if (mCarPopTime <= 0.0f)
	{
		// �Ԃ̐����ʒu
		CVector carPopPos;
		// �������̉�]
		CVector popRotation;
		// �ǂ̓��ɂ��邩
		CVehicleBase::ERoadType carRoadType;
		// ����|�C���g
		std::vector<CNavNode*> patrolPoints;

		// �����_���ɏꏊ������
		// ����o�����璆�g������
		if (RandomDecidePopPosition(carRoadType, carPopPos))
		{
			// ���̓�1�Ȃ�
			if (carRoadType == CVehicleBase::ERoadType::eLeft1)
			{
				// ������1�Ԗڂ̓��̏���|�C���g
				patrolPoints = mpPatrolPointsL1;
				// �E�����̉�]��ݒ�
				popRotation = VEHICLE_RIGHT_ROTATION;
			}
			// ���̓�2�Ȃ�
			else if (carRoadType == CVehicleBase::ERoadType::eLeft2)
			{
				// ������2�Ԗڂ̓��̏���|�C���g
				patrolPoints = mpPatrolPointsL2;
				// �������̉�]��ݒ�
				popRotation = VEHICLE_LEFT_ROTATION;
			}
			// �E�̓�1�Ȃ�
			else if (carRoadType == CVehicleBase::ERoadType::eRight1)
			{
				// �E����1�Ԗڂ̓��̏���|�C���g
				patrolPoints = mpPatrolPointsR1;
				// �������̉�]��ݒ�
				popRotation = VEHICLE_LEFT_ROTATION;
			}
			// �E�̓�2�Ȃ�
			else
			{
				// �E����2�Ԗڂ̓��̏���|�C���g
				patrolPoints = mpPatrolPointsR2;
				// �E�����̉�]��ݒ�
				popRotation = VEHICLE_RIGHT_ROTATION;
			}

			// ���ɗL���ɂȂ��Ă��Ȃ��Ԃ�L���ɂ���
			for (CCar* car : mpCars)
			{
				// ���ɗL���Ȃ玟�̎�
				if (car->IsEnable())
					continue;

				// �ʒu��]�ݒ�
				car->Position(carPopPos);
				car->Rotation(popRotation);

				// �ǂ̓��ɂ��邩��ݒ�
				car->ChangeRoadType(carRoadType);
				// ����|�C���g�̃��X�g��ݒ�
				car->SetPatrolPoints(patrolPoints);
				// �ϐ������Z�b�g
				car->Reset();

				// �`��X�V�J�n
				car->SetOnOff(true);
				// �o���܂ł̎��Ԃ�ݒ肵�Ȃ���
				mCarPopTime = CAR_POP_TIME;
				break;
			}
		}
	}

	

	// �g���b�N�̏o�����Ԃ�0�ȉ��Ȃ�o��
	if (mTruckPopTime <= 0.0f)
	{
		// �g���b�N�������Ȃ�o��������
		if (!mpGarbageTruck->IsEnable())
		{
			// �g���b�N�̐����ʒu
			CVector truckPopPos;
			// �������̉�]
			CVector popRotation;
			// �ǂ̓��ɂ��邩
			CVehicleBase::ERoadType truckRoadType;
			// ����|�C���g
			std::vector<CNavNode*> patrolPoints;

			// �����_���ɏꏊ������
			// ����o�����璆�g������
			if (RandomDecidePopPosition(truckRoadType, truckPopPos))
			{
				// ���̓�1�Ȃ�
				if (truckRoadType == CVehicleBase::ERoadType::eLeft1)
				{
					// ������1�Ԗڂ̓��̏���|�C���g
					patrolPoints = mpPatrolPointsL1;
					// �E�����̉�]��ݒ�
					popRotation = VEHICLE_RIGHT_ROTATION;
				}
				// ���̓�2�Ȃ�
				else if (truckRoadType == CVehicleBase::ERoadType::eLeft2)
				{
					// ������2�Ԗڂ̓��̏���|�C���g
					patrolPoints = mpPatrolPointsL2;
					// �������̉�]��ݒ�
					popRotation = VEHICLE_LEFT_ROTATION;
				}
				// �E�̓�1�Ȃ�
				else if (truckRoadType == CVehicleBase::ERoadType::eRight1)
				{
					// �E����1�Ԗڂ̓��̏���|�C���g
					patrolPoints = mpPatrolPointsR1;
					// �������̉�]��ݒ�
					popRotation = VEHICLE_LEFT_ROTATION;
				}
				// �E�̓�2�Ȃ�
				else
				{
					// �E����2�Ԗڂ̓��̏���|�C���g
					patrolPoints = mpPatrolPointsR2;
					// �E�����̉�]��ݒ�
					popRotation = VEHICLE_RIGHT_ROTATION;
				}

				// �ʒu��]�ݒ�
				mpGarbageTruck->Position(truckPopPos);
				mpGarbageTruck->Rotation(popRotation);

				// �ǂ̓��ɂ��邩��ݒ�
				mpGarbageTruck->ChangeRoadType(truckRoadType);
				// ����|�C���g�̃��X�g��ݒ�
				mpGarbageTruck->SetPatrolPoints(patrolPoints);
				// �ϐ������Z�b�g
				mpGarbageTruck->Reset();

				// �`��X�V�J�n
				mpGarbageTruck->SetOnOff(true);

				// �o���܂ł̎��Ԃ�ݒ肵�Ȃ���
				mTruckPopTime = TRUCK_POP_TIME;
			}
		}
	}
}

// �Ԃ̏o���܂ł̎��Ԃ��J�E���g
void CVehicleManager::CountCarPopTime()
{
	mCarPopTime -= Times::DeltaTime();
}

// �g���b�N�̏o���܂ł̎��Ԃ��J�E���g
void CVehicleManager::CountTruckPopTime()
{
	mTruckPopTime -= Times::DeltaTime();
}

// ���d�u���g���b�N�̏o���܂ł̎��Ԃ��J�E���g
void CVehicleManager::CountBlackTruckPopTime()
{
	mPunishTruckPopTime -= Times::DeltaTime();
}

// ���ꂼ��̓��ɏo���\�ɂȂ�܂ł��J�E���g�_�E��
// ������1�ԓ��H
void CVehicleManager::CountLeft1CanPopTime()
{
	mLeft1CanPopTime -= Times::DeltaTime();
}
// ������2�ԓ��H
void CVehicleManager::CountLeft2CanPopTime()
{
	mLeft2CanPopTime -= Times::DeltaTime();
}
// �E����1�ԓ��H
void CVehicleManager::CountRight1CanPopTime()
{
	mRight1CanPopTime -= Times::DeltaTime();
}
// �E����2�ԓ��H
void CVehicleManager::CountRight2CanPopTime()
{
	mRight2CanPopTime -= Times::DeltaTime();
}

// �����_���Ŏԗ����o��������ꏊ�����߂�
bool CVehicleManager::RandomDecidePopPosition(CVehicleBase::ERoadType& roadType, CVector& popPos)
{
	// �����\�ȓ��̐�
	int canPopRoadNum = 0;
	// ���ꂼ�ꂪ���蓖�Ă����ԍ�
	// ���蓖�Ă���Ȃ���΁A-1
	int left1RoadNum = -1;
	int left2RoadNum = -1;
	int right1RoadNum = -1;
	int right2RoadNum = -1;
	// �������\�Ȃ�
	if (IsSpawn(CVehicleBase::ERoadType::eLeft1))
	{
		// �����\�ȓ��̐��𑝂₵
		canPopRoadNum++;
		// �ԍ������蓖�Ă�
		left1RoadNum = canPopRoadNum;
	}
	if (IsSpawn(CVehicleBase::ERoadType::eLeft2))
	{
		canPopRoadNum++;
		left2RoadNum = canPopRoadNum;
	}
	if (IsSpawn(CVehicleBase::ERoadType::eRight1))
	{
		canPopRoadNum++;
		right1RoadNum = canPopRoadNum;
	}
	if (IsSpawn(CVehicleBase::ERoadType::eRight2))
	{
		canPopRoadNum++;
		right2RoadNum = canPopRoadNum;
	}

	// �����ꏊ������
	if (canPopRoadNum == 0) return false;
	// 1���琶���\�ȓ��̐��̗���
	int randomNumber = Math::Rand(1, canPopRoadNum);

	// �����Ɗ��蓖�Ă��ԍ�����v����΂��̏ꏊ�֐���
	if (randomNumber == left1RoadNum)
	{
		// ���̎�ނ�ݒ�
		roadType = CVehicleBase::ERoadType::eLeft1;
		// �������W��ݒ�
		popPos = VEHICLE_LEFT_POS1;
		// ���̓��̎��ɏo���\�ɂȂ�܂ł̎��Ԃ�ݒ�
		mLeft1CanPopTime = NEXT_CAN_POP_TIME;
	}
	else if (randomNumber == left2RoadNum)
	{
		roadType = CVehicleBase::ERoadType::eLeft2;
		popPos = VEHICLE_LEFT_POS2;
		mLeft2CanPopTime = NEXT_CAN_POP_TIME;
	}
	else if (randomNumber == right1RoadNum)
	{
		roadType = CVehicleBase::ERoadType::eRight1;
		popPos = VEHICLE_RIGHT_POS1;
		mRight1CanPopTime = NEXT_CAN_POP_TIME;
	}
	else if (randomNumber == right2RoadNum)
	{
		roadType = CVehicleBase::ERoadType::eRight2;
		popPos = VEHICLE_RIGHT_POS2;
		mRight2CanPopTime = NEXT_CAN_POP_TIME;
	}

	// �����ꏊ����
	return true;
}

// �w�肵�����ɎԂ𐶐��ł��邩
bool CVehicleManager::IsSpawn(CVehicleBase::ERoadType roadType)
{
	// �������Ԗڂ̓�
	if (roadType == CVehicleBase::ERoadType::eLeft1)
	{
		return mLeft1CanPopTime <= 0.0f;
	}
	// �������Ԗڂ̓�
	else if (roadType == CVehicleBase::ERoadType::eLeft2)
	{
		return mLeft2CanPopTime <= 0.0f;
	}
	// �E�����Ԗڂ̓�
	else if (roadType == CVehicleBase::ERoadType::eRight1)
	{
		return mRight1CanPopTime <= 0.0f;
	}
	// �E�����Ԗڂ̓�
	else
	{
		return mRight2CanPopTime <= 0.0f;
	}
}
