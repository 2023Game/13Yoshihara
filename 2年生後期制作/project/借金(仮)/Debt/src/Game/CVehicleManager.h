#pragma once
#include "CModel.h"
class CCar;
class CGarbageTruck;

class CVehicleManager
{
public:
	CVehicleManager();
	~CVehicleManager();
	
	void Update();
	// 使用する車とトラックを全て生成
	void CreateVehicle(CModel* car, CModel* garbageTruck, CModel* blackTruck);
	// ステージ範囲外の車、トラックの更新、描画を止める
	void HideVehicle();
	// 乗り物を出現させる
	void SpawnVehicle();

	// 車の出現までの時間をカウント
	void CountCarPopTime();

	// トラックの出現までの時間をカウント
	void CountTruckPopTime();

	// お仕置きトラックの出現までの時間をカウント
	void CountBlackTruckPopTime();

private:
	std::vector<CCar*> mCars;		// 車のリスト
	CGarbageTruck* mpGarbageTruck;	// 通常トラック
	CGarbageTruck* mpBlackTruck;	// お仕置きトラック

	// 出現までの時間
	float mCarPopTime;			// 車
	float mTruckPopTime;		// トラック
	float mBlackTruckPopTime;	// お仕置きトラック
};