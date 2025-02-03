#pragma once
#include "CGarbageTruck.h"

class CPunisherTruck : public CGarbageTruck
{
public:
	// コンストラクタ
	CPunisherTruck(CModel* model, const CVector& pos, const CVector& rotation,
		ERoadType road, std::vector<CNavNode*> patrolPoints);
	// デストラクタ
	~CPunisherTruck();

	// 変数をリセット
	void Reset() override;

	// 更新
	void Update();
	// 描画
	void Render();

private:
	// 移動処理
	void UpdateMove() override;
	// 回収処理
	void UpdateCollect() override;
};