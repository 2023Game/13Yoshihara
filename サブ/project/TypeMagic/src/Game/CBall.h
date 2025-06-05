#pragma once
#include "CSpellBase.h"

// ボール呪文
// 呪文ベースクラスを継承
class CBall : public CSpellBase
{
public:
	// コンストラクタ
	CBall(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target, int generateNum = 0);
	// デストラクタ
	~CBall();

	// 更新処理
	void Update() override;

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	// コライダーを生成
	void CreateCol() override;

	// 発射中の更新
	void UpdateShooting() override;

	// 生成番号
	int mGenerateNum;
};