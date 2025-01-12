#include "CVehicleManager.h"
#include "CCar.h"
#include "CGarbageTruck.h"
#include "CTrashVehicleSpawnZone.h"
#include <random>
#include "Maths.h"
#include "CNavNode.h"
#include "CNavManager.h"

CVehicleManager* CVehicleManager::spInstance = nullptr;

// �o���܂ł̎���
#define CAR_POP_TIME 3.0f		// ��
#define TRUCK_POP_TIME 1.0f		// �g���b�N
#define PUNISH_POP_TIME 3.0f		// ���d�u���g���b�N

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
	// �����ꏊ
	mpSpawnZone = new CTrashVehicleSpawnZone();

	// �Ԃ̃��f��
	mpCarModel = CResourceManager::Get<CModel>("Car");

	// �o�H�T���p�̃m�[�h���쐬
	CreateNavNodes();

	// �ԗ��̍쐬
	CreateVehicle(CResourceManager::Get<CModel>("Car"),
		CResourceManager::Get<CModel>("GarbageTruck"),
		CResourceManager::Get<CModel>("BlackTruck"));
}

// �f�X�g���N�^
CVehicleManager::~CVehicleManager()
{
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
		auto itr = mPatrolPointsL1.begin();
		auto end = mPatrolPointsL1.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mPatrolPointsL1.erase(itr);
			delete del;
		}

		// ������2�Ԗڂ̓���
		// ����|�C���g�ɔz�u�����m�[�h��S�č폜
		itr = mPatrolPointsL2.begin();
		end = mPatrolPointsL2.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mPatrolPointsL2.erase(itr);
			delete del;
		}

		// �E����1�Ԗڂ̓���
		// ����|�C���g�ɔz�u�����m�[�h��S�č폜
		itr = mPatrolPointsR1.begin();
		end = mPatrolPointsR1.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mPatrolPointsR1.erase(itr);
			delete del;
		}

		// �E����2�Ԗڂ̓���
		// ����|�C���g�ɔz�u�����m�[�h��S�č폜
		itr = mPatrolPointsR2.begin();
		end = mPatrolPointsR2.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mPatrolPointsR2.erase(itr);
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
	for (CCar* car : mCars)
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
	for (CCar* car : mCars)
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


	// �o���܂ł̎��Ԃ�0�ȉ��Ȃ�o��
	SpawnVehicle();
}

// �g�p����g���b�N��S�Đ���
void CVehicleManager::CreateVehicle(CModel* car, CModel* garbageTruck, CModel* blackTruck)
{
	mCars.push_back(new CCar(car, CAR_LEFT_POS1, CAR_LEFT_ROTATION, CVehicleBase::ERoadType::eLeft1, mPatrolPointsL1));

	mpGarbageTruck = new CGarbageTruck(garbageTruck, CAR_RIGHT_POS1, CAR_RIGHT_ROTATION, CVehicleBase::ERoadType::eRight1, mPatrolPointsR1);
	mpPunishTruck = new CGarbageTruck(blackTruck, CAR_RIGHT_POS2, CAR_RIGHT_ROTATION, CVehicleBase::ERoadType::eRight2, mPatrolPointsR2);
}

// �o�H�T���p�̃m�[�h���쐬
void CVehicleManager::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// ������1�Ԗڂ̓�
		mPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_1, true));
		mPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_2, true));
		mPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_3, true));
		mPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_4, true));
		mPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_5, true));

		// ������2�Ԗڂ̓�
		mPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_1, true));
		mPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_2, true));
		mPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_3, true));
		mPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_4, true));
		mPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_5, true));

		// �E����1�Ԗڂ̓�
		mPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_1, true));
		mPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_2, true));
		mPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_3, true));
		mPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_4, true));
		mPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_5, true));

		// �E����2�Ԗڂ̓�
		mPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_1, true));
		mPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_2, true));
		mPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_3, true));
		mPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_4, true));
		mPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_5, true));
	}
}

