#include "CVehicleManager.h"
#include "CCar.h"
#include "CGarbageTruck.h"

// 位置
#define CAR_RIGHT_POS1 CVector(60.0f,0.0f,-200.0f)	// 右から1番
#define CAR_RIGHT_POS2 CVector(20.0f,0.0f,-200.0f)	// 右から2番
#define CAR_LEFT_POS1 CVector(-60.0f,0.0f,200.0f)	// 左から1番
#define CAR_LEFT_POS2 CVector(-20.0f,0.0f,200.0f)	// 左から2番
// 回転
#define CAR_RIGHT_ROTATION CVector(0.0f,.0f,0.0f)	// 右道路の方向
#define CAR_LEFT_ROTATION CVector(0.0f,180.0f,0.0f)	// 左道路の方向

// 出現までの時間
#define CAR_POP_TIME 5.0f		// 車
#define TRUCK_POP_TIME 20.0f	// トラック
#define BLACK_POP_TIME 3.0f		// お仕置きトラック

CVehicleManager::CVehicleManager()
{
	CreateVehicle(CResourceManager::Get<CModel>("Car"),
		CResourceManager::Get<CModel>("GarbageTruck"),
		CResourceManager::Get<CModel>("BlackTruck"));
	// 車の出現までの時間を設定
	mCarPopTime = CAR_POP_TIME;
	// トラックの出現までの時間を設定
	mTruckPopTime = TRUCK_POP_TIME;
	// お仕置きトラックの出現までの時間を設定
	mBlackTruckPopTime = BLACK_POP_TIME;
}

CVehicleManager::~CVehicleManager()
{
}

void CVehicleManager::Update()
{
	// ステージ外へ行った乗り物を非表示
	HideVehicle();

	// 出現までの時間をカウント
	CountCarPopTime();
	CountTruckPopTime();

	// 出現までの時間が0以下なら出現
	SpawnVehicle();
}

// 使用する車とトラックを全て生成
void CVehicleManager::CreateVehicle(CModel* car, CModel* garbageTruck, CModel* blackTruck)
{
	mCars.push_back(new CCar(car, CAR_RIGHT_POS1, CAR_RIGHT_ROTATION));	// 右から1番	：0
	mCars.push_back(new CCar(car, CAR_RIGHT_POS2, CAR_RIGHT_ROTATION));	// 右から2番	：1
	mCars.push_back(new CCar(car, CAR_LEFT_POS1, CAR_LEFT_ROTATION));	// 左から1番	：2
	mCars.push_back(new CCar(car, CAR_LEFT_POS2, CAR_LEFT_ROTATION));	// 左から2番	：3
	mpGarbageTruck = new CGarbageTruck(garbageTruck, CAR_RIGHT_POS1, CAR_RIGHT_ROTATION);
	mpBlackTruck = new CGarbageTruck(blackTruck, CAR_RIGHT_POS2, CAR_RIGHT_ROTATION);
}

// ステージ範囲外の車、トラックの更新、描画を止める
void CVehicleManager::HideVehicle()
{
	// 車
	for (CCar* car : mCars)
	{
		// ステージエリア外なら更新、描画を止める
		if (car->Position().Z() > 250.0f || car->Position().Z() < -250.0f)
		{
			car->SetEnable(false);
			car->SetShow(false);
		}
	}
	// トラック
	if (mpGarbageTruck->Position().Z() > 250.0f || mpGarbageTruck->Position().Z() < -250.0f)
	{
		mpGarbageTruck->SetEnable(false);
		mpGarbageTruck->SetShow(false);
	}
	// お仕置きトラック
	if (mpBlackTruck->Position().Z() > 250.0f || mpBlackTruck->Position().Z() < -250.0f)
	{
		mpBlackTruck->SetEnable(false);
		mpBlackTruck->SetShow(false);
	}
}

// 乗り物を出現させる
void CVehicleManager::SpawnVehicle()
{
	// 車の出現時間が0以下なら出現
	if (mCarPopTime <= 0.0f)
	{
		// 位置回転設定
		mCars[1]->Position(CAR_LEFT_POS1);
		mCars[1]->Rotation(CAR_LEFT_ROTATION);

		// 描画更新開始
		mCars[1]->SetEnable(true);
		mCars[1]->SetShow(true);
		
		// 出現までの時間を設定しなおす
		mCarPopTime = CAR_POP_TIME;
	}
	// トラックの出現時間が0以下なら出現
	if (mTruckPopTime <= 0.0f)
	{
		// 位置回転設定
		mpGarbageTruck->Position(CAR_LEFT_POS2);
		mpGarbageTruck->Rotation(CAR_LEFT_ROTATION);

		// 描画更新開始
		mpGarbageTruck->SetEnable(true);
		mpGarbageTruck->SetShow(true);

		// 出現までの時間を設定しなおす
		mTruckPopTime = TRUCK_POP_TIME;
	}
	// TODO：お仕置きだけ違う処理
	// お仕置きトラックの出現時間が0以下なら出現
	if (mBlackTruckPopTime <= 0.0f)
	{
		// 描画更新開始
		mpBlackTruck->SetEnable(true);
		mpBlackTruck->SetShow(true);

		// 出現までの時間を設定しなおす
		mBlackTruckPopTime = BLACK_POP_TIME;
	}

}

// 車の出現までの時間をカウント
void CVehicleManager::CountCarPopTime()
{
	mCarPopTime -= Times::DeltaTime();
}

// トラックの出現までの時間をカウント
void CVehicleManager::CountTruckPopTime()
{
	mTruckPopTime -= Times::DeltaTime();
}

// お仕置きトラックの出現までの時間をカウント
void CVehicleManager::CountBlackTruckPopTime()
{
	mBlackTruckPopTime -= Times::DeltaTime();
}
