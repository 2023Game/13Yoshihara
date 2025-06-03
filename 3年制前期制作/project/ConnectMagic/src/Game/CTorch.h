#pragma once
#include "CFire.h"

// 松明クラス
class CTorch : public CFire
{
public:
	// コンストラクタ
	CTorch();
	// デストラクタ
	~CTorch();

private:
	// コライダーを生成
	void CreateCol() override;
};