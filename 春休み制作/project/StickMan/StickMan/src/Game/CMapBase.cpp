#include "CMapBase.h"
#include "CConnectWall.h"

// 繋ぐ壁のオフセット座標
#define OFFSET_POS_TB CVector(0.0f,0.0f,95.0f)		// 上下
#define OFFSET_POS_LR CVector(65.0f,0.0f,0.0f)		// 左右

// 繋ぐ壁のオフセット回転
#define OFFSET_ROT_BOTTOM	CVector(0.0f,180.0f,0.0f)	// 下
#define OFFSET_ROT_LEFT		CVector(0.0f,-90.0f,0.0f)	// 左
#define OFFSET_ROT_RIGHT	CVector(0.0f,90.0f,0.0f)	// 右

// コンストラクタ
CMapBase::CMapBase(bool isUp, bool isDown, bool isLeft, bool isRight)
	: CFieldBase()
	, mIsConnectT(isUp)
	, mIsConnectB(isDown)
	, mIsConnectL(isLeft)
	, mIsConnectR(isRight)
{
}

// デストラクタ
CMapBase::~CMapBase()
{
}

// 繋ぐ壁を生成
void CMapBase::CreateConnectWall()
{
	// 上に繋げれない場合
	if (mIsConnectT)
	{
		CConnectWall* wallT = new CConnectWall();
		wallT->Position(Position() - OFFSET_POS_TB);
	}
	// 下に繋げれない場合
	if (mIsConnectB)
	{
		CConnectWall* wallB = new CConnectWall();
		wallB->Position(Position() + OFFSET_POS_TB);
		wallB->SetDefaultRot(OFFSET_ROT_BOTTOM);
	}
	// 左に繋げれない場合
	if (mIsConnectL)
	{
		CConnectWall* wallL = new CConnectWall();
		wallL->Position(Position() - OFFSET_POS_LR);
		wallL->SetDefaultRot(OFFSET_ROT_LEFT);
	}
	// 右に繋げれない場合
	if (mIsConnectR)
	{
		CConnectWall* wallR = new CConnectWall();
		wallR->Position(Position() + OFFSET_POS_LR);
		wallR->SetDefaultRot(OFFSET_ROT_RIGHT);
	}
}
