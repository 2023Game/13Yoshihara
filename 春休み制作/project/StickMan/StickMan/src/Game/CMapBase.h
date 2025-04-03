#pragma once
#include "CFieldBase.h"

class CConnectWall;

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

	// 繋ぐ壁を生成
	void CreateConnectWall();

private:
	// 道が繋げれるかどうか
	bool mIsConnectT;
	bool mIsConnectB;
	bool mIsConnectL;
	bool mIsConnectR;

	// 繋ぐ壁のリスト
	std::vector<CConnectWall*> mpConnectWalls;
};