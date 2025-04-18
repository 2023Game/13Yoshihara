#include "CTrashVehicleManager.h"
#include "CTrashCar.h"
#include "CGarbageTruck.h"
#include "CPunisherTruck.h"
#include <random>
#include "Maths.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "Primitive.h"
#include "CTrashPlayer.h"
#include "CTrashField.h"
#include "CTrashEnemyManager.h"

CTrashVehicleManager* CTrashVehicleManager::spInstance = nullptr;

// 出現までの時間
#define CAR_POP_TIME 3.0f		// 車
#define TRUCK_POP_TIME 1.0f		// トラック
#define PUNISH_POP_TIME 3.0f	// お仕置きトラック

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

// 次、道に出現可能になるまでの時間
#define NEXT_CAN_POP_TIME 2.0f

// 車の最大数
#define CARS_MAX_NUM 2

// 車両管理クラスのインスタンスを取得
CTrashVehicleManager* CTrashVehicleManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CTrashVehicleManager::CTrashVehicleManager()
	: mCarPopTime(CAR_POP_TIME)
	, mTruckPopTime(TRUCK_POP_TIME)
	, mPunishTruckPopTime(PUNISH_POP_TIME)
	, mpGarbageTruck(nullptr)
	, mpPunishTruck(nullptr)
{
	spInstance = this;

	// 車両のモデル
	mpCarModel = CResourceManager::Get<CModel>("Car");
	mpGarbageTruckModel = CResourceManager::Get<CModel>("GarbageTruck");
	mpPunishTruckModel = CResourceManager::Get<CModel>("BlackTruck");

	// 経路探索用のノードを作成
	CreateNavNodes();

	// 車両の作成
	CreateVehicle(mpCarModel, mpGarbageTruckModel, mpPunishTruckModel);
}

// デストラクタ
CTrashVehicleManager::~CTrashVehicleManager()
{
	// 車のモデルがnullptrでないなら
	if (mpCarModel != nullptr)
	{
		// nullptrにする
		mpCarModel = nullptr;
	}
	// 通常トラックのモデルがnullptrでないなら
	if (mpGarbageTruckModel != nullptr)
	{
		// nullptrにする
		mpGarbageTruckModel = nullptr;
	}
	// お仕置きトラックのモデルがnullptrでないなら
	if (mpPunishTruckModel != nullptr)
	{
		// nullptrにする
		mpPunishTruckModel = nullptr;
	}

	// リストを空にする
	{
		auto itr = mpCars.begin();
		auto end = mpCars.end();
		while (itr != end)
		{
			CTrashCar* del = *itr;
			itr = mpCars.erase(itr);
			delete del;
		}
	}
	SAFE_DELETE(mpGarbageTruck);	// 通常トラック
	SAFE_DELETE(mpPunishTruck);		// お仕置きトラック

	// インスタンスと削除している車両管理クラスが同一なら削除
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
		auto itr = mpPatrolPointsL1.begin();
		auto end = mpPatrolPointsL1.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mpPatrolPointsL1.erase(itr);
			delete del;
		}

		// 左から2番目の道の
		// 巡回ポイントに配置したノードを全て削除
		itr = mpPatrolPointsL2.begin();
		end = mpPatrolPointsL2.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mpPatrolPointsL2.erase(itr);
			delete del;
		}

		// 右から1番目の道の
		// 巡回ポイントに配置したノードを全て削除
		itr = mpPatrolPointsR1.begin();
		end = mpPatrolPointsR1.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mpPatrolPointsR1.erase(itr);
			delete del;
		}

		// 右から2番目の道の
		// 巡回ポイントに配置したノードを全て削除
		itr = mpPatrolPointsR2.begin();
		end = mpPatrolPointsR2.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mpPatrolPointsR2.erase(itr);
			delete del;
		}
	}
}

