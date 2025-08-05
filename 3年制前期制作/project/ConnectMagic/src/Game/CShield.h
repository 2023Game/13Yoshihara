#pragma once
#include "CObjectBase.h"

class CImage3D;

// 箱が次の部屋に行けないようにするシールド
class CShield : public CObjectBase
{
public:
	// コンストラクタ
	CShield();
	// デストラクタ
	~CShield();

	// 更新
	void Update() override;

private:
	// コライダーを生成
	void CreateCol() override;
	// コライダー
	CCollider* mpCol;

	// 画像を生成
	void CreateImg();
	// シールドの画像
	CImage3D* mpImg;
};