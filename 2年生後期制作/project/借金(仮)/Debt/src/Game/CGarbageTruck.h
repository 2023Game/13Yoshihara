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

	// 回収処理
	void UpdateCollect();

};