#pragma once
#include "CFire.h"
#include "CItemDrop.h"

class CCampFire : public CFire , public CItemDrop
{
public:
	// コンストラクタ
	CCampFire();
	// デストラクタ
	~CCampFire();

private:
	// コライダーを生成
	void CreateCol() override;
	// 燃えたときの処理
	void Burning() override;
};