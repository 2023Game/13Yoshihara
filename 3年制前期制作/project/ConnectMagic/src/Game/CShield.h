#pragma once
#include "CObjectBase.h"

// オブジェクトが次の部屋に行けないようにするシールド
class CShield : public CObjectBase
{
public:
	// コンストラクタ
	CShield();
	// デストラクタ
	~CShield();

	// 描画
	void Render() override;

private:
	// コライダーを生成
	void CreateCol() override;
};