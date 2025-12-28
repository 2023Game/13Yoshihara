#include "CConnectPoint.h"
#include "CConnectObject.h"
#include "CConnectTarget.h"

// コンストラクタ
CConnectPoint::CConnectPoint(CConnectObject* connectObj)
	: CBillBoardImage("Effect/ConnectPoint.png", ETag::eConnectPoint, ETaskPauseType::eGame)
	, mpConnectObj(connectObj)
{
	// 優先度を設定
	SetPriority(ETaskPriority::eConnectPoint);
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

	if (mpConnectObj != nullptr)
	{
		// 持ち主の座標へ
		Position(mpConnectObj->GetTarget()->Position());
	}
}

// 接続部がついているオブジェクトを取得する
CConnectObject* CConnectPoint::GetConnectObj() const
{
	return mpConnectObj;
}

// 接続部がついているオブジェクトを設定
void CConnectPoint::SetConnectObj(CConnectObject* obj)
{
	mpConnectObj = obj;
}