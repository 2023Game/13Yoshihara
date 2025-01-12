#include "CVehicleManager.h"
#include "CCar.h"
#include "CGarbageTruck.h"
#include "CTrashVehicleSpawnZone.h"
#include <random>
#include "Maths.h"
#include "CNavNode.h"
#include "CNavManager.h"

CVehicleManager* CVehicleManager::spInstance = nullptr;

// 出現までの時間
#define CAR_POP_TIME 3.0f		// 車
#define TRUCK_POP_TIME 1.0f		// トラック
#define PUNISH_POP_TIME 3.0f		// お仕置きトラック

// 左から1番目の道の巡回ポイント
#define PATROLPOINT_L1_1 CVector( -85.0f,0.0f, 500.0f)
#define PATROLPOINT_L1_2 CVector( -60.0f,0.0f, 470.0f)
#define PATROLPOINT_L1_3 CVector( -60.0f,0.0f,-470.0f)
#define PATROLPOINT_L1_4 CVector( -85.0f,0.0f,-500.0f)
#define PATROLPOINT_L1_5 CVector(-300.0f,0.0f,-500.0f)

// 左から2番目の道の巡回ポイント
#define PATROLPOINT_L2_1 CVector( 20.0f,0.0f, 580.0f)
#define PATROLPOINT_L2_2 CVector(-20.0f,0.0f, 540.0f)
#define PATROLPOINT_L2_3 CVector(-20.0f,0.0f,-540.0f)
#define PATROLPOINT_L2_4 CVector( 20.0f,0.0f,-580.0f)
#define PATROLPOINT_L2_5 CVector(300.0f,0.0f,-580.0f)

// 右から1番目の道の巡回ポイント
#define PATROLPOINT_R1_1 CVector( 85.0f,0.0f,-500.0f)
#define PATROLPOINT_R1_2 CVector( 60.0f,0.0f,-470.0f)
#define PATROLPOINT_R1_3 CVector( 60.0f,0.0f, 470.0f)
#define PATROLPOINT_R1_4 CVector( 85.0f,0.0f, 500.0f)
#define PATROLPOINT_R1_5 CVector(300.0f,0.0f, 500.0f)

// 右から2番目の道の巡回ポイント
#define PATROLPOINT_R2_1 CVector( -20.0f,0.0f,-580.0f)
#define PATROLPOINT_R2_2 CVector(  20.0f,0.0f,-540.0f)
#define PATROLPOINT_R2_3 CVector(  20.0f,0.0f, 540.0f)
#define PATROLPOINT_R2_4 CVector( -20.0f,0.0f, 580.0f)
#define PATROLPOINT_R2_5 CVector(-300.0f,0.0f, 580.0f)


// 車両管理クラスのインスタンスを取得
CVehicleManager* CVehicleManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CVehicleManager::CVehicleManager()
	: mCarPopTime(CAR_POP_TIME)
	, mTruckPopTime(TRUCK_POP_TIME)
	, mPunishTruckPopTime(PUNISH_POP_TIME)
	, mpGarbageTruck(nullptr)
	, mpPunishTruck(nullptr)
{
	spInstance = this;
	// 生成場所
	mpSpawnZone = new CTrashVehicleSpawnZone();

	// 車のモデル
	mpCarModel = CResourceManager::Get<CModel>("Car");

	// 経路探索用のノードを作成
	CreateNavNodes();

	// 車両の作成
	CreateVehicle(CResourceManager::Get<CModel>("Car"),
		CResourceManager::Get<CModel>("GarbageTruck"),
		CResourceManager::Get<CModel>("BlackTruck"));
}

