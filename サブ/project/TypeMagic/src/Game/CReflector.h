#pragma once
#include "CSpellBase.h"

// リフレクター呪文
// 呪文ベースクラスを継承
class CReflector : public CSpellBase
{
public:
	// コンストラクタ
	CReflector(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target);
	// デストラクタ
	~CReflector();

	// 更新
	void Update() override;

private:
	// コライダーを生成
	void CreateCol() override;

	// 発射中の更新
	void UpdateShooting() override;
};