//レイと全ての車両の衝突判定
bool CTrashVehicleManager::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit, bool alreadyHit)
{
	// 衝突情報保存用
	CHitInfo tHit;
	// 衝突したかどうかフラグ
	bool isHit = alreadyHit;
	// 全ての車との衝突をチェック
	for (CTrashCar* car : mpCars)
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
bool CTrashVehicleManager::NavCollisionRay(const CVector& start, const CVector& end, CHitInfo* hit, bool alreadyHit)
{
	// 衝突情報保存用
	CHitInfo tHit;
	// 衝突したかどうかフラグ
	bool isHit = alreadyHit;
	// 全ての車との衝突をチェック
	for (CTrashCar* car : mpCars)
	{
		// 無効なら次へ
		if (!car->IsEnable()) continue;
		// 移動中なら次へ
		if (car->IsMove()) continue;

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
	// トラックが存在し、
	// 有効かつ移動していないなら
	if (mpGarbageTruck != nullptr &&
		mpGarbageTruck->IsEnable() &&
		!mpGarbageTruck->IsMove())
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
	// お仕置きトラックが存在し、
	// 有効かつ移動していないなら
	if (mpPunishTruck != nullptr &&
		mpPunishTruck->IsEnable()&&
		!mpPunishTruck->IsMove())
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

// カメラと衝突する車両のコライダ―を設定する
void CTrashVehicleManager::SetCameraCollision(CCamera* camera)
{
	// 全ての車両との衝突を追加
	for (CTrashCar* car : mpCars)
	{
		// リストに追加
		camera->AddCollider(car->GetBodyCol());
	}
	camera->AddCollider(mpGarbageTruck->GetBodyCol());
	camera->AddCollider(mpPunishTruck->GetBodyCol());
}

// 更新
void CTrashVehicleManager::Update()
{
	HideVehicle();

	// お仕置き用の敵が出現していないなら
	CTrashEnemyManager* enemyMgr = CTrashEnemyManager::Instance();
	if (!enemyMgr->GetPopPunisherEnemy())
	{
		// 出現までの時間をカウント
		CountCarPopTime();
		CountTruckPopTime();
		CountBlackTruckPopTime();
		// それぞれの道の生成可能までの時間をカウント
		CountLeft1CanPopTime();
		CountLeft2CanPopTime();
		CountRight1CanPopTime();
		CountRight2CanPopTime();
	}

	// 出現までの時間が0以下なら出現
	SpawnVehicle();
}

// 道のX座標を取得
float CTrashVehicleManager::GetRoadPosX(ERoadType road)
{
	// 左から1番目の道
	if (road == ERoadType::eLeft1)
	{
		return PATROLPOINT_L1_2.X();
	}
	// 左から2番目の道
	else if (road == ERoadType::eLeft2)
	{
		return PATROLPOINT_L2_2.X();
	}
	// 右から1番目の道
	else if (road == ERoadType::eRight1)
	{
		return PATROLPOINT_R1_2.X();
	}
	// 右から2番目の道
	else
	{
		return PATROLPOINT_R2_2.X();
	}
}

// 指定した道の巡回ポイントを取得
std::vector<CNavNode*> CTrashVehicleManager::GetPatrolPoints(ERoadType road)
{
	// 左から1番目の道
	if (road == ERoadType::eLeft1)
	{
		return mpPatrolPointsL1;
	}
	// 左から2番目の道
	else if (road == ERoadType::eLeft2)
	{
		return mpPatrolPointsL2;
	}
	// 右から1番目の道
	else if (road == ERoadType::eRight1)
	{
		return mpPatrolPointsR1;
	}
	// 右から2番目の道
	else
	{
		return mpPatrolPointsR2;
	}
}

// お仕置き用のトラックが生成されているか
bool CTrashVehicleManager::GetPopPunisherTruck() const
{
	return mpPunishTruck->IsEnable();
}

// 使用するトラックを全て生成
void CTrashVehicleManager::CreateVehicle(CModel* car, CModel* garbageTruck, CModel* blackTruck)
{
	for (int i = 0; i < CARS_MAX_NUM; i++)
	{
		mpCars.push_back(new CTrashCar(car, VEHICLE_LEFT_POS1, VEHICLE_RIGHT_ROTATION, ERoadType::eLeft1, mpPatrolPointsL1));
	}

	mpGarbageTruck = new CGarbageTruck(garbageTruck, VEHICLE_LEFT_POS1, VEHICLE_RIGHT_ROTATION, ERoadType::eLeft1, mpPatrolPointsR1);
	mpPunishTruck = new CPunisherTruck(blackTruck, VEHICLE_LEFT_POS1, VEHICLE_RIGHT_ROTATION, ERoadType::eLeft1, mpPatrolPointsR2);
}

// 経路探索用のノードを作成
void CTrashVehicleManager::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// 左から1番目の道
		mpPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_1, true));
		mpPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_2, true));
		mpPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_3, true));
		mpPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_4, true));
		mpPatrolPointsL1.push_back(new CNavNode(PATROLPOINT_L1_5, true));

		// 左から2番目の道
		mpPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_1, true));
		mpPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_2, true));
		mpPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_3, true));
		mpPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_4, true));
		mpPatrolPointsL2.push_back(new CNavNode(PATROLPOINT_L2_5, true));

		// 右から1番目の道
		mpPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_1, true));
		mpPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_2, true));
		mpPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_3, true));
		mpPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_4, true));
		mpPatrolPointsR1.push_back(new CNavNode(PATROLPOINT_R1_5, true));

		// 右から2番目の道
		mpPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_1, true));
		mpPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_2, true));
		mpPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_3, true));
		mpPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_4, true));
		mpPatrolPointsR2.push_back(new CNavNode(PATROLPOINT_R2_5, true));
	}
}

