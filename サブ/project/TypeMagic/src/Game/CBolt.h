#pragma once
#include "CSpellBase.h"

// ボルト呪文
// 呪文ベースクラスを継承
class CBolt : public CSpellBase
{
public:
	// コンストラクタ
	CBolt(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target);
	// デストラクタ
	~CBolt();

private:
	// コライダーを生成
	void CreateCol() override;

	// 発射中の更新
	void UpdateShooting() override;
};