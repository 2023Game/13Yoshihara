#pragma once
#include "CFieldBase.h"

// チュートリアルマップ
class CTutorialMap : public CFieldBase
{
public:
	// コンストラクタ
	CTutorialMap();
	// デストラクタ
	~CTutorialMap();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
};