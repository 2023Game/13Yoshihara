#pragma once
#include "CModel.h"
#include "CColliderMesh.h"
#include "CVehicleBase.h"

class CCar;
class CGarbageTruck;
class CNavNode;

// 位置
#define VEHICLE_LEFT_POS1  CVector(-300.0f,0.0f, 500.0f)	// 左から1番
#define VEHICLE_LEFT_POS2  CVector( 300.0f,0.0f, 580.0f)	// 左から2番
#define VEHICLE_RIGHT_POS1 CVector( 300.0f,0.0f,-500.0f)	// 右から1番
#define VEHICLE_RIGHT_POS2 CVector(-300.0f,0.0f,-580.0f)	// 右から2番

// 回転
#define VEHICLE_LEFT_ROTATION  CVector(0.0f,270.0f,0.0f)	// 右方向
#define VEHICLE_RIGHT_ROTATION CVector(0.0f, 90.0f,0.0f)	// 左方向

// 車両管理クラス
class CVehicleManager
{
public:
	// 車両管理クラスのインスタンスを取得
	static CVehicleManager* Instance();
	// コンストラクタ
	CVehicleManager();
	// デストラクタ
	~CVehicleManager();

	/// <summary>
	/// レイと全ての車両との衝突判定
	/// </summary>
	/// <param name="start">レイの開始位置</param>
	/// <param name="end">レイの終了位置</param>
	/// <param name="hit">衝突情報返却用</param>
	/// <param name="alreadyHit">既に衝突しているか</param>
	/// <returns>衝突していたら、trueを返す</returns>
	bool CollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit, bool alreadyHit);

	/// <summary>
	/// レイと全ての車両との衝突判定（経路探索用）
	/// </summary>
	/// <param name="start">レイの開始位置</param>
	/// <param name="end">レイの終了位置</param>
	/// <param name="hit">衝突情報返却用</param>
	/// <param name="alreadyHit">既に衝突しているか</param>
	/// <returns>衝突していたら、trueを返す</returns>
	bool NavCollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit, bool alreadyHit);
	
	// 更新
	void Update();

	/// <summary>
	/// 指定した道に生成できるか
	/// </summary>
	/// <param name="roadType">どの道にいるか</param>
	/// <returns>trueならば、生成できる</returns>
	bool IsSpawn(CVehicleBase::ERoadType roadType);

	/// <summary>
	/// 道のX座標を取得
	/// </summary>
	/// <param name="road">道の指定
	/// </param>
	/// <returns></returns>
	float GetRoadPosX(CVehicleBase::ERoadType road);

	/// <summary>
	/// 指定した道の巡回ポイントを取得
	/// </summary>
	/// <param name="road">道の指定
	/// </param>
	/// <returns></returns>
	std::vector<CNavNode*> GetPatrolPoints(CVehicleBase::ERoadType road);
private:
	// 使用するトラックを全て生成
	void CreateVehicle(CModel* car, CModel* garbageTruck, CModel* blackTruck);
	// 経路探索用のノードを作成
	void CreateNavNodes();
	
	// ステージ範囲外の車、トラックの更新、描画を止める
	void HideVehicle();
	// 乗り物を出現させる
	void SpawnVehicle();

	// 車の出現までの時間をカウントダウン
	void CountCarPopTime();
	// トラックの出現までの時間をカウントダウン
	void CountTruckPopTime();
	// お仕置きトラックの出現までの時間をカウントダウン
	void CountBlackTruckPopTime();

	// それぞれの道に出現可能になるまでをカウントダウン
	void CountLeft1CanPopTime();	// 左から1番道路
	void CountLeft2CanPopTime();	// 左から2番道路
	void CountRight1CanPopTime();	// 右から1番道路
	void CountRight2CanPopTime();	// 右から2番道路

	/// <summary>
	/// ランダムで車両を出現させる場所を決める
	/// </summary>
	/// <param name="roadType">どの道にいるか</param>
	/// <param name="popPos">出現する場所</param>
	/// <returns>trueならば、生成場所が決定</returns>
	bool RandomDecidePopPosition(CVehicleBase::ERoadType& roadType, CVector& popPos);


	// 車両管理クラスのインスタンスへのポインタ
	static CVehicleManager* spInstance;

	CModel* mpCarModel;	// 車のモデル

	std::list<CCar*> mCars;		// 車のリスト
	CGarbageTruck* mpGarbageTruck;	// 通常トラック
	CGarbageTruck* mpPunishTruck;	// お仕置きトラック

	// 出現までの時間
	float mCarPopTime;			// 車
	float mTruckPopTime;		// トラック
	float mPunishTruckPopTime;	// お仕置きトラック

	// 次この道に出現可能になるまでの時間
	float mLeft1CanPopTime;	 // 左から1番道路
	float mLeft2CanPopTime;  // 左から2番道路
	float mRight1CanPopTime; // 右から1番道路
	float mRight2CanPopTime; // 右から2番道路

	// 巡回ポイントのリスト
	std::vector<CNavNode*> mPatrolPointsL1;	// 左から1番道路
	std::vector<CNavNode*> mPatrolPointsL2; // 左から2番道路
	std::vector<CNavNode*> mPatrolPointsR1; // 右から1番道路
	std::vector<CNavNode*> mPatrolPointsR2; // 右から2番道路
};