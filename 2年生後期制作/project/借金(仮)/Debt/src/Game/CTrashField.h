#pragma once
#include "CFieldBase.h"

class CVehicleBase;

class CTrashField : public CFieldBase
{
public:
	// コンストラクタ
	CTrashField();
	// デストラクタ
	~CTrashField();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// フィールドのオブジェクト生成
	void CreateFieldObjects() override;
	// 経路探索用のノードを生成
	void CreateNavNodes() override;
};