#pragma once
#include "CModel.h"
class CCar;
class CGarbageTruck;

class CCarAndTruckManager
{
public:
	CCarAndTruckManager();
	~CCarAndTruckManager();
	
	void Update();
	// 使用する車とトラックを全て生成
	void CreateCarAndTruck(CModel* car, CModel* garbageTruck);
	// ステージ範囲外の車、トラックの更新、描画を止める
	void HideCarAndTruck();
private:
	std::vector<CCar*> mCars;	// 車のリスト
	CGarbageTruck* mpGarbageTruck;// 通常トラック
	CGarbageTruck* mpBlackTruck;	// お仕置きトラック
};