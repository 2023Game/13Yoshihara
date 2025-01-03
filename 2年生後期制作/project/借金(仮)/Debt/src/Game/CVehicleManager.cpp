#include "CVehicleManager.h"
#include "CCar.h"
#include "CGarbageTruck.h"
#include "CTrashVehicleSpawnZone.h"
#include <random>

CVehicleManager* CVehicleManager::spInstance = nullptr;

// �o���܂ł̎���
#define CAR_POP_TIME 3.0f		// ��
#define TRUCK_POP_TIME 1.0f		// �g���b�N
#define PUNISH_POP_TIME 3.0f		// ���d�u���g���b�N

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
}

//���C�ƑS�Ă̎ԗ��̏Փ˔���
bool CVehicleManager::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �Փˏ��ۑ��p
	CHitInfo tHit;
	// �Փ˂������ǂ����t���O
	bool isHit = false;
	// �S�Ă̎ԂƂ̏Փ˂��`�F�b�N
	for (CCar* car : mCars)
	{
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
	// �g���b�N�����݂���Ȃ�
	if (mpGarbageTruck != nullptr)
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
	// ���d�u���g���b�N�����݂���Ȃ�
	if (mpPunishTruck != nullptr)
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

// �X�V
void CVehicleManager::Update()
{
	// �X�e�[�W�O�֍s������蕨���\��
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
	mCars.push_back(new CCar(car, CAR_LEFT_POS1, CAR_LEFT_ROTATION, CVehicleBase::ERoadType::eLeft1));

	mpGarbageTruck = new CGarbageTruck(garbageTruck, CAR_RIGHT_POS1, CAR_RIGHT_ROTATION, CVehicleBase::ERoadType::eRight1);
	mpPunishTruck = new CGarbageTruck(blackTruck, CAR_RIGHT_POS2, CAR_RIGHT_ROTATION, CVehicleBase::ERoadType::eRight2);
}

// �X�e�[�W�͈͊O�̎ԁA�g���b�N�̍X�V�A�`����~�߂�
void CVehicleManager::HideVehicle()
{
	// ��
	for (CCar* car : mCars)
	{
		// �X�e�[�W�G���A�O�Ȃ�X�V�A�`����~�߂�
		if (car->Position().Z() > CAR_LEFT_POS1.Z() || car->Position().Z() < CAR_RIGHT_POS1.Z())
		{
			car->SetEnable(false);
			car->SetShow(false);
		}
	}
	// �g���b�N
	if (mpGarbageTruck->Position().Z() > CAR_LEFT_POS1.Z() || mpGarbageTruck->Position().Z() < CAR_RIGHT_POS1.Z())
	{
		mpGarbageTruck->SetEnable(false);
		mpGarbageTruck->SetShow(false);
	}
	// ���d�u���g���b�N
	if (mpPunishTruck->Position().Z() > CAR_LEFT_POS1.Z() || mpPunishTruck->Position().Z() < CAR_RIGHT_POS1.Z())
	{
		mpPunishTruck->SetEnable(false);
		mpPunishTruck->SetShow(false);
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
			// �ǂ̓��ɂ��邩
			CVehicleBase::ERoadType punishRoadType;

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

			// ���̓��Ȃ獶�̓��p�̉�]��ݒ�
			if (punishRoadType == CVehicleBase::ERoadType::eLeft1 ||
				punishRoadType == CVehicleBase::ERoadType::eLeft2)
				popRotation = CAR_LEFT_ROTATION;
			// ����ȊO�͉E�̓��Ȃ̂ŉE�̓��p�̉�]��ݒ�
			else
				popRotation = CAR_RIGHT_ROTATION;

			// �ʒu��]�ݒ�
			mpPunishTruck->Position(punishPopPos);
			mpPunishTruck->Rotation(popRotation);

			// �ǂ̓��ɂ��邩��ݒ�
			mpPunishTruck->ChangeRoadType(punishRoadType);

			// �`��X�V�J�n
			mpPunishTruck->SetEnable(true);
			mpPunishTruck->SetShow(true);

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

		// ���̓��Ȃ獶�̓��p�̉�]��ݒ�
		if (carRoadType == CVehicleBase::ERoadType::eLeft1 ||
			carRoadType == CVehicleBase::ERoadType::eLeft2)
			popRotation = CAR_LEFT_ROTATION;
		// ����ȊO�͉E�̓��Ȃ̂ŉE�̓��p�̉�]��ݒ�
		else
			popRotation = CAR_RIGHT_ROTATION;

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

			// �`��X�V�J�n
			car->SetEnable(true);
			car->SetShow(true);
			// �Ԃ𐶐�����
			isPop = true;
			break;
		}
		// �Ԃ𐶐��ł��Ă��Ȃ����
		if (!isPop)
		{
			// �V�����Ԃ�ǉ����ĕ`��X�V�J�n
			mCars.push_back(new CCar(mpCarModel, carPopPos, popRotation, carRoadType));
			mCars.back()->SetEnable(true);
			mCars.back()->SetShow(true);
		}
		// �o���܂ł̎��Ԃ�ݒ肵�Ȃ���
		mCarPopTime = CAR_POP_TIME;
	}

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

			truckPopPos = CAR_RIGHT_POS1;
			truckRoadType = CVehicleBase::ERoadType::eRight1;

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

			// ���̓��Ȃ獶�̓��p�̉�]��ݒ�
			if (truckRoadType == CVehicleBase::ERoadType::eLeft1 ||
				truckRoadType == CVehicleBase::ERoadType::eLeft2)
				popRotation = CAR_LEFT_ROTATION;
			// ����ȊO�͉E�̓��Ȃ̂ŉE�̓��p�̉�]��ݒ�
			else
				popRotation = CAR_RIGHT_ROTATION;

			// �ʒu��]�ݒ�
			mpGarbageTruck->Position(truckPopPos);
			mpGarbageTruck->Rotation(popRotation);

			// �ǂ̓��ɂ��邩��ݒ�
			mpGarbageTruck->ChangeRoadType(truckRoadType);

			// �`��X�V�J�n
			mpGarbageTruck->SetEnable(true);
			mpGarbageTruck->SetShow(true);

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
	// �����_���G���W���̏�����
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 3);	// 0~3�܂ł͈̔�

	int randomNumber = dist(gen);

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

// �~�܂��Ă���ԗ��̃��X�g���擾
std::list<CVehicleBase*> CVehicleManager::StopVehicle()
{
	std::list<CVehicleBase*> stopVehicle;
	
	// �S�Ă̎Ԃ��`�F�b�N
	for (CCar* car : mCars)
	{
		// �����Ă����玟��
		if (car->IsMove()) continue;

		// �����Ă��Ȃ��̂Ń��X�g�֒ǉ�
		stopVehicle.push_back(car);
	}

	// �S�~���W�Ԃ������Ă��Ȃ��Ȃ�
	if (!mpGarbageTruck->IsMove())
	{
		// ���X�g�֒ǉ�
		stopVehicle.push_back(mpGarbageTruck);
	}

	// ���d�u���g���b�N�������Ă��Ȃ��Ȃ�
	if (!mpPunishTruck->IsMove())
	{
		// ���X�g�֒ǉ�
		stopVehicle.push_back(mpPunishTruck);
	}

	// �~�܂��Ă���ԗ��̃��X�g��Ԃ�
	return stopVehicle;
}
