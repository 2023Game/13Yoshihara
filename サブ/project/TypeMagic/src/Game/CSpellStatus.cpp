#include "CSpellStatus.h"
#include "SpellDefine.h"

// コンストラクタ
CSpellStatus::CSpellStatus(ESpellElementalType elemental, ESpellShapeType shape)
{
	// 呪文のステータスを設定
	SetStatus(elemental, shape);
}

// デストラクタ
CSpellStatus::~CSpellStatus()
{
}

// 呪文のステータスを取得
SpellStatus CSpellStatus::GetSpellStatus() const
{
	return mSpellStatus;
}

// 速度を設定
void CSpellStatus::SetSpeed(float speed)
{
	mSpellStatus.speed = speed;
}

// ステータス設定
void CSpellStatus::SetStatus(ESpellElementalType elemental, ESpellShapeType shape)
{
	int power = 0;				// 攻撃力
	float speed = 0.0f;			// 速度
	int hp = 0;					// 耐久

	// 属性ごとで設定
	switch (elemental)
	{
	case ESpellElementalType::eFire:
		power += FIRE_POWER;
		speed += FIRE_SPEED;
		hp += FIRE_HP;
		break;
	case ESpellElementalType::eWind:
		power += WIND_POWER;
		speed += WIND_SPEED;
		hp += WIND_HP;
		break;
	case ESpellElementalType::eEarth:
		power += EARTH_POWER;
		speed += EARTH_SPEED;
		hp += EARTH_HP;
		break;
	case ESpellElementalType::eThunder:
		power += THUNDER_POWER;
		speed += THUNDER_SPEED;
		hp += THUNDER_HP;
		break;
	case ESpellElementalType::eWater:
		power += WATER_POWER;
		speed += WATER_SPEED;
		hp += WATER_HP;
		break;
	case ESpellElementalType::eNeutral:
		power += NEUTRAL_POWER;
		speed += NEUTRAL_SPEED;
		hp += NEUTRAL_HP;
		break;
	}

	// 形ごとで設定
	switch (shape)
	{
	case ESpellShapeType::eBall:
		power += BALL_POWER;
		speed += BALL_SPEED;
		hp += BALL_HP;
		break;
	case ESpellShapeType::eBolt:
		power += BOLT_POWER;
		speed += BOLT_SPEED;
		hp += BOLT_HP;
		break;
	case ESpellShapeType::eBreath:
		power += BREATH_POWER;
		speed += BREATH_SPEED;
		hp += BREATH_HP;
		break;
	case ESpellShapeType::eTeleport:
		power += TELEPORT_POWER;
		speed += TELEPORT_SPEED;
		hp += TELEPORT_HP;
		break;
	case ESpellShapeType::eShield:
		power += SHIELD_POWER;
		speed += SHIELD_SPEED;
		hp += SHIELD_HP;
		break;
	case ESpellShapeType::eReflector:
		power += REFLECTOR_POWER;
		speed += REFLECTOR_SPEED;
		hp += REFLECTOR_HP;
		break;
	}

	// ステータスを設定
	mSpellStatus = { elemental,shape,power,speed,hp };
}