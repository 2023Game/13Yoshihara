#pragma once
#include "CVehicleBase.h"
#include "CVehicleStatus.h"

class CModel;

// 車のクラス
class CCar : public CVehicleBase, public CVehicleStatus
{
public:
	// コンストラクタ
	CCar(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road);
	// デストラクタ
	~CCar();

	// 更新
	void Update();
	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

private:
	// 移動処理
	void UpdateMove();
	// 停止処理
	void UpdateStop();
	// 壊れた処理
	void UpdateBroken();
};