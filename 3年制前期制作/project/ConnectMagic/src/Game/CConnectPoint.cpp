#include "CConnectPoint.h"
#include "CConnectObject.h"

// コンストラクタ
CConnectPoint::CConnectPoint(CConnectObject* connectObj)
	: CBillBoardImage("Effect/ConnectPoint.png", ETag::eConnectPoint, ETaskPauseType::eGame)
	, mpConnectObj(connectObj)
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

// 接続部がついているオブジェクトを取得する
CConnectObject* CConnectPoint::GetConnectObj()
{
	return mpConnectObj;
}
