#pragma once
#include "CSpellBase.h"

// シールド呪文
// 呪文ベースクラスを継承
class CShield : public CSpellBase
{
public:
	// コンストラクタ
	CShield(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target);
	// デストラクタ
	~CShield();

	// 更新
	void Update() override;

private:
	// コライダーを生成
	void CreateCol() override;

	// 発射中の更新
	void UpdateShooting() override;

	// 現在の角度
	float mCurrentAngle;
};