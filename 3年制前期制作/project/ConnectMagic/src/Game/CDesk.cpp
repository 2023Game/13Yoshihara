#include "CDesk.h"
#include "CMap_1.h"
#include "CConnectWall.h"

#define SCALE 1.5f
#define ROT	CVector(0.0f,90.0f,0.0f)

// 繋ぐ壁のオフセット座標
#define OFFSET_POS_TB CVector(0.0f,0.0f,95.0f)		// 上下
#define OFFSET_POS_LR CVector(65.0f,0.0f,0.0f)		// 左右

// 繋ぐ壁のオフセット回転
#define OFFSET_ROT_BOTTOM	CVector(0.0f,180.0f,0.0f)	// 下
#define OFFSET_ROT_LEFT		CVector(0.0f,-90.0f,0.0f)	// 左
#define OFFSET_ROT_RIGHT	CVector(0.0f,90.0f,0.0f)	// 右

// コンストラクタ
CDesk::CDesk()
	: mIsConnectT(false)
	, mIsConnectB(false)
	, mIsConnectL(false)
	, mIsConnectR(false)
{
	mpModel = CResourceManager::Get<CModel>("Desk");

	Scale(Scale() * SCALE);
	Rotation(ROT);

	// コライダーを生成
	CreateCol("DeskCol", "", "");

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CDesk::~CDesk()
{
}

// 上に繋がるかを設定
void CDesk::SetIsConnectTop(bool isTop)
{
	mIsConnectT = isTop;
}

// 下に繋がるかを設定
void CDesk::SetIsConnectBottom(bool isBottom)
{
	mIsConnectB = isBottom;
}

// 左に繋がるかを設定
void CDesk::SetIsConnectLeft(bool isLeft)
{
	mIsConnectL = isLeft;
}

// 右に繋がるかを設定
void CDesk::SetIsConnectRight(bool isRight)
{
	mIsConnectR = isRight;
}

// フィールドオブジェクトを生成
void CDesk::CreateFieldObjects()
{
	//mpMap = new CMap_1();
	//// 親に設定
	//mpMap->SetParent(this);
}

// 繋ぐ壁を生成
void CDesk::CreateConnectWall()
{
	// 上に繋げれない場合
	if (mIsConnectT)
	{
		CConnectWall* wallT = new CConnectWall();
		// 親に設定
		wallT->SetParent(this);
		wallT->Position(Position() - OFFSET_POS_TB);
	}
	// 下に繋げれない場合
	if (mIsConnectB)
	{
		CConnectWall* wallB = new CConnectWall();
		// 親に設定
		wallB->SetParent(this);
		wallB->Position(Position() + OFFSET_POS_TB);
		wallB->SetDefaultRot(OFFSET_ROT_BOTTOM);
	}
	// 左に繋げれない場合
	if (mIsConnectL)
	{
		CConnectWall* wallL = new CConnectWall();
		// 親に設定
		wallL->SetParent(this);
		wallL->Position(Position() - OFFSET_POS_LR);
		wallL->SetDefaultRot(OFFSET_ROT_LEFT);
	}
	// 右に繋げれない場合
	if (mIsConnectR)
	{
		CConnectWall* wallR = new CConnectWall();
		// 親に設定
		wallR->SetParent(this);
		wallR->Position(Position() + OFFSET_POS_LR);
		wallR->SetDefaultRot(OFFSET_ROT_RIGHT);
	}
}