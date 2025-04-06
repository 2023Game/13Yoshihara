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

	// 上に繋がるかを設定
	void SetIsConnectTop(bool isTop);
	// 下に繋がるかを設定
	void SetIsConnectBottom(bool isBottom);
	// 左に繋がるかを設定
	void SetIsConnectLeft(bool isLeft);
	// 右に繋がるかを設定
	void SetIsConnectRight(bool isRight);

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