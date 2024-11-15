#include "CCarAndTruckManager.h"
#include "CCar.h"
#include "CGarbageTruck.h"

#define SCALE CVector(1.0f,1.0f,1.0f)
#define CAR_RIGHT_POS1 CVector(60.0f,0.0f,-150.0f)
#define CAR_RIGHT_POS2 CVector(20.0f,0.0f,-150.0f)
#define CAR_RIGHT_ROTATION CVector(0.0f,.0f,0.0f)
#define CAR_LEFT_POS1 CVector(-60.0f,0.0f,150.0f)
#define CAR_LEFT_POS2 CVector(-20.0f,0.0f,150.0f)
#define CAR_LEFT_ROTATION CVector(0.0f,180.0f,0.0f)

CCarAndTruckManager::CCarAndTruckManager()
{
	CreateCarAndTruck(CResourceManager::Get<CModel>("Car"),
		CResourceManager::Get<CModel>("GarbageTruck"));
	mpGarbageTruck->SetEnable(false);
	mpGarbageTruck->SetShow(false);
	mCars[1]->SetEnable(false);
	mCars[1]->SetShow(false);
}

CCarAndTruckManager::~CCarAndTruckManager()
{
}

void CCarAndTruckManager::Update()
{
}

// �g�p����Ԃƃg���b�N��S�Đ���
void CCarAndTruckManager::CreateCarAndTruck(CModel* car, CModel* garbageTruck)
{
	mCars.push_back(new CCar(car, CAR_RIGHT_POS1, SCALE, CAR_RIGHT_ROTATION));	// 1�ԉE	�F0
	mCars.push_back(new CCar(car, CAR_RIGHT_POS2, SCALE, CAR_RIGHT_ROTATION));	// �E����2�ԁF1
	mCars.push_back(new CCar(car, CAR_LEFT_POS1, SCALE, CAR_LEFT_ROTATION));	// 1�ԍ�	�F2
	mCars.push_back(new CCar(car, CAR_LEFT_POS2, SCALE, CAR_LEFT_ROTATION));	// ������2�ԁF3
	mpGarbageTruck = new CGarbageTruck(garbageTruck, CAR_RIGHT_POS1, SCALE, CAR_RIGHT_ROTATION);
	mpBlackTruck = new CGarbageTruck(garbageTruck, CAR_RIGHT_POS2, SCALE, CAR_RIGHT_ROTATION);
}
