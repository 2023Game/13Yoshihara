#include "CVehicleManager.h"
#include "CCar.h"
#include "CGarbageTruck.h"

// �ʒu
#define CAR_RIGHT_POS1 CVector(60.0f,0.0f,-200.0f)	// �E����1��
#define CAR_RIGHT_POS2 CVector(20.0f,0.0f,-200.0f)	// �E����2��
#define CAR_LEFT_POS1 CVector(-60.0f,0.0f,200.0f)	// ������1��
#define CAR_LEFT_POS2 CVector(-20.0f,0.0f,200.0f)	// ������2��
// ��]
#define CAR_RIGHT_ROTATION CVector(0.0f,.0f,0.0f)	// �E���H�̕���
#define CAR_LEFT_ROTATION CVector(0.0f,180.0f,0.0f)	// �����H�̕���

// �o���܂ł̎���
#define CAR_POP_TIME 5.0f		// ��
#define TRUCK_POP_TIME 20.0f	// �g���b�N
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
	// �Ԃ̏o�����Ԃ�0�ȉ��Ȃ�o��
	if (mCarPopTime <= 0.0f)
	{
		// �ʒu��]�ݒ�
		mCars[1]->Position(CAR_LEFT_POS1);
		mCars[1]->Rotation(CAR_LEFT_ROTATION);

		// �`��X�V�J�n
		mCars[1]->SetEnable(true);
		mCars[1]->SetShow(true);
		
		// �o���܂ł̎��Ԃ�ݒ肵�Ȃ���
		mCarPopTime = CAR_POP_TIME;
	}
	// �g���b�N�̏o�����Ԃ�0�ȉ��Ȃ�o��
	if (mTruckPopTime <= 0.0f)
	{
		// �ʒu��]�ݒ�
		mpGarbageTruck->Position(CAR_LEFT_POS2);
		mpGarbageTruck->Rotation(CAR_LEFT_ROTATION);

		// �`��X�V�J�n
		mpGarbageTruck->SetEnable(true);
		mpGarbageTruck->SetShow(true);

		// �o���܂ł̎��Ԃ�ݒ肵�Ȃ���
		mTruckPopTime = TRUCK_POP_TIME;
	}
	// TODO�F���d�u�������Ⴄ����
	// ���d�u���g���b�N�̏o�����Ԃ�0�ȉ��Ȃ�o��
	if (mBlackTruckPopTime <= 0.0f)
	{
		// �`��X�V�J�n
		mpBlackTruck->SetEnable(true);
		mpBlackTruck->SetShow(true);

		// �o���܂ł̎��Ԃ�ݒ肵�Ȃ���
		mBlackTruckPopTime = BLACK_POP_TIME;
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
