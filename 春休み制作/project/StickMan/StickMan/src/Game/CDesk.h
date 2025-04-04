#pragma once
#include "CFieldBase.h"

class CConnectWall;

// 机
class CDesk : public CFieldBase
{
public:
	// コンストラクタ
	CDesk();
	// デストラクタ
	~CDesk();

	// 繋がるかを設定
	void SetIsConnect(bool isTop, bool isBottom, bool isLeft, bool isRight);

	// 繋ぐ壁を生成
	void CreateConnectWall();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects();

	// 机上のフィールドを生成
	CFieldBase* mpMap;

	// 道が繋げれるかどうか
	bool mIsConnectT;
	bool mIsConnectB;
	bool mIsConnectL;
	bool mIsConnectR;
	// 繋ぐ壁のリスト
	std::vector<CConnectWall*> mpConnectWalls;
};