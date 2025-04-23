#include "CConnectPoint.h"

// コンストラクタ
CConnectPoint::CConnectPoint()
	: CBillBoardImage("Effect/ConnectPoint.png", ETag::eConnectPoint, ETaskPauseType::eGame)
{
}

// デストラクタ
CConnectPoint::~CConnectPoint()
{
}

// 更新
void CConnectPoint::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();
}