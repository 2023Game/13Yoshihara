#pragma once
#include "CFieldBase.h"

class CTrashField : public CFieldBase
{
public:
	// コンストラクタ
	CTrashField();
	// デストラクタ
	~CTrashField();

	// 更新
	void Update();
	// 描画
	void Render();

private:
	// フィールドのオブジェクト生成
	void CreateFieldObjects() override;
};