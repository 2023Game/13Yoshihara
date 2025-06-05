#include "CSpellStatus.h"
#include "SpellDefine.h"

// �R���X�g���N�^
CSpellStatus::CSpellStatus(ESpellElementalType elemental, ESpellShapeType shape)
{
	// �����̃X�e�[�^�X��ݒ�
	SetStatus(elemental, shape);
}

// �f�X�g���N�^
CSpellStatus::~CSpellStatus()
{
}

// �����̃X�e�[�^�X���擾
SpellStatus CSpellStatus::GetSpellStatus() const
{
	return mSpellStatus;
}

// ���x��ݒ�
void CSpellStatus::SetSpeed(float speed)
{
	mSpellStatus.speed = speed;
}

// �X�e�[�^�X�ݒ�
void CSpellStatus::SetStatus(ESpellElementalType elemental, ESpellShapeType shape)
{
	int power = 0;				// �U����
	float speed = 0.0f;			// ���x
	int hp = 0;					// �ϋv

	// �������ƂŐݒ�
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

	// �`���ƂŐݒ�
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

	// �X�e�[�^�X��ݒ�
	mSpellStatus = { elemental,shape,power,speed,hp };
}