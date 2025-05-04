#include "CConnectTarget.h"

// ターゲット部分のデバッグ表示
#if _DEBUG
#include "Primitive.h"
#endif

// コンストラクタ
CConnectTarget::CConnectTarget()
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
