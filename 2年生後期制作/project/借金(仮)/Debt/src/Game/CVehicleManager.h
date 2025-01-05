#pragma once
#include "CModel.h"
#include "CColliderMesh.h"
#include "CVehicleBase.h"

class CCar;
class CGarbageTruck;
class CTrashVehicleSpawnZone;

// 位置
#define CAR_LEFT_POS1 CVector(-60.0f,0.0f,300.0f)	// 左から1番
#define CAR_LEFT_POS2 CVector(-20.0f,0.0f,300.0f)	// 左から2番
#define CAR_RIGHT_POS1 CVector(60.0f,0.0f,-300.0f)	// 右から1番
#define CAR_RIGHT_POS2 CVector(20.0f,0.0f,-300.0f)	// 右から2番

// 回転
#define CAR_LEFT_ROTATION CVector(0.0f,180.0f,0.0f)	// 左道路の方向
#define CAR_RIGHT_ROTATION CVector(0.0f,0.0f,0.0f)	// 右道路の方向

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
	// 使用するトラックを全て生成
	void CreateVehicle(CModel* car, CModel* garbageTruck, CModel* blackTruck);
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

	// ランダムで車両を出現させる場所を決める

	/// <summary>
	/// ランダムで車両を出現させる場所を決める
	/// </summary>
	/// <param name="roadType">どの道にいるか</param>
	/// <param name="popPos">出現する場所</param>
	void RandomDecidePopPosition(CVehicleBase::ERoadType& roadType, CVector& popPos);

	/// <summary>
	/// 指定した道に停止している車両がいるかチェックする
	/// </summary>
	/// <param name="roadType">どの道にいるか</param>
	/// <returns>trueならば、いる</returns>
	bool IsVehicle(CVehicleBase::ERoadType roadType);
	/// <summary>
	/// 指定した道の生成場所に車両がいるかチェックする
	/// </summary>
	/// <param name="roadType">どの道にいるか</param>
	/// <returns>trueならば、いる</returns>
	bool IsSpawnZone(CVehicleBase::ERoadType roadType);

private:
	// 車両管理クラスのインスタンスへのポインタ
	static CVehicleManager* spInstance;
	// 生成場所に車がいるか判断するクラス
	CTrashVehicleSpawnZone* mpSpawnZone;

	CModel* mpCarModel;	// 車のモデル

	std::list<CCar*> mCars;		// 車のリスト
	CGarbageTruck* mpGarbageTruck;	// 通常トラック
	CGarbageTruck* mpPunishTruck;	// お仕置きトラック

	// 出現までの時間
	float mCarPopTime;			// 車
	float mTruckPopTime;		// トラック
	float mPunishTruckPopTime;	// お仕置きトラック
};