// 移動が終了した車、トラックの更新、描画を止める
void CTrashVehicleManager::HideVehicle()
{
	// 車
	for (CTrashCar* car : mpCars)
	{
		// 移動終了ならなら更新、描画を止める
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
void CTrashVehicleManager::SpawnVehicle()
{	
	// 車の出現時間が0以下なら出現
	if (mCarPopTime <= 0.0f)
	{
		// 車の生成位置
		CVector carPopPos;
		// 生成時の回転
		CVector popRotation;
		// どの道にいるか
		ERoadType carRoadType;
		// 巡回ポイント
		std::vector<CNavNode*> patrolPoints;

		// ランダムに場所を決定
		// 決定出来たら中身を処理
		if (RandomDecidePopPosition(carRoadType, carPopPos))
		{
			// 左の道1なら
			if (carRoadType == ERoadType::eLeft1)
			{
				// 左から1番目の道の巡回ポイント
				patrolPoints = mpPatrolPointsL1;
				// 右向きの回転を設定
				popRotation = VEHICLE_RIGHT_ROTATION;
			}
			// 左の道2なら
			else if (carRoadType == ERoadType::eLeft2)
			{
				// 左から2番目の道の巡回ポイント
				patrolPoints = mpPatrolPointsL2;
				// 左向きの回転を設定
				popRotation = VEHICLE_LEFT_ROTATION;
			}
			// 右の道1なら
			else if (carRoadType == ERoadType::eRight1)
			{
				// 右から1番目の道の巡回ポイント
				patrolPoints = mpPatrolPointsR1;
				// 左向きの回転を設定
				popRotation = VEHICLE_LEFT_ROTATION;
			}
			// 右の道2なら
			else
			{
				// 右から2番目の道の巡回ポイント
				patrolPoints = mpPatrolPointsR2;
				// 右向きの回転を設定
				popRotation = VEHICLE_RIGHT_ROTATION;
			}

			// 既に有効になっていない車を有効にする
			for (CTrashCar* car : mpCars)
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
				// 出現までの時間を設定しなおす
				mCarPopTime = CAR_POP_TIME;
				break;
			}
		}
	}

	// トラックの出現時間が0以下なら出現
	// 死んでいたら出現させない
	if (mTruckPopTime <= 0.0f&&
		!mpGarbageTruck->IsDeath())
	{
		// トラックが無効なら出現させる
		// お仕置きトラックが出ているときは出現させない
		if (!mpGarbageTruck->IsEnable() &&
			!mpPunishTruck->IsEnable())
		{
			// トラックの生成位置
			CVector truckPopPos;
			// 生成時の回転
			CVector popRotation;
			// どの道にいるか
			ERoadType truckRoadType;
			// 巡回ポイント
			std::vector<CNavNode*> patrolPoints;

			// ランダムに場所を決定
			// 決定出来たら中身を処理
			if (RandomDecidePopPosition(truckRoadType, truckPopPos))
			{
				// 左の道1なら
				if (truckRoadType == ERoadType::eLeft1)
				{
					// 左から1番目の道の巡回ポイント
					patrolPoints = mpPatrolPointsL1;
					// 右向きの回転を設定
					popRotation = VEHICLE_RIGHT_ROTATION;
				}
				// 左の道2なら
				else if (truckRoadType == ERoadType::eLeft2)
				{
					// 左から2番目の道の巡回ポイント
					patrolPoints = mpPatrolPointsL2;
					// 左向きの回転を設定
					popRotation = VEHICLE_LEFT_ROTATION;
				}
				// 右の道1なら
				else if (truckRoadType == ERoadType::eRight1)
				{
					// 右から1番目の道の巡回ポイント
					patrolPoints = mpPatrolPointsR1;
					// 左向きの回転を設定
					popRotation = VEHICLE_LEFT_ROTATION;
				}
				// 右の道2なら
				else
				{
					// 右から2番目の道の巡回ポイント
					patrolPoints = mpPatrolPointsR2;
					// 右向きの回転を設定
					popRotation = VEHICLE_RIGHT_ROTATION;
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

	// お仕置きトラックの出現時間が0以下なら出現
	if (mPunishTruckPopTime <= 0.0f)
	{
		// お仕置きトラックが無効なら出現させる
		if (!mpPunishTruck->IsEnable())
		{
			// お仕置きトラックの生成位置
			CVector punishPopPos;
			// 生成時の回転
			CVector popRotation;
			// どの道にいるか
			ERoadType punishRoadType;
			// 巡回ポイント
			std::vector<CNavNode*> patrolPoints;

			// プレイヤーの場所によって生成場所を決定
			CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
			CVector playerPos = player->Position();
			// プレイヤーのX座標がプラス側のエリア外なら
			if (playerPos.X() >= ROAD_X_AREA)
			{
				// 右から1番目の道
				punishRoadType = ERoadType::eRight1;
				// 右から1番目の道の巡回ポイント
				patrolPoints = mpPatrolPointsR1;
				// 右から1番目の道の座標を設定
				punishPopPos = VEHICLE_RIGHT_POS1;
				// 左向きの回転を設定
				popRotation = VEHICLE_LEFT_ROTATION;
			}
			// プレイヤーのX座標がマイナス側のエリア外なら
			else if (playerPos.X() <= -ROAD_X_AREA)
			{
				// 左から1番目の道
				punishRoadType = ERoadType::eLeft1;
				// 左から1番目の道の巡回ポイント
				patrolPoints = mpPatrolPointsL1;
				// 左から1番目の道の座標を設定
				punishPopPos = VEHICLE_LEFT_POS1;
				// 右向きの回転を設定
				popRotation = VEHICLE_RIGHT_ROTATION;
			}
			// エリア外でない場合
			else
			{
				// 出現までの時間を設定しなおして生成しない
				mPunishTruckPopTime = PUNISH_POP_TIME;
				return;
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
}

// 車の出現までの時間をカウント
void CTrashVehicleManager::CountCarPopTime()
{
	mCarPopTime -= Times::DeltaTime();
}

// トラックの出現までの時間をカウント
void CTrashVehicleManager::CountTruckPopTime()
{
	mTruckPopTime -= Times::DeltaTime();
}

// お仕置きトラックの出現までの時間をカウント
void CTrashVehicleManager::CountBlackTruckPopTime()
{
	// プレイヤーを取得
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// プレイヤーがエリア外ならカウント
	if (player->AreaOutX())
	{
		mPunishTruckPopTime -= Times::DeltaTime();
	}
	// プレイヤーがエリア内なら時間をリセット
	else
	{
		mPunishTruckPopTime = PUNISH_POP_TIME;
	}
}

// それぞれの道に出現可能になるまでをカウントダウン
// 左から1番道路
void CTrashVehicleManager::CountLeft1CanPopTime()
{
	mLeft1CanPopTime -= Times::DeltaTime();
}
// 左から2番道路
void CTrashVehicleManager::CountLeft2CanPopTime()
{
	mLeft2CanPopTime -= Times::DeltaTime();
}
// 右から1番道路
void CTrashVehicleManager::CountRight1CanPopTime()
{
	mRight1CanPopTime -= Times::DeltaTime();
}
// 右から2番道路
void CTrashVehicleManager::CountRight2CanPopTime()
{
	mRight2CanPopTime -= Times::DeltaTime();
}

// ランダムで車両を出現させる場所を決める
bool CTrashVehicleManager::RandomDecidePopPosition(ERoadType& roadType, CVector& popPos)
{
	// 生成可能な道の数
	int canPopRoadNum = 0;
	// それぞれが割り当てされる番号
	// 割り当てされなければ、-1
	int left1RoadNum = -1;
	int left2RoadNum = -1;
	int right1RoadNum = -1;
	int right2RoadNum = -1;
	// 生成が可能なら
	if (IsSpawn(ERoadType::eLeft1))
	{
		// 生成可能な道の数を増やし
		canPopRoadNum++;
		// 番号を割り当てる
		left1RoadNum = canPopRoadNum;
	}
	if (IsSpawn(ERoadType::eLeft2))
	{
		canPopRoadNum++;
		left2RoadNum = canPopRoadNum;
	}
	if (IsSpawn(ERoadType::eRight1))
	{
		canPopRoadNum++;
		right1RoadNum = canPopRoadNum;
	}
	if (IsSpawn(ERoadType::eRight2))
	{
		canPopRoadNum++;
		right2RoadNum = canPopRoadNum;
	}

	// 生成場所未決定
	if (canPopRoadNum == 0) return false;
	// 1から生成可能な道の数の乱数
	int randomNumber = Math::Rand(1, canPopRoadNum);

	// 乱数と割り当てた番号が一致すればその場所へ生成
	if (randomNumber == left1RoadNum)
	{
		// 道の種類を設定
		roadType = ERoadType::eLeft1;
		// 生成座標を設定
		popPos = VEHICLE_LEFT_POS1;
		// この道の次に出現可能になるまでの時間を設定
		mLeft1CanPopTime = NEXT_CAN_POP_TIME;
	}
	else if (randomNumber == left2RoadNum)
	{
		roadType = ERoadType::eLeft2;
		popPos = VEHICLE_LEFT_POS2;
		mLeft2CanPopTime = NEXT_CAN_POP_TIME;
	}
	else if (randomNumber == right1RoadNum)
	{
		roadType = ERoadType::eRight1;
		popPos = VEHICLE_RIGHT_POS1;
		mRight1CanPopTime = NEXT_CAN_POP_TIME;
	}
	else if (randomNumber == right2RoadNum)
	{
		roadType = ERoadType::eRight2;
		popPos = VEHICLE_RIGHT_POS2;
		mRight2CanPopTime = NEXT_CAN_POP_TIME;
	}

	// 生成場所決定
	return true;
}

// 指定した道に車を生成できるか
bool CTrashVehicleManager::IsSpawn(ERoadType roadType)
{
	// 左から一番目の道
	if (roadType == ERoadType::eLeft1)
	{
		return mLeft1CanPopTime <= 0.0f;
	}
	// 左から二番目の道
	else if (roadType == ERoadType::eLeft2)
	{
		return mLeft2CanPopTime <= 0.0f;
	}
	// 右から一番目の道
	else if (roadType == ERoadType::eRight1)
	{
		return mRight1CanPopTime <= 0.0f;
	}
	// 右から二番目の道
	else
	{
		return mRight2CanPopTime <= 0.0f;
	}
}
