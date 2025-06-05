#pragma once
#include "CSpellBase.h"
#include "CSpellStatus.h"

// ブレス呪文
// 呪文ベースクラスを継承
class CBreath : public CSpellBase
{
public:
	// コンストラクタ
	CBreath(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target);
	// デストラクタ
	~CBreath();

private:
	// コライダーを生成
	void CreateCol() override;

	// 発射中の更新
	void UpdateShooting() override;
};