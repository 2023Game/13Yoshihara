#pragma once
#include "CTextUI3D.h"

// ダメージ表記のUI
class CDamageUI3D : public CTextUI3D
{
public:
	// コンストラクタ
	CDamageUI3D(CVector pos);
	// デストラクタ
	~CDamageUI3D();

	// 更新
	void Update() override;

private:
	// 座標をぶらして返す
	CVector RandPos(const CVector& pos);

	// 自動で消えるまでの時間
	float mAutoDeleteTime;
};