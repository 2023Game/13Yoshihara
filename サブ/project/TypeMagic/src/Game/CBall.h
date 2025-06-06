#pragma once
#include "CSpellBase.h"

// ボール呪文
// 呪文ベースクラスを継承
class CBall : public CSpellBase
{
public:
	// コンストラクタ
	CBall(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target);
	// デストラクタ
	~CBall();

	// 更新処理
	void Update() override;

private:
	// コライダーを生成
	void CreateCol() override;

	// 発射中の更新
	void UpdateShooting() override;
};