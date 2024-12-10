#include "CVehicleManager.h"
#include "CCar.h"
#include "CGarbageTruck.h"
#include <random>

// �ʒu
#define CAR_RIGHT_POS1 CVector(60.0f,0.0f,-200.0f)	// �E����1��
#define CAR_RIGHT_POS2 CVector(20.0f,0.0f,-200.0f)	// �E����2��
#define CAR_LEFT_POS1 CVector(-60.0f,0.0f,200.0f)	// ������1��
#define CAR_LEFT_POS2 CVector(-20.0f,0.0f,200.0f)	// ������2��
// ��]
#define CAR_RIGHT_ROTATION CVector(0.0f,.0f,0.0f)	// �E���H�̕���
#define CAR_LEFT_ROTATION CVector(0.0f,180.0f,0.0f)	// �����H�̕���

// �o���܂ł̎���
#define CAR_POP_TIME 1.0f		// ��
#define TRUCK_POP_TIME 1.0f	// �g���b�N
#define BLACK_POP_TIME 3.0f		// ���d�u���g���b�N

CVehicleManager::CVehicleManager()
{
	CreateVehicle(CResourceManager::Get<CModel>("Car"),
		CResourceManager::Get<CModel>("GarbageTruck"),
		CResourceManager::Get<CModel>("BlackTruck"));
	// �Ԃ̏o���܂ł̎��Ԃ�ݒ�
	mCarPopTime = CAR_POP_TIME;
	// �g���b�N�̏o���܂ł̎��Ԃ�ݒ�
	mTruckPopTime = TRUCK_POP_TIME;
	// ���d�u���g���b�N�̏o���܂ł̎��Ԃ�ݒ�
	mBlackTruckPopTime = BLACK_POP_TIME;
}

CVehicleManager::~CVehicleManager()
{
}

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

// �g�p����Ԃƃg���b�N��S�Đ���
void CVehicleManager::CreateVehicle(CModel* car, CModel* garbageTruck, CModel* blackTruck)
{
	mCars.push_back(new CCar(car, CAR_RIGHT_POS1, CAR_RIGHT_ROTATION));	// �E����1��	�F0
	mCars.push_back(new CCar(car, CAR_RIGHT_POS2, CAR_RIGHT_ROTATION));	// �E����2��	�F1
	mCars.push_back(new CCar(car, CAR_LEFT_POS1, CAR_LEFT_ROTATION));	// ������1��	�F2
	mCars.push_back(new CCar(car, CAR_LEFT_POS2, CAR_LEFT_ROTATION));	// ������2��	�F3
	mpGarbageTruck = new CGarbageTruck(garbageTruck, CAR_RIGHT_POS1, CAR_RIGHT_ROTATION);
	mpBlackTruck = new CGarbageTruck(blackTruck, CAR_RIGHT_POS2, CAR_RIGHT_ROTATION);
}

// �X�e�[�W�͈͊O�̎ԁA�g���b�N�̍X�V�A�`����~�߂�
void CVehicleManager::HideVehicle()
{
	// ��
	for (CCar* car : mCars)
	{
		// �X�e�[�W�G���A�O�Ȃ�X�V�A�`����~�߂�
		if (car->Position().Z() > 250.0f || car->Position().Z() < -250.0f)
		{
			car->SetEnable(false);
			car->SetShow(false);
		}
	}
	// �g���b�N
	if (mpGarbageTruck->Position().Z() > 250.0f || mpGarbageTruck->Position().Z() < -250.0f)
	{
		mpGarbageTruck->SetEnable(false);
		mpGarbageTruck->SetShow(false);
	}
	// ���d�u���g���b�N
	if (mpBlackTruck->Position().Z() > 250.0f || mpBlackTruck->Position().Z() < -250.0f)
	{
		mpBlackTruck->SetEnable(false);
		mpBlackTruck->SetShow(false);
	}
}

