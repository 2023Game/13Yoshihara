#pragma once
#include "SpellType.h"

// 呪文のステータス
struct SpellStatus
{
	ESpellElementalType elemental;	// 属性
	ESpellShapeType shape;			// 形
	int power;				// 攻撃力
	float speed;			// 速度
	int hp;					// 耐久
};

// 呪文のステータス
class CSpellStatus
{
public:
	// コンストラクタ
	CSpellStatus(ESpellElementalType elemental, ESpellShapeType shape);
	// デストラクタ
	~CSpellStatus();

	// 呪文のステータスを取得
	SpellStatus GetSpellStatus() const;

	// 速度を設定
	void SetSpeed(float speed);

protected:
	// 呪文のステータス
	SpellStatus mSpellStatus;

private:
	// ステータス設定
	void SetStatus(ESpellElementalType elemental, ESpellShapeType shape);
};