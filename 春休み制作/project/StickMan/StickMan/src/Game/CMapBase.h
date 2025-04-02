#pragma once
#include "CFieldBase.h"

class CBlockWall;

class CMapBase : public CFieldBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="isUp">上側につなげれるか</param>
	/// <param name="isDown">下側につなげれるか</param>
	/// <param name="isLeft">左側につなげれるか</param>
	/// <param name="isRight">右側につなげれるか</param>
	CMapBase(bool isUp, bool isDown, bool isLeft, bool isRight);
	// デストラクタ
	~CMapBase();

	// 塞ぐ壁を生成
	void CreateBlockWall();

private:
	// 道が繋げれるかどうか
	bool mIsConnectUp;
	bool mIsConnectDown;
	bool mIsConnectL;
	bool mIsConnectR;

	// 塞ぐ壁のリスト
	std::vector<CBlockWall*> mpBlockWalls;

	// テーブル

};