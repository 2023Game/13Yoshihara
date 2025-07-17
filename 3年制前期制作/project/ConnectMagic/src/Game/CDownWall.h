#pragma once
#include "CSwitchObject.h"

// 下がっていく壁
class CDownWall : public CSwitchObject
{
public:
	// コンストラクタ
	CDownWall(CVector defaultPos);
	// デストラクタ
	~CDownWall();

private:
	// コライダーを生成
	void CreateCol() override;

	// 作用していない時の処理
	void UpdateOff() override;
	// 作用している時の処理
	void UpdateOn() override;

	// デフォルトのY座標
	float mDefaultPosY;
};