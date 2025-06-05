#include "CBolt.h"

// コンストラクタ
CBolt::CBolt(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eBolt, owner, target)
{
}

// デストラクタ
CBolt::~CBolt()
{
}

// コライダーを生成
void CBolt::CreateCol()
{
}

// 発射中の更新
void CBolt::UpdateShooting()
{
}
