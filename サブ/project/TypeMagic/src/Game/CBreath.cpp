#include "CBreath.h"

// コンストラクタ
CBreath::CBreath(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eBreath, owner, target)
{
}

// デストラクタ
CBreath::~CBreath()
{
}

// コライダーを生成
void CBreath::CreateCol()
{
	
}

// 発射中の更新
void CBreath::UpdateShooting()
{
}
