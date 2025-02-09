#pragma once
#include "CObjectBase.h"

class CModel;

class CDeliveryHome : public CObjectBase
{
public:
	// コンストラクタ
	CDeliveryHome();
	// デストラクタ
	~CDeliveryHome();

	// 更新
	void Update();
	// 描画
	void Render();
private:
	// 3dモデル
	CModel* mpModel;
	// 壁のコライダ―
	CCollider* mpWallCol;
	// ゴール判定のコライダー
	CCollider* mpGoalCol;
};