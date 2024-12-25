#include "CVehicleManager.h"
#include "CCar.h"
#include "CGarbageTruck.h"
#include <random>

// 出現までの時間
#define CAR_POP_TIME 3.0f		// 車
#define TRUCK_POP_TIME 1.0f	// トラック
#define BLACK_POP_TIME 3.0f		// お仕置きトラック

CVehicleManager::CVehicleManager()
	: mSpawnZone(CAR_LEFT_POS1.X(), CAR_LEFT_POS2.X(), CAR_RIGHT_POS1.X(), CAR_RIGHT_POS2.X())
{
	mpCarModel = CResourceManager::Get<CModel>("Car");

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

// 使用するトラックを全て生成
void CVehicleManager::CreateVehicle(CModel* car, CModel* garbageTruck, CModel* blackTruck)
{
	//mCars.push_back(new CCar(car, CAR_LEFT_POS1, CAR_LEFT_ROTATION));	// 左から1番	：0
	//mCars.push_back(new CCar(car, CAR_LEFT_POS2, CAR_LEFT_ROTATION));	// 左から2番	：1
	//mCars.push_back(new CCar(car, CAR_RIGHT_POS1, CAR_RIGHT_ROTATION));	// 右から1番	：2
	//mCars.push_back(new CCar(car, CAR_RIGHT_POS2, CAR_RIGHT_ROTATION));	// 右から2番	：3

	mpGarbageTruck = new CGarbageTruck(garbageTruck, CAR_RIGHT_POS1, CAR_RIGHT_ROTATION, CVehicleBase::ERoadType::eRight1);
	mpPunishTruck = new CGarbageTruck(blackTruck, CAR_RIGHT_POS2, CAR_RIGHT_ROTATION, CVehicleBase::ERoadType::eRight2);
}

// ステージ範囲外の車、トラックの更新、描画を止める
void CVehicleManager::HideVehicle()
{
	// 車
	for (CCar* car : mCars)
	{
		// ステージエリア外なら更新、描画を止める
		if (car->Position().Z() > CAR_LEFT_POS1.Z() || car->Position().Z() < CAR_RIGHT_POS1.Z())
		{
			car->SetEnable(false);
			car->SetShow(false);
		}
	}
	// トラック
	if (mpGarbageTruck->Position().Z() > CAR_LEFT_POS1.Z() || mpGarbageTruck->Position().Z() < CAR_RIGHT_POS1.Z())
	{
		mpGarbageTruck->SetEnable(false);
		mpGarbageTruck->SetShow(false);
	}
	// お仕置きトラック
	if (mpPunishTruck->Position().Z() > CAR_LEFT_POS1.Z() || mpPunishTruck->Position().Z() < CAR_RIGHT_POS1.Z())
	{
		mpPunishTruck->SetEnable(false);
		mpPunishTruck->SetShow(false);
	}
}

// 乗り物を出現させる
void CVehicleManager::SpawnVehicle()
{
	// TODO：お仕置きだけ違う処理
	// お仕置きトラックの出現時間が0以下なら出現
	if (mBlackTruckPopTime <= 0.0f)
	{
		// お仕置きトラックが無効なら出現させる
		if (!mpPunishTruck->IsEnable())
		{
			// お仕置きトラックの生成位置
			CVector punishPopPos;
			// どの道にいるか
			CVehicleBase::ERoadType punishRoadType;

			// ランダムに場所を決定
			RandomDecidePopPosition(punishRoadType, punishPopPos);
			//// 停止している車両がいる道で生成されるとき抽選しなおし
			//while (IsVehicle(punishRoadType))
			//{
			//	// ランダムに場所を決定
			//	RandomDecidePopPosition(punishRoadType, punishPopPos);
			//}
			CVector popRotation;	// 生成された時の方向

			// 左の道なら左の道用の回転を設定
			if (punishRoadType == CVehicleBase::ERoadType::eLeft1 ||
				punishRoadType == CVehicleBase::ERoadType::eLeft2)
				popRotation = CAR_LEFT_ROTATION;
			// それ以外は右の道なので右の道用の回転を設定
			else
				popRotation = CAR_RIGHT_ROTATION;

			// 位置回転設定
			mpPunishTruck->Position(punishPopPos);
			mpPunishTruck->Rotation(popRotation);

			// どの道にいるかを設定
			mpPunishTruck->ChangeRoadType(punishRoadType);

			// 描画更新開始
			mpPunishTruck->SetEnable(true);
			mpPunishTruck->SetShow(true);

			// 出現までの時間を設定しなおす
			mBlackTruckPopTime = BLACK_POP_TIME;
		}
	}

	// 車の出現時間が0以下なら出現
	if (mCarPopTime <= 0.0f)
	{
		// 車の生成位置
		CVector carPopPos;
		// どの道にいるか
		CVehicleBase::ERoadType carRoadType;

		// ランダムに場所を決定
		RandomDecidePopPosition(carRoadType, carPopPos);
		//// 停止している車両がいる道で生成されるとき抽選しなおし
		//while (IsVehicle(carRoadType))
		//{
		//	// ランダムに場所を決定
		//	RandomDecidePopPosition(carRoadType, carPopPos);
		//}
		CVector popRotation;	// 生成されたときの方向
		bool isPop = false;	// 生成したかどうか

		// 左の道なら左の道用の回転を設定
		if (carRoadType == CVehicleBase::ERoadType::eLeft1 ||
			carRoadType == CVehicleBase::ERoadType::eLeft2)
			popRotation = CAR_LEFT_ROTATION;
		// それ以外は右の道なので右の道用の回転を設定
		else
			popRotation = CAR_RIGHT_ROTATION;

		// 既に有効になっていない車を有効にする
		for (CCar* car : mCars)
		{
			// 既に有効なら次の車
			if (car->IsEnable())
				continue;

			// 位置回転設定
			car->Position(carPopPos);
			car->Rotation(popRotation);

			// どの道にいるかを設定
			car->ChangeRoadType(carRoadType);

			// 描画更新開始
			car->SetEnable(true);
			car->SetShow(true);
			// 車を生成した
			isPop = true;
			break;
		}
		// 車を生成できていなければ
		if (!isPop)
		{
			// 新しい車を追加して描画更新開始
			mCars.push_back(new CCar(mpCarModel, carPopPos, popRotation, carRoadType));
			mCars.back()->SetEnable(true);
			mCars.back()->SetShow(true);
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
			// トラックの生成位置
			CVector truckPopPos;
			// どの道にいるか
			CVehicleBase::ERoadType truckRoadType;

			// ランダムに場所を決定
			RandomDecidePopPosition(truckRoadType, truckPopPos);
			//// 停止している車両がいる道で生成されるとき抽選しなおし
			//while (IsVehicle(truckRoadType))
			//{
			//	// ランダムに場所を決定
			//	RandomDecidePopPosition(truckRoadType, truckPopPos);
			//}
			CVector popRotation;

			// 左の道なら左の道用の回転を設定
			if (truckRoadType == CVehicleBase::ERoadType::eLeft1 ||
				truckRoadType == CVehicleBase::ERoadType::eLeft2)
				popRotation = CAR_LEFT_ROTATION;
			// それ以外は右の道なので右の道用の回転を設定
			else
				popRotation = CAR_RIGHT_ROTATION;

			// 位置回転設定
			mpGarbageTruck->Position(truckPopPos);
			mpGarbageTruck->Rotation(popRotation);

			// どの道にいるかを設定
			mpGarbageTruck->ChangeRoadType(truckRoadType);

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
void CVehicleManager::RandomDecidePopPosition(CVehicleBase::ERoadType& roadType, CVector& popPos)
{
	// ランダムエンジンの初期化
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 3);	// 0~3までの範囲

	int randomNumber = dist(gen);

	// 0なら左から1番
	if (randomNumber == 0)
	{
		roadType = CVehicleBase::ERoadType::eLeft1;
		popPos = CAR_LEFT_POS1;
	}
	// 1なら左から2番
	else if (randomNumber == 1)
	{
		roadType = CVehicleBase::ERoadType::eLeft2;
		popPos = CAR_LEFT_POS2;
	}
	// 2なら右から1番
	else if (randomNumber == 2)
	{
		roadType = CVehicleBase::ERoadType::eRight1;
		popPos = CAR_RIGHT_POS1;
	}
	// 3なら右から2番
	else
	{
		roadType = CVehicleBase::ERoadType::eRight2;
		popPos = CAR_RIGHT_POS2;
	}

}

// 指定した道に停止している車両がいるかチェックする
bool CVehicleManager::IsVehicle(CVehicleBase::ERoadType roadType)
{
	// 全ての車を確認
	for (CCar* car : mCars)
	{
		// 無効なら出現していないのでチェックしない
		if (!car->IsEnable())
			continue;

		// 同じ道かつ、停止していたらtrue
		if (roadType == car->GetRoadType() && !car->IsMove())
			return true;
	}

	// トラックを確認
	// 有効なら出現しているのでチェック
	if (mpGarbageTruck->IsEnable())
	{
		// 同じ道かつ、停止していたらtrue
		if (roadType == mpGarbageTruck->GetRoadType() && !mpGarbageTruck->IsMove())
			return true;
	}

	// お仕置きトラックを確認
	// 有効なら出現しているのでチェック
	if (mpPunishTruck->IsEnable())
	{
		// 同じ道かつ、停止していたらtrue
		if (roadType == mpPunishTruck->GetRoadType() && !mpPunishTruck->IsMove())
			return true;
	}

	// 停止している車両がいないのでfalse
	return false;
}