// ��蕨���o��������
void CVehicleManager::SpawnVehicle()
{
	// �����_���G���W���̏�����
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 3);	// 0~3�܂�

	CVector blackPopPos;	// ���d�u���g���b�N�̐����ʒu
	CVector carPopPos;		// �Ԃ̐����ʒu
	CVector truckPopPos;	// �g���b�N�̐����ʒu


	// TODO�F���d�u�������Ⴄ����
	// ���d�u���g���b�N�̏o�����Ԃ�0�ȉ��Ȃ�o��
	if (mBlackTruckPopTime <= 0.0f)
	{
		// ���d�u���g���b�N�������Ȃ�o��������
		if (!mpBlackTruck->IsEnable())
		{
			// �����_���ɏꏊ������
			blackPopPos = RandomDecidePopPosition();
			// ���̎ԗ��Ɠ������Ő��������Ƃ����I���Ȃ���
			while (IsVehicle(blackPopPos.X()))
			{
				blackPopPos = RandomDecidePopPosition();
			}
			CVector popRotation;	// �������ꂽ���̕���

			// ��������X���W��0��菬�����ꍇ
			// ���̓��ɐ��������̂ŕ��������̓��̂��̂�ݒ�
			if (blackPopPos.X() < 0.0f)
				popRotation = CAR_LEFT_ROTATION;
			// 0�ȏ�̏ꍇ�E�̓��̂��̂�ݒ�
			else
				popRotation = CAR_RIGHT_ROTATION;

			mpBlackTruck->Position(blackPopPos);
			mpBlackTruck->Rotation(popRotation);


			// �`��X�V�J�n
			mpBlackTruck->SetEnable(true);
			mpBlackTruck->SetShow(true);

			// �o���܂ł̎��Ԃ�ݒ肵�Ȃ���
			mBlackTruckPopTime = BLACK_POP_TIME;
		}
	}

	// �Ԃ̏o�����Ԃ�0�ȉ��Ȃ�o��
	if (mCarPopTime <= 0.0f)
	{
		// �����_���ɏꏊ������
		carPopPos = RandomDecidePopPosition();
		// ���̎ԗ��Ɠ������W�Ő��������Ƃ����I���Ȃ���
		while (IsVehicle(carPopPos.X()))
		{
			carPopPos = RandomDecidePopPosition();
		}
		CVector popRotation;	// �������ꂽ�Ƃ��̕���

		// ��������X���W��0��菬�����ꍇ
		// ���̓��ɐ��������̂ŕ��������̓��̂��̂�ݒ�
		if (carPopPos.X() < 0.0f)
			popRotation = CAR_LEFT_ROTATION;
		// 0�ȏ�̏ꍇ�E�̓��̂��̂�ݒ�
		else
			popRotation = CAR_RIGHT_ROTATION;

		// ���ɗL���ɂȂ��Ă��Ȃ��Ԃ�L���ɂ���
		for (int i = 0; i < mCars.size(); i++)
		{
			// ���ɗL���Ȃ玟�̎�
			if (mCars[i]->IsEnable())
				continue;

			// �ʒu��]�ݒ�
			mCars[i]->Position(carPopPos);
			mCars[i]->Rotation(popRotation);

			// �`��X�V�J�n
			mCars[i]->SetEnable(true);
			mCars[i]->SetShow(true);
			break;
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
			// �����_���ɏꏊ������
			truckPopPos = RandomDecidePopPosition();
			// ���̎ԗ��Ɠ������Ő��������Ƃ����I���Ȃ���
			while (IsVehicle(truckPopPos.X()))
			{
				truckPopPos = RandomDecidePopPosition();
			}
			CVector popRotation;

			// X���W��0��菬�����ꍇ
			// ���̓��ɐ��������̂ŕ��������̓��̂��̂ɐݒ�
			if (truckPopPos.X() < 0.0f)
				popRotation = CAR_LEFT_ROTATION;
			// 0�ȏ�̏ꍇ�E�̓�
			else
				popRotation = CAR_RIGHT_ROTATION;

			// �ʒu��]�ݒ�
			mpGarbageTruck->Position(truckPopPos);
			mpGarbageTruck->Rotation(popRotation);

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
	mBlackTruckPopTime -= Times::DeltaTime();
}

// �����_���Ŏԗ����o��������ꏊ�����߂�
CVector CVehicleManager::RandomDecidePopPosition()
{
	// �����_���G���W���̏�����
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 3);	// 0~3�܂ł͈̔�

	int randomNumber = dist(gen);

	// 0�Ȃ獶����1��
	if (randomNumber == 0)
		return CAR_LEFT_POS1;
	// 1�Ȃ獶����2��
	else if (randomNumber == 1)
		return CAR_LEFT_POS2;
	// 2�Ȃ�E����1��
	else if (randomNumber == 2)
		return CAR_RIGHT_POS1;
	// 3�Ȃ�E����2��
	else
		return CAR_RIGHT_POS2;
}

// �w�肵��X���W(��)�ɑ��̎ԗ������邩�`�F�b�N����
bool CVehicleManager::IsVehicle(float xPos)
{
	// �S�Ă̎Ԃ��m�F
	for (int i = 0; i < mCars.size(); i++)
	{
		// �����Ȃ�o�����Ă��Ȃ��̂Ń`�F�b�N���Ȃ�
		if (!mCars[i]->IsEnable())
			continue;

		// X���W�������Ȃ瓯����
		if (mCars[i]->Position().X() == xPos)
			return true;
	}

	// �g���b�N���m�F
	// �L���Ȃ�o�����Ă���̂Ń`�F�b�N
	if (mpGarbageTruck->IsEnable())
	{
		// X���W�������Ȃ瓯����
		if (mpGarbageTruck->Position().X() == xPos)
			return true;
	}

	// ���d�u���g���b�N���m�F
	// �L���Ȃ�o�����Ă���̂Ń`�F�b�N
	if (mpBlackTruck->IsEnable())
	{
		// X���W�������Ȃ瓯����
		if (mpBlackTruck->Position().X() == xPos)
			return true;
	}

	// �S�ĈႤ���Ȃ�false
	return false;
}