// デストラクタ
CVehicleManager::~CVehicleManager()
{
	if (spInstance == this)
	{
		spInstance = nullptr;
	}

	// 経路探索用のノードを削除
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// 左から1番目の道の
		// 巡回ポイントに配置したノードを全て削除
		auto itr = mPatrolPointsL1.begin();
		auto end = mPatrolPointsL1.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mPatrolPointsL1.erase(itr);
			delete del;
		}

		// 左から2番目の道の
		// 巡回ポイントに配置したノードを全て削除
		itr = mPatrolPointsL2.begin();
		end = mPatrolPointsL2.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mPatrolPointsL2.erase(itr);
			delete del;
		}

		// 右から1番目の道の
		// 巡回ポイントに配置したノードを全て削除
		itr = mPatrolPointsR1.begin();
		end = mPatrolPointsR1.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mPatrolPointsR1.erase(itr);
			delete del;
		}

		// 右から2番目の道の
		// 巡回ポイントに配置したノードを全て削除
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

//レイと全ての車両の衝突判定
bool CVehicleManager::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit, bool alreadyHit)
{
	// 衝突情報保存用
	CHitInfo tHit;
	// 衝突したかどうかフラグ
	bool isHit = alreadyHit;
	// 全ての車との衝突をチェック
	for (CCar* car : mCars)
	{
		// 無効なら次へ
		if (!car->IsEnable()) continue;

		// 車との衝突判定
		if (CCollider::CollisionRay(car->GetBodyCol(), start, end, &tHit))
		{
			// まだ他に衝突していない場合か、
			// 既に衝突しているコライダ―より近い場合は、
			if (!isHit || tHit.dist < hit->dist)
			{
				// 衝突情報を更新
				*hit = tHit;
				isHit = true;
			}
		}
	}
	// トラックが存在するかつ有効なら
	if (mpGarbageTruck != nullptr && mpGarbageTruck->IsEnable())
	{
		// トラックとの衝突判定
		if (CCollider::CollisionRay(mpGarbageTruck->GetBodyCol(), start, end, &tHit))
		{
			// まだ他に衝突していない場合か、
			// 既に衝突しているコライダ―より近い場合は、
			if (!isHit || tHit.dist < hit->dist)
			{
				// 衝突情報を更新
				*hit = tHit;
				isHit = true;
			}
		}
	}
	// お仕置きトラックが存在するかつ有効なら
	if (mpPunishTruck != nullptr && mpPunishTruck->IsEnable())
	{
		// お仕置きトラックとの衝突判定
		if (CCollider::CollisionRay(mpPunishTruck->GetBodyCol(), start, end, &tHit))
		{
			// まだ他に衝突していない場合か、
			// 既に衝突しているコライダ―より近い場合は、
			if (!isHit || tHit.dist < hit->dist)
			{
				// 衝突情報を更新
				*hit = tHit;
				isHit = true;
			}
		}
	}
	return isHit;
}

// レイと全ての車両との衝突判定（経路探索用）
bool CVehicleManager::NavCollisionRay(const CVector& start, const CVector& end, CHitInfo* hit, bool alreadyHit)
{
	// 衝突情報保存用
	CHitInfo tHit;
	// 衝突したかどうかフラグ
	bool isHit = alreadyHit;
	// 全ての車との衝突をチェック
	for (CCar* car : mCars)
	{
		// 無効なら次へ
		if (!car->IsEnable()) continue;
		// 移動中なら次へ
		if (car->IsMove()) continue;

		// 車との衝突判定
		if (CCollider::CollisionRay(car->GetNavCol(), start, end, &tHit))
		{
			// まだ他に衝突していない場合か、
			// 既に衝突しているコライダ―より近い場合は、
			if (!isHit || tHit.dist < hit->dist)
			{
				// 衝突情報を更新
				*hit = tHit;
				isHit = true;
			}
		}
	}
	// トラックが存在し、
	// 有効かつ移動していないなら
	if (mpGarbageTruck != nullptr &&
		mpGarbageTruck->IsEnable() &&
		!mpGarbageTruck->IsMove())
	{
		// トラックとの衝突判定
		if (CCollider::CollisionRay(mpGarbageTruck->GetNavCol(), start, end, &tHit))
		{
			// まだ他に衝突していない場合か、
			// 既に衝突しているコライダ―より近い場合は、
			if (!isHit || tHit.dist < hit->dist)
			{
				// 衝突情報を更新
				*hit = tHit;
				isHit = true;
			}
		}
	}
	// お仕置きトラックが存在し、
	// 有効かつ移動していないなら
	if (mpPunishTruck != nullptr &&
		mpPunishTruck->IsEnable()&&
		!mpPunishTruck->IsMove())
	{
		// お仕置きトラックとの衝突判定
		if (CCollider::CollisionRay(mpPunishTruck->GetNavCol(), start, end, &tHit))
		{
			// まだ他に衝突していない場合か、
			// 既に衝突しているコライダ―より近い場合は、
			if (!isHit || tHit.dist < hit->dist)
			{
				// 衝突情報を更新
				*hit = tHit;
				isHit = true;
			}
		}
	}
	return isHit;
}

