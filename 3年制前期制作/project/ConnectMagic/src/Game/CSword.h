#pragma once
#include "CWeapon.h"
#include "ObjectTag.h"
#include "CollisionLayer.h"

class CModel;

// 剣のクラス
class CSword : public CWeapon
{
public:
	// コンストラクタ
	CSword(CObjectBase* owner, ETag tag,
		Tags hitTags, Layers hitLayers);
	// デストラクタ
	~CSword();

	// 描画
	void Render() override;

private:
	CModel* mpModel;	// 剣のモデル
};
