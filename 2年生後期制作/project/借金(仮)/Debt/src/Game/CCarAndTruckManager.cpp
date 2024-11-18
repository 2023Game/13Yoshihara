#include "CCarAndTruckManager.h"
#include "CCar.h"
#include "CGarbageTruck.h"

#define SCALE CVector(1.0f,1.0f,1.0f)
#define CAR_RIGHT_POS1 CVector(60.0f,0.0f,-200.0f)
#define CAR_RIGHT_POS2 CVector(20.0f,0.0f,-200.0f)
#define CAR_RIGHT_ROTATION CVector(0.0f,.0f,0.0f)
#define CAR_LEFT_POS1 CVector(-60.0f,0.0f,200.0f)
#define CAR_LEFT_POS2 CVector(-20.0f,0.0f,200.0f)
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
	HideCarAndTruck();
}

// 使用する車とトラックを全て生成
void CCarAndTruckManager::CreateCarAndTruck(CModel* car, CModel* garbageTruck)
{
	mCars.push_back(new CCar(car, CAR_RIGHT_POS1, SCALE, CAR_RIGHT_ROTATION));	// 1番右	：0
	mCars.push_back(new CCar(car, CAR_RIGHT_POS2, SCALE, CAR_RIGHT_ROTATION));	// 右から2番：1
	mCars.push_back(new CCar(car, CAR_LEFT_POS1, SCALE, CAR_LEFT_ROTATION));	// 1番左	：2
	mCars.push_back(new CCar(car, CAR_LEFT_POS2, SCALE, CAR_LEFT_ROTATION));	// 左から2番：3
	mpGarbageTruck = new CGarbageTruck(garbageTruck, CAR_RIGHT_POS1, SCALE, CAR_RIGHT_ROTATION);
	mpBlackTruck = new CGarbageTruck(garbageTruck, CAR_RIGHT_POS2, SCALE, CAR_RIGHT_ROTATION);
}

// ステージ範囲外の車、トラックの更新、描画を止める
void CCarAndTruckManager::HideCarAndTruck()
{
	for (CCar* car : mCars)
	{
		// ステージエリア外なら更新、描画を止める
		if (car->Position().Z() > 250.0f || car->Position().Z() < -250.0f)
		{
			car->SetEnable(false);
			car->SetShow(false);
		}
	}
	if (mpGarbageTruck->Position().Z() > 250.0f || mpGarbageTruck->Position().Z() < -250.0f)
	{
		mpGarbageTruck->SetEnable(false);
		mpGarbageTruck->SetShow(false);
	}
	if (mpBlackTruck->Position().Z() > 250.0f || mpBlackTruck->Position().Z() < -250.0f)
	{
		mpBlackTruck->SetEnable(false);
		mpBlackTruck->SetShow(false);
	}
}