// 更新
void CVehicleManager::Update()
{
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
	mCars.push_back(new CCar(car, CAR_LEFT_POS1, CAR_LEFT_ROTATION, CVehicleBase::ERoadType::eLeft1, mPatrolPointsL1));

	mpGarbageTruck = new CGarbageTruck(garbageTruck, CAR_RIGHT_POS1, CAR_RIGHT_ROTATION, CVehicleBase::ERoadType::eRight1, mPatrolPointsR1);
	mpPunishTruck = new CGarbageTruck(blackTruck, CAR_RIGHT_POS2, CAR_RIGHT_ROTATION, CVehicleBase::ERoadType::eRight2, mPatrolPointsR2);
}

// 経路探索用のノードを作成
void CVehicleManager::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// 左から1番目の道
		mPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_1, true));
		mPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_2, true));
		mPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_3, true));
		mPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_4, true));
		mPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_5, true));

		// 左から2番目の道
		mPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_1, true));
		mPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_2, true));
		mPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_3, true));
		mPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_4, true));
		mPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_5, true));

		// 右から1番目の道
		mPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_1, true));
		mPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_2, true));
		mPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_3, true));
		mPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_4, true));
		mPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_5, true));

		// 右から2番目の道
		mPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_1, true));
		mPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_2, true));
		mPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_3, true));
		mPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_4, true));
		mPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_5, true));
	}
}

// ステージ範囲外の車、トラックの更新、描画を止める
void CVehicleManager::HideVehicle()
{
	// 車
	for (CCar* car : mCars)
	{
		// ステージエリア外なら更新、描画を止める
		if (car->GetMoveEnd())
		{
			car->SetOnOff(false);
		}
	}
	// トラック
	if (mpGarbageTruck->GetMoveEnd())
	{
		mpGarbageTruck->SetOnOff(false);
	}
	// お仕置きトラック
	if (mpPunishTruck->GetMoveEnd())
	{
		mpPunishTruck->SetOnOff(false);
	}
}

