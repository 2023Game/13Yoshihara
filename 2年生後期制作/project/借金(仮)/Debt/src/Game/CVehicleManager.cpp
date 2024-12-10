#include "CVehicleManager.h"
#include "CCar.h"
#include "CGarbageTruck.h"
#include <random>

// 位置
#define CAR_RIGHT_POS1 CVector(60.0f,0.0f,-200.0f)	// 右から1番
#define CAR_RIGHT_POS2 CVector(20.0f,0.0f,-200.0f)	// 右から2番
#define CAR_LEFT_POS1 CVector(-60.0f,0.0f,200.0f)	// 左から1番
#define CAR_LEFT_POS2 CVector(-20.0f,0.0f,200.0f)	// 左から2番
// 回転
#define CAR_RIGHT_ROTATION CVector(0.0f,.0f,0.0f)	// 右道路の方向
#define CAR_LEFT_ROTATION CVector(0.0f,180.0f,0.0f)	// 左道路の方向

// 出現までの時間
#define CAR_POP_TIME 1.0f		// 車
#define TRUCK_POP_TIME 1.0f	// トラック
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
	CountBlackTruckPopTime();


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
	// ランダムエンジンの初期化
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 3);	// 0~3まで

	CVector blackPopPos;	// お仕置きトラックの生成位置
	CVector carPopPos;		// 車の生成位置
	CVector truckPopPos;	// トラックの生成位置


	// TODO：お仕置きだけ違う処理
	// お仕置きトラックの出現時間が0以下なら出現
	if (mBlackTruckPopTime <= 0.0f)
	{
		// お仕置きトラックが無効なら出現させる
		if (!mpBlackTruck->IsEnable())
		{
			// ランダムに場所を決定
			blackPopPos = RandomDecidePopPosition();
			// 他の車両と同じ道で生成されるとき抽選しなおし
			while (IsVehicle(blackPopPos.X()))
			{
				blackPopPos = RandomDecidePopPosition();
			}
			CVector popRotation;	// 生成された時の方向

			// 生成するX座標が0より小さい場合
			// 左の道に生成されるので方向を左の道のものを設定
			if (blackPopPos.X() < 0.0f)
				popRotation = CAR_LEFT_ROTATION;
			// 0以上の場合右の道のものを設定
			else
				popRotation = CAR_RIGHT_ROTATION;

			mpBlackTruck->Position(blackPopPos);
			mpBlackTruck->Rotation(popRotation);


			// 描画更新開始
			mpBlackTruck->SetEnable(true);
			mpBlackTruck->SetShow(true);

			// 出現までの時間を設定しなおす
			mBlackTruckPopTime = BLACK_POP_TIME;
		}
	}

	// 車の出現時間が0以下なら出現
	if (mCarPopTime <= 0.0f)
	{
		// ランダムに場所を決定
		carPopPos = RandomDecidePopPosition();
		// 他の車両と同じ座標で生成されるとき抽選しなおし
		while (IsVehicle(carPopPos.X()))
		{
			carPopPos = RandomDecidePopPosition();
		}
		CVector popRotation;	// 生成されたときの方向

		// 生成するX座標が0より小さい場合
		// 左の道に生成されるので方向を左の道のものを設定
		if (carPopPos.X() < 0.0f)
			popRotation = CAR_LEFT_ROTATION;
		// 0以上の場合右の道のものを設定
		else
			popRotation = CAR_RIGHT_ROTATION;

		// 既に有効になっていない車を有効にする
		for (int i = 0; i < mCars.size(); i++)
		{
			// 既に有効なら次の車
			if (mCars[i]->IsEnable())
				continue;

			// 位置回転設定
			mCars[i]->Position(carPopPos);
			mCars[i]->Rotation(popRotation);

			// 描画更新開始
			mCars[i]->SetEnable(true);
			mCars[i]->SetShow(true);
			break;
		}
		// 出現までの時間を設定しなおす
		mCarPopTime = CAR_POP_TIME;
	}

	// トラックの出現時間が0以下なら出現
	if (mTruckPopTime <= 0.0f)
	{
		// トラックが無効なら出現させる
		if (!mpGarbageTruck->IsEnable())
		{
			// ランダムに場所を決定
			truckPopPos = RandomDecidePopPosition();
			// 他の車両と同じ道で生成されるとき抽選しなおし
			while (IsVehicle(truckPopPos.X()))
			{
				truckPopPos = RandomDecidePopPosition();
			}
			CVector popRotation;

			// X座標が0より小さい場合
			// 左の道に生成されるので方向を左の道のものに設定
			if (truckPopPos.X() < 0.0f)
				popRotation = CAR_LEFT_ROTATION;
			// 0以上の場合右の道
			else
				popRotation = CAR_RIGHT_ROTATION;

			// 位置回転設定
			mpGarbageTruck->Position(truckPopPos);
			mpGarbageTruck->Rotation(popRotation);

			// 描画更新開始
			mpGarbageTruck->SetEnable(true);
			mpGarbageTruck->SetShow(true);

			// 出現までの時間を設定しなおす
			mTruckPopTime = TRUCK_POP_TIME;
		}
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

// ランダムで車両を出現させる場所を決める
CVector CVehicleManager::RandomDecidePopPosition()
{
	// ランダムエンジンの初期化
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 3);	// 0~3までの範囲

	int randomNumber = dist(gen);

	// 0なら左から1番
	if (randomNumber == 0)
		return CAR_LEFT_POS1;
	// 1なら左から2番
	else if (randomNumber == 1)
		return CAR_LEFT_POS2;
	// 2なら右から1番
	else if (randomNumber == 2)
		return CAR_RIGHT_POS1;
	// 3なら右から2番
	else
		return CAR_RIGHT_POS2;
}

// 指定したX座標(道)に他の車両がいるかチェックする
bool CVehicleManager::IsVehicle(float xPos)
{
	// 全ての車を確認
	for (int i = 0; i < mCars.size(); i++)
	{
		// 無効なら出現していないのでチェックしない
		if (!mCars[i]->IsEnable())
			continue;

		// X座標が同じなら同じ道
		if (mCars[i]->Position().X() == xPos)
			return true;
	}

	// トラックを確認
	// 有効なら出現しているのでチェック
	if (mpGarbageTruck->IsEnable())
	{
		// X座標が同じなら同じ道
		if (mpGarbageTruck->Position().X() == xPos)
			return true;
	}

	// お仕置きトラックを確認
	// 有効なら出現しているのでチェック
	if (mpBlackTruck->IsEnable())
	{
		// X座標が同じなら同じ道
		if (mpBlackTruck->Position().X() == xPos)
			return true;
	}

	// 全て違う道ならfalse
	return false;
}
