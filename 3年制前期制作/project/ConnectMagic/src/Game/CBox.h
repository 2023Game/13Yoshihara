#pragma once
#include "CConnectObject.h"

class CBox : public CConnectObject
{
public:
	// コンストラクタ
	CBox(float scaleRatio = 1.0f);
	// デストラクタ
	~CBox();

private:
	// コライダーを生成
	void CreateCol() override;

	// キャラと衝突判定をするコライダー
	CCollider* mpCharaCol;
};