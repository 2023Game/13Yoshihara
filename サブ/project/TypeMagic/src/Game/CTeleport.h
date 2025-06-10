#pragma once
#include "CSpellBase.h"

// テレポート呪文
// 呪文ベースクラスを継承
class CTeleport : public CSpellBase
{
public:
	// コンストラクタ
	CTeleport(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target);
	// デストラクタ
	~CTeleport();

private:
	// コライダーを生成
	void CreateCol() override;

	// 待機中の更新
	void UpdateIdle() override;
	// 発射中の更新
	void UpdateShooting() override;
};