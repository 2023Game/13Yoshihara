#include "CMapBase.h"
#include "CBlockWall.h"

#define SCALE 10.0f
// 塞ぐ壁のオフセット座標
#define OFFSET_POS_SHORT 50.0f	// 長い方
#define OFFSET_POS_LONG 95.0f	// 短い方

// 上下の塞ぐ壁のオフセット回転
#define OFFSET_ROT_UpDown CVector(0.0f,90.0f,0.0f)

// コンストラクタ
CMapBase::CMapBase(bool isUp, bool isDown, bool isLeft, bool isRight)
	: CFieldBase()
	, mIsConnectUp(isUp)
	, mIsConnectDown(isDown)
	, mIsConnectL(isLeft)
	, mIsConnectR(isRight)
{
	Scale(Scale() * SCALE);
}

// デストラクタ
CMapBase::~CMapBase()
{
}

// 塞ぐ壁を生成
void CMapBase::CreateBlockWall()
{
	// 上に繋げれない場合
	if (!mIsConnectUp)
	{
		CBlockWall* wallL = new CBlockWall();
		CBlockWall* wallR = new CBlockWall();
		wallL->Position(Position() + CVector(-OFFSET_POS_SHORT, 0.0f, -OFFSET_POS_LONG));
		wallR->Position(Position() + CVector( OFFSET_POS_SHORT, 0.0f, -OFFSET_POS_LONG));
		wallL->Scale(Scale());
		wallR->Scale(Scale());
		wallL->Rotation(OFFSET_ROT_UpDown);
		wallR->Rotation(OFFSET_ROT_UpDown);
	}
	// 下に繋げれない場合
	if (!mIsConnectDown)
	{
		CBlockWall* wallL = new CBlockWall();
		CBlockWall* wallR = new CBlockWall();
		wallL->Position(Position() + CVector(-OFFSET_POS_SHORT, 0.0f, OFFSET_POS_LONG));
		wallR->Position(Position() + CVector( OFFSET_POS_SHORT, 0.0f, OFFSET_POS_LONG));
		wallL->Scale(Scale());
		wallR->Scale(Scale());
		wallL->Rotation(OFFSET_ROT_UpDown);
		wallR->Rotation(OFFSET_ROT_UpDown);
	}
	// 左に繋げれない場合
	if (!mIsConnectL)
	{
		CBlockWall* wallU = new CBlockWall();
		CBlockWall* wallD = new CBlockWall();
		wallU->Position(Position() + CVector(-OFFSET_POS_LONG, 0.0f, -OFFSET_POS_SHORT));
		wallD->Position(Position() + CVector(-OFFSET_POS_LONG, 0.0f, OFFSET_POS_SHORT));
		wallU->Scale(Scale());
		wallD->Scale(Scale());
	}
	// 右に繋げれない場合
	if (!mIsConnectR)
	{
		CBlockWall* wallU = new CBlockWall();
		CBlockWall* wallD = new CBlockWall();
		wallU->Position(Position() + CVector(OFFSET_POS_LONG, 0.0f, -OFFSET_POS_SHORT));
		wallD->Position(Position() + CVector(OFFSET_POS_LONG, 0.0f,  OFFSET_POS_SHORT));
		wallU->Scale(Scale());
		wallD->Scale(Scale());
	}
}
