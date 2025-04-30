#pragma once
#include "CObjectBase.h"

// 接続できるターゲット部分
class CConnectTarget : public CObjectBase
{
public:
	// コンストラクタ
	CConnectTarget();
	// デストラクタ
	~CConnectTarget();

	// 描画
	void Render() override;
};