#pragma once
#include "CVehicleBase.h"

class CModel;

// 車のクラス
class CCar : public CVehicleBase
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
};