// 乗り物を出現させる
void CVehicleManager::SpawnVehicle()
{
	/*

	// TODO：お仕置きだけ違う処理
	// お仕置きトラックの出現時間が0以下なら出現
	if (mPunishTruckPopTime <= 0.0f)
	{
		// お仕置きトラックが無効なら出現させる
		if (!mpPunishTruck->IsEnable())
		{
			// お仕置きトラックの生成位置
			CVector punishPopPos;
			// どの道にいるか
			CVehicleBase::ERoadType punishRoadType;
			// 巡回ポイント
			std::vector<CNavNode*> patrolPoints;

			// ランダムに場所を決定
			RandomDecidePopPosition(punishRoadType, punishPopPos);
			// 生成場所に車両がいたら抽選しなおし
			if (IsSpawnZone(punishRoadType))
			{
				// ランダムに場所を決定
				RandomDecidePopPosition(punishRoadType, punishPopPos);
			}
			//// 停止している車両がいる道で生成されるとき抽選しなおし
			//while (IsVehicle(punishRoadType))
			//{
			//	// ランダムに場所を決定
			//	RandomDecidePopPosition(punishRoadType, punishPopPos);
			//}
			CVector popRotation;	// 生成された時の方向

			// 左の道1なら
			if (punishRoadType == CVehicleBase::ERoadType::eLeft1)
			{
				// 左から1番目の道の巡回ポイント
				patrolPoints = mPatrolPointsL1;
				// 左の道用の回転を設定
				popRotation = CAR_LEFT_ROTATION;
			}
			// 左の道2なら
			else if (punishRoadType == CVehicleBase::ERoadType::eLeft2)
			{
				// 左から2番目の道の巡回ポイント
				patrolPoints = mPatrolPointsL2;
				// 左の道用の回転を設定
				popRotation = CAR_LEFT_ROTATION;
			}
			// 右の道1なら
			else if (punishRoadType == CVehicleBase::ERoadType::eRight1)
			{
				// 右から1番目の道の巡回ポイント
				patrolPoints = mPatrolPointsR1;
				// 右の道用の回転を設定
				popRotation = CAR_RIGHT_ROTATION;
			}
			// 右の道2なら
			else
			{
				// 右から2番目の道の巡回ポイント
				patrolPoints = mPatrolPointsR2;
				// 右の道用の回転を設定
				popRotation = CAR_RIGHT_ROTATION;
			}

			// 位置回転設定
			mpPunishTruck->Position(punishPopPos);
			mpPunishTruck->Rotation(popRotation);

			// どの道にいるかを設定
			mpPunishTruck->ChangeRoadType(punishRoadType);
			// 巡回ポイントのリストを設定
			mpPunishTruck->SetPatrolPoints(patrolPoints);
			// 変数をリセット
			mpPunishTruck->Reset();

			// 描画更新開始
			mpPunishTruck->SetOnOff(true);

			// 出現までの時間を設定しなおす
			mPunishTruckPopTime = PUNISH_POP_TIME;
		}
	}

	// 車の出現時間が0以下なら出現
	if (mCarPopTime <= 0.0f)
	{
		// 車の生成位置
		CVector carPopPos;
		// どの道にいるか
		CVehicleBase::ERoadType carRoadType;
		// 巡回ポイント
		std::vector<CNavNode*> patrolPoints;

		// ランダムに場所を決定
		RandomDecidePopPosition(carRoadType, carPopPos);
		// 生成場所に車両がいたら抽選しなおし
		if (IsSpawnZone(carRoadType))
		{
			// ランダムに場所を決定
			RandomDecidePopPosition(carRoadType, carPopPos);
		}
		//// 停止している車両がいる道で生成されるとき抽選しなおし
		//while (IsVehicle(carRoadType))
		//{
		//	// ランダムに場所を決定
		//	RandomDecidePopPosition(carRoadType, carPopPos);
		//}
		CVector popRotation;	// 生成されたときの方向
		bool isPop = false;	// 生成したかどうか

		// 左の道1なら
		if (carRoadType == CVehicleBase::ERoadType::eLeft1)
		{
			// 左から1番目の道の巡回ポイント
			patrolPoints = mPatrolPointsL1;
			// 左の道用の回転を設定
			popRotation = CAR_LEFT_ROTATION;
		}
		// 左の道2なら
		else if (carRoadType == CVehicleBase::ERoadType::eLeft2)
		{
			// 左から2番目の道の巡回ポイント
			patrolPoints = mPatrolPointsL2;
			// 左の道用の回転を設定
			popRotation = CAR_LEFT_ROTATION;
		}
		// 右の道1なら
		else if (carRoadType == CVehicleBase::ERoadType::eRight1)
		{
			// 右から1番目の道の巡回ポイント
			patrolPoints = mPatrolPointsR1;
			// 右の道用の回転を設定
			popRotation = CAR_RIGHT_ROTATION;
		}
		// 右の道2なら
		else
		{
			// 右から2番目の道の巡回ポイント
			patrolPoints = mPatrolPointsR2;
			// 右の道用の回転を設定
			popRotation = CAR_RIGHT_ROTATION;
		}

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
			// 巡回ポイントのリストを設定
			car->SetPatrolPoints(patrolPoints);
			// 変数をリセット
			car->Reset();

			// 描画更新開始
			car->SetOnOff(true);
			// 車を生成した
			isPop = true;
			break;
		}
		// 車を生成できていなければ
		if (!isPop)
		{
			// 新しい車を追加して描画更新開始
			mCars.push_back(new CCar(mpCarModel, carPopPos, popRotation, carRoadType, patrolPoints));
			mCars.back()->SetOnOff(true);
		}
		// 出現までの時間を設定しなおす
		mCarPopTime = CAR_POP_TIME;
	}

	*/

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
			// 巡回ポイント
			std::vector<CNavNode*> patrolPoints;

			truckPopPos = CAR_RIGHT_POS2;
			truckRoadType = CVehicleBase::ERoadType::eRight2;

			//// ランダムに場所を決定
			//RandomDecidePopPosition(truckRoadType, truckPopPos);
			//// 生成場所に車両がいたら抽選しなおし
			//if (IsSpawnZone(truckRoadType))
			//{
			//	// ランダムに場所を決定
			//	RandomDecidePopPosition(truckRoadType, truckPopPos);
			//}
			//// 停止している車両がいる道で生成されるとき抽選しなおし
			//while (IsVehicle(truckRoadType))
			//{
			//	// ランダムに場所を決定
			//	RandomDecidePopPosition(truckRoadType, truckPopPos);
			//}
			CVector popRotation;

			// 左の道1なら
			if (truckRoadType == CVehicleBase::ERoadType::eLeft1)
			{
				// 左から1番目の道の巡回ポイント
				patrolPoints = mPatrolPointsL1;
				// 左の道用の回転を設定
				popRotation = CAR_LEFT_ROTATION;
			}
			// 左の道2なら
			else if (truckRoadType == CVehicleBase::ERoadType::eLeft2)
			{
				// 左から2番目の道の巡回ポイント
				patrolPoints = mPatrolPointsL2;
				// 左の道用の回転を設定
				popRotation = CAR_LEFT_ROTATION;
			}
			// 右の道1なら
			else if (truckRoadType == CVehicleBase::ERoadType::eRight1)
			{
				// 右から1番目の道の巡回ポイント
				patrolPoints = mPatrolPointsR1;
				// 右の道用の回転を設定
				popRotation = CAR_RIGHT_ROTATION;
			}
			// 右の道2なら
			else
			{
				// 右から2番目の道の巡回ポイント
				patrolPoints = mPatrolPointsR2;
				// 右の道用の回転を設定
				popRotation = CAR_RIGHT_ROTATION;
			}

			// 位置回転設定
			mpGarbageTruck->Position(truckPopPos);
			mpGarbageTruck->Rotation(popRotation);

			// どの道にいるかを設定
			mpGarbageTruck->ChangeRoadType(truckRoadType);
			// 巡回ポイントのリストを設定
			mpGarbageTruck->SetPatrolPoints(patrolPoints);
			// 変数をリセット
			mpGarbageTruck->Reset();

			// 描画更新開始
			mpGarbageTruck->SetOnOff(true);

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
	mPunishTruckPopTime -= Times::DeltaTime();
}

// ランダムで車両を出現させる場所を決める
void CVehicleManager::RandomDecidePopPosition(CVehicleBase::ERoadType& roadType, CVector& popPos)
{
	// 0から3の乱数
	int randomNumber = Math::Rand(0, 3);

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

// 指定した道の生成場所に車両がいるかチェックする
bool CVehicleManager::IsSpawnZone(CVehicleBase::ERoadType roadType)
{
	// 左から一番目の道
	if (roadType == CVehicleBase::ERoadType::eLeft1)
	{
		return !mpSpawnZone->GetCanPops().IsLeft1CanPop;
	}
	// 左から二番目の道
	else if (roadType == CVehicleBase::ERoadType::eLeft2)
	{
		return !mpSpawnZone->GetCanPops().IsLeft2CanPop;
	}
	// 右から一番目の道
	else if (roadType == CVehicleBase::ERoadType::eRight1)
	{
		return !mpSpawnZone->GetCanPops().IsRight1CanPop;
	}
	// 右から二番目の道
	else
	{
		return !mpSpawnZone->GetCanPops().IsRight2CanPop;
	}
}
