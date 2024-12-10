#pragma once
#include "CCharaBase.h"
#include "CGarbageTruckStatus.h"
#include "CCar.h"

class CModel;

// ゴミ収集車のクラス
// 車のクラスを継承
class CGarbageTruck : public CCar
{
public:
	CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation);
	~CGarbageTruck();

	void Update();
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	void Render();


private:
	// 停止処理
	void UpdateStop();
	// 回収処理
	void UpdateCollect();

	CGarbageTruckStatus mTruckStatus;	// ゴミ収集車のステータス追加分
};