#pragma once
#include "CCharaBase.h"
#include "CGarbageTruckStatus.h"
#include "CVehicleBase.h"
#include "CGarbageTruckStatus.h"

class CModel;

// ゴミ収集車のクラス
// 車のクラスを継承
class CGarbageTruck : public CVehicleBase , public CGarbageTruckStatus
{
public:
	// コンストラクタ
	CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road);
	// デストラクタ
	~CGarbageTruck();

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
	// 回収処理
	void UpdateCollect();

};