// �X�e�[�W�͈͊O�̎ԁA�g���b�N�̍X�V�A�`����~�߂�
void CVehicleManager::HideVehicle()
{
	// ��
	for (CCar* car : mCars)
	{
		// �X�e�[�W�G���A�O�Ȃ�X�V�A�`����~�߂�
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
	/*

	// TODO�F���d�u�������Ⴄ����
	// ���d�u���g���b�N�̏o�����Ԃ�0�ȉ��Ȃ�o��
	if (mPunishTruckPopTime <= 0.0f)
	{
		// ���d�u���g���b�N�������Ȃ�o��������
		if (!mpPunishTruck->IsEnable())
		{
			// ���d�u���g���b�N�̐����ʒu
			CVector punishPopPos;
			// �ǂ̓��ɂ��邩
			CVehicleBase::ERoadType punishRoadType;
			// ����|�C���g
			std::vector<CNavNode*> patrolPoints;

			// �����_���ɏꏊ������
			RandomDecidePopPosition(punishRoadType, punishPopPos);
			// �����ꏊ�Ɏԗ��������璊�I���Ȃ���
			if (IsSpawnZone(punishRoadType))
			{
				// �����_���ɏꏊ������
				RandomDecidePopPosition(punishRoadType, punishPopPos);
			}
			//// ��~���Ă���ԗ������铹�Ő��������Ƃ����I���Ȃ���
			//while (IsVehicle(punishRoadType))
			//{
			//	// �����_���ɏꏊ������
			//	RandomDecidePopPosition(punishRoadType, punishPopPos);
			//}
			CVector popRotation;	// �������ꂽ���̕���

			// ���̓�1�Ȃ�
			if (punishRoadType == CVehicleBase::ERoadType::eLeft1)
			{
				// ������1�Ԗڂ̓��̏���|�C���g
				patrolPoints = mPatrolPointsL1;
				// ���̓��p�̉�]��ݒ�
				popRotation = CAR_LEFT_ROTATION;
			}
			// ���̓�2�Ȃ�
			else if (punishRoadType == CVehicleBase::ERoadType::eLeft2)
			{
				// ������2�Ԗڂ̓��̏���|�C���g
				patrolPoints = mPatrolPointsL2;
				// ���̓��p�̉�]��ݒ�
				popRotation = CAR_LEFT_ROTATION;
			}
			// �E�̓�1�Ȃ�
			else if (punishRoadType == CVehicleBase::ERoadType::eRight1)
			{
				// �E����1�Ԗڂ̓��̏���|�C���g
				patrolPoints = mPatrolPointsR1;
				// �E�̓��p�̉�]��ݒ�
				popRotation = CAR_RIGHT_ROTATION;
			}
			// �E�̓�2�Ȃ�
			else
			{
				// �E����2�Ԗڂ̓��̏���|�C���g
				patrolPoints = mPatrolPointsR2;
				// �E�̓��p�̉�]��ݒ�
				popRotation = CAR_RIGHT_ROTATION;
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

	// �Ԃ̏o�����Ԃ�0�ȉ��Ȃ�o��
	if (mCarPopTime <= 0.0f)
	{
		// �Ԃ̐����ʒu
		CVector carPopPos;
		// �ǂ̓��ɂ��邩
		CVehicleBase::ERoadType carRoadType;
		// ����|�C���g
		std::vector<CNavNode*> patrolPoints;

		// �����_���ɏꏊ������
		RandomDecidePopPosition(carRoadType, carPopPos);
		// �����ꏊ�Ɏԗ��������璊�I���Ȃ���
		if (IsSpawnZone(carRoadType))
		{
			// �����_���ɏꏊ������
			RandomDecidePopPosition(carRoadType, carPopPos);
		}
		//// ��~���Ă���ԗ������铹�Ő��������Ƃ����I���Ȃ���
		//while (IsVehicle(carRoadType))
		//{
		//	// �����_���ɏꏊ������
		//	RandomDecidePopPosition(carRoadType, carPopPos);
		//}
		CVector popRotation;	// �������ꂽ�Ƃ��̕���
		bool isPop = false;	// �����������ǂ���

		// ���̓�1�Ȃ�
		if (carRoadType == CVehicleBase::ERoadType::eLeft1)
		{
			// ������1�Ԗڂ̓��̏���|�C���g
			patrolPoints = mPatrolPointsL1;
			// ���̓��p�̉�]��ݒ�
			popRotation = CAR_LEFT_ROTATION;
		}
		// ���̓�2�Ȃ�
		else if (carRoadType == CVehicleBase::ERoadType::eLeft2)
		{
			// ������2�Ԗڂ̓��̏���|�C���g
			patrolPoints = mPatrolPointsL2;
			// ���̓��p�̉�]��ݒ�
			popRotation = CAR_LEFT_ROTATION;
		}
		// �E�̓�1�Ȃ�
		else if (carRoadType == CVehicleBase::ERoadType::eRight1)
		{
			// �E����1�Ԗڂ̓��̏���|�C���g
			patrolPoints = mPatrolPointsR1;
			// �E�̓��p�̉�]��ݒ�
			popRotation = CAR_RIGHT_ROTATION;
		}
		// �E�̓�2�Ȃ�
		else
		{
			// �E����2�Ԗڂ̓��̏���|�C���g
			patrolPoints = mPatrolPointsR2;
			// �E�̓��p�̉�]��ݒ�
			popRotation = CAR_RIGHT_ROTATION;
		}

		// ���ɗL���ɂȂ��Ă��Ȃ��Ԃ�L���ɂ���
		for (CCar* car : mCars)
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
			// �Ԃ𐶐�����
			isPop = true;
			break;
		}
		// �Ԃ𐶐��ł��Ă��Ȃ����
		if (!isPop)
		{
			// �V�����Ԃ�ǉ����ĕ`��X�V�J�n
			mCars.push_back(new CCar(mpCarModel, carPopPos, popRotation, carRoadType, patrolPoints));
			mCars.back()->SetOnOff(true);
		}
		// �o���܂ł̎��Ԃ�ݒ肵�Ȃ���
		mCarPopTime = CAR_POP_TIME;
	}

	*/

	// �g���b�N�̏o�����Ԃ�0�ȉ��Ȃ�o��
	if (mTruckPopTime <= 0.0f)
	{
		// �g���b�N�������Ȃ�o��������
		if (!mpGarbageTruck->IsEnable())
		{	
			// �g���b�N�̐����ʒu
			CVector truckPopPos;
			// �ǂ̓��ɂ��邩
			CVehicleBase::ERoadType truckRoadType;
			// ����|�C���g
			std::vector<CNavNode*> patrolPoints;

			truckPopPos = CAR_RIGHT_POS2;
			truckRoadType = CVehicleBase::ERoadType::eRight2;

			//// �����_���ɏꏊ������
			//RandomDecidePopPosition(truckRoadType, truckPopPos);
			//// �����ꏊ�Ɏԗ��������璊�I���Ȃ���
			//if (IsSpawnZone(truckRoadType))
			//{
			//	// �����_���ɏꏊ������
			//	RandomDecidePopPosition(truckRoadType, truckPopPos);
			//}
			//// ��~���Ă���ԗ������铹�Ő��������Ƃ����I���Ȃ���
			//while (IsVehicle(truckRoadType))
			//{
			//	// �����_���ɏꏊ������
			//	RandomDecidePopPosition(truckRoadType, truckPopPos);
			//}
			CVector popRotation;

			// ���̓�1�Ȃ�
			if (truckRoadType == CVehicleBase::ERoadType::eLeft1)
			{
				// ������1�Ԗڂ̓��̏���|�C���g
				patrolPoints = mPatrolPointsL1;
				// ���̓��p�̉�]��ݒ�
				popRotation = CAR_LEFT_ROTATION;
			}
			// ���̓�2�Ȃ�
			else if (truckRoadType == CVehicleBase::ERoadType::eLeft2)
			{
				// ������2�Ԗڂ̓��̏���|�C���g
				patrolPoints = mPatrolPointsL2;
				// ���̓��p�̉�]��ݒ�
				popRotation = CAR_LEFT_ROTATION;
			}
			// �E�̓�1�Ȃ�
			else if (truckRoadType == CVehicleBase::ERoadType::eRight1)
			{
				// �E����1�Ԗڂ̓��̏���|�C���g
				patrolPoints = mPatrolPointsR1;
				// �E�̓��p�̉�]��ݒ�
				popRotation = CAR_RIGHT_ROTATION;
			}
			// �E�̓�2�Ȃ�
			else
			{
				// �E����2�Ԗڂ̓��̏���|�C���g
				patrolPoints = mPatrolPointsR2;
				// �E�̓��p�̉�]��ݒ�
				popRotation = CAR_RIGHT_ROTATION;
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

// �����_���Ŏԗ����o��������ꏊ�����߂�
void CVehicleManager::RandomDecidePopPosition(CVehicleBase::ERoadType& roadType, CVector& popPos)
{
	// 0����3�̗���
	int randomNumber = Math::Rand(0, 3);

	// 0�Ȃ獶����1��
	if (randomNumber == 0)
	{
		roadType = CVehicleBase::ERoadType::eLeft1;
		popPos = CAR_LEFT_POS1;
	}
	// 1�Ȃ獶����2��
	else if (randomNumber == 1)
	{
		roadType = CVehicleBase::ERoadType::eLeft2;
		popPos = CAR_LEFT_POS2;
	}
	// 2�Ȃ�E����1��
	else if (randomNumber == 2)
	{
		roadType = CVehicleBase::ERoadType::eRight1;
		popPos = CAR_RIGHT_POS1;
	}
	// 3�Ȃ�E����2��
	else
	{
		roadType = CVehicleBase::ERoadType::eRight2;
		popPos = CAR_RIGHT_POS2;
	}

}

// �w�肵�����ɒ�~���Ă���ԗ������邩�`�F�b�N����
bool CVehicleManager::IsVehicle(CVehicleBase::ERoadType roadType)
{
	// �S�Ă̎Ԃ��m�F
	for (CCar* car : mCars)
	{
		// �����Ȃ�o�����Ă��Ȃ��̂Ń`�F�b�N���Ȃ�
		if (!car->IsEnable())
			continue;

		// ���������A��~���Ă�����true
		if (roadType == car->GetRoadType() && !car->IsMove())
			return true;
	}

	// �g���b�N���m�F
	// �L���Ȃ�o�����Ă���̂Ń`�F�b�N
	if (mpGarbageTruck->IsEnable())
	{
		// ���������A��~���Ă�����true
		if (roadType == mpGarbageTruck->GetRoadType() && !mpGarbageTruck->IsMove())
			return true;
	}

	// ���d�u���g���b�N���m�F
	// �L���Ȃ�o�����Ă���̂Ń`�F�b�N
	if (mpPunishTruck->IsEnable())
	{
		// ���������A��~���Ă�����true
		if (roadType == mpPunishTruck->GetRoadType() && !mpPunishTruck->IsMove())
			return true;
	}

	// ��~���Ă���ԗ������Ȃ��̂�false
	return false;
}

// �w�肵�����̐����ꏊ�Ɏԗ������邩�`�F�b�N����
bool CVehicleManager::IsSpawnZone(CVehicleBase::ERoadType roadType)
{
	// �������Ԗڂ̓�
	if (roadType == CVehicleBase::ERoadType::eLeft1)
	{
		return !mpSpawnZone->GetCanPops().IsLeft1CanPop;
	}
	// �������Ԗڂ̓�
	else if (roadType == CVehicleBase::ERoadType::eLeft2)
	{
		return !mpSpawnZone->GetCanPops().IsLeft2CanPop;
	}
	// �E�����Ԗڂ̓�
	else if (roadType == CVehicleBase::ERoadType::eRight1)
	{
		return !mpSpawnZone->GetCanPops().IsRight1CanPop;
	}
	// �E�����Ԗڂ̓�
	else
	{
		return !mpSpawnZone->GetCanPops().IsRight2CanPop;
	}
}
