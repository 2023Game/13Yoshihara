#include "CConnectTarget.h"
#include "CConnectObject.h"

// ターゲット部分のデバッグ表示
#if _DEBUG
#include "Primitive.h"
#endif

// コンストラクタ
CConnectTarget::CConnectTarget(CConnectObject* connectObj)
	: mpConnectObj(connectObj)
{
}

// デストラクタ
CConnectTarget::~CConnectTarget()
{
}

// 描画
void CConnectTarget::Render()
{
	// ターゲット部分のデバッグ表示
#if _DEBUG
	Primitive::DrawWireBox
	(
		Position(),
		Scale(),
		CColor::red
	);
#endif
}

// 接続部がついているオブジェクトを取得
CConnectObject* CConnectTarget::GetConnectObj()
{
	return mpConnectObj;
}
