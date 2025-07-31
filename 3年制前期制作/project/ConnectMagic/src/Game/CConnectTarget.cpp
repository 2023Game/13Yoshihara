#include "CConnectTarget.h"
#include "CConnectObject.h"

// ターゲット部分のデバッグ表示
#if _DEBUG
#include "Primitive.h"
#endif

// コンストラクタ
CConnectTarget::CConnectTarget(CConnectObject* connectObj)
	: CObjectBase(ETag::eConnectTarget, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mpConnectObj(connectObj)
{
}

// デストラクタ
CConnectTarget::~CConnectTarget()
{
	if (mpConnectObj != nullptr)
	{
		mpConnectObj->DeleteObject(this);
		mpConnectObj = nullptr;
	}
}

// 描画
void CConnectTarget::Render()
{
	// ターゲット部分のデバッグ表示
#if _DEBUG
	//Primitive::DrawWireBox
	//(
	//	Position(),
	//	Scale(),
	//	CColor::red
	//);
#endif
}

// 接続部がついているオブジェクトを取得
CConnectObject* CConnectTarget::GetConnectObj() const
{
	return mpConnectObj;
}

// 接続部がついているオブジェクトを設定
void CConnectTarget::SetConnectObj(CConnectObject* obj)
{
	mpConnectObj = obj;
}
