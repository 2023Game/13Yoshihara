#include "CSpellCaster.h"
#include "SpellDefine.h"
#include "CBall.h"
#include "CBolt.h"
#include "CBreath.h"

// �R���X�g���N�^
CSpellCaster::CSpellCaster(CObjectBase* owner)
	: mpOwner(owner)
	, mpOpponent(nullptr)
	, mGenerateNum(0)
	, mSpellElemental(ESpellElementalType::eNeutral)
	, mSpellShape(ESpellShapeType::eBall)
	, mElapsedTime(0.0f)
	, mStep(0)
	, mIsSpellCasting(false)
	, mGenerateSpeed(0.0f)
	, mIdleTime(0.0f)
{
}

// �f�X�g���N�^
CSpellCaster::~CSpellCaster()
{
}

// �X�V
void CSpellCaster::Update()
{
	// �r�����łȂ��Ȃ珈�����Ȃ�
	if (!mIsSpellCasting) return;

	// �`
	switch (mSpellShape)
	{
	case ESpellShapeType::eBall:		CastBall();			break;
	case ESpellShapeType::eBolt:		CastBolt();			break;
	case ESpellShapeType::eBreath:		CastBreath();		break;
	case ESpellShapeType::eTeleport:	CastTeleport();		break;
	case ESpellShapeType::eShield:		CastShield();		break;
	case ESpellShapeType::eReflector:	CastReflector();	break;
	}
}

// �{�[�������̉r��
void CSpellCaster::CastBall()
{
	switch (mStep)
	{
		// �����Ԋu�̑ҋ@
	case 0:
		// ���Ԍo��
		mElapsedTime += Times::DeltaTime();
		// �������x�̐��l�𒴂�����
		if (mElapsedTime > mGenerateSpeed)
		{
			mElapsedTime = 0.0f;
			// ����
			mStep++;
		}
		break;

		// ��������
	case 1:
	{
		// �������W�̃��X�g
		CVector generatePosList[] = {
			BALL_OFFSET_POS_1,
			BALL_OFFSET_POS_2,
			BALL_OFFSET_POS_3,
			BALL_OFFSET_POS_4,
			BALL_OFFSET_POS_5,
		};
		// �������W
		CVector offsetPos = generatePosList[mGenerateNum];
		// ������̕����Őݒ�
		offsetPos =
			mpOwner->VectorX() * offsetPos.X() +
			mpOwner->VectorY() * offsetPos.Y() +
			mpOwner->VectorZ() * offsetPos.Z();

		// �{�[�������𐶐�
		CBall* ball = new CBall(mSpellElemental
			, mpOwner, mpOpponent);
		// ���W��ݒ�
		ball->Position(mpOwner->Position() + offsetPos);
		// ���X�g�ɒǉ�
		mSpells.push_back(ball);
		// ���������v���X
		mGenerateNum++;

		// �ő吔����������
		if (mGenerateNum == BALL_MAX_GENERATE_NUM)
		{
			// ����
			mStep++;
		}
		// ���ĂȂ��̂�
		else
		{
			// �߂�
			mStep--;
		}
		break;
	}
		// ���ˑҋ@
	case 2:		
		// ���Ԍo��
		mElapsedTime += Times::DeltaTime();
		// �ҋ@���Ԃ𒴂�����
		if (mElapsedTime > mIdleTime)
		{
			mElapsedTime = 0.0f;
			// ����
			mStep++;
		}
		break;

		// ���Ԃɔ��ˏ�Ԃ�
	case 3:
		// ���ˏ�Ԃ�
		mSpells[mGenerateNum - 1]->ChangeState(CSpellBase::EState::eShooting);

		mGenerateNum--;
		// �S�Ĕ��ˏ�Ԃɂ����̂�
		if (mGenerateNum <= 0)
		{
			// ����
			mStep++;
		}
		// ���ĂȂ��̂�
		else
		{
			// �߂�
			mStep--;
		}

		break;

	case 4:
		// �r���I��
		mIsSpellCasting = false;
		break;
	}
	
}

// �{���g�����̉r��
void CSpellCaster::CastBolt()
{
}

// �u���X�����̉r��
void CSpellCaster::CastBreath()
{
}

// �e���|�[�g�����̉r��
void CSpellCaster::CastTeleport()
{
}

// �e���|�[�g�����̉r��
void CSpellCaster::CastShield()
{
}

// ���t���N�^�[�����̉r��
void CSpellCaster::CastReflector()
{
}

// �������x�Ƒҋ@���Ԃ̐ݒ�
void CSpellCaster::SetTime(ESpellElementalType elemental, ESpellShapeType shape)
{
	float generateSpeed = 0.0f;	// �������x
	float idleTime = 0.0f;		// �ҋ@����

	switch (elemental)
	{
	case ESpellElementalType::eFire:
		generateSpeed += FIRE_GENERATE_SPEED;
		idleTime += FIRE_IDLE_TIME;
		break;
	case ESpellElementalType::eWind:
		generateSpeed += WIND_GENERATE_SPEED;
		idleTime += WIND_IDLE_TIME;
		break;
	case ESpellElementalType::eEarth:
		generateSpeed += EARTH_GENERATE_SPEED;
		idleTime += EARTH_IDLE_TIME;
		break;
	case ESpellElementalType::eThunder:
		generateSpeed += THUNDER_GENERATE_SPEED;
		idleTime += THUNDER_IDLE_TIME;
		break;
	case ESpellElementalType::eWater:
		generateSpeed += WATER_GENERATE_SPEED;
		idleTime += WATER_IDLE_TIME;
		break;
	case ESpellElementalType::eNeutral:
		generateSpeed += NEUTRAL_GENERATE_SPEED;
		idleTime += NEUTRAL_IDLE_TIME;
		break;
	}

	switch (shape)
	{
	case ESpellShapeType::eBall:
		generateSpeed += BALL_GENERATE_SPEED;
		idleTime += BALL_IDLE_TIME;
		break;
	case ESpellShapeType::eBolt:
		generateSpeed += BOLT_GENERATE_SPEED;
		idleTime += BOLT_IDLE_TIME;
		break;
	case ESpellShapeType::eBreath:
		generateSpeed += BREATH_GENERATE_SPEED;
		idleTime += BREATH_IDLE_TIME;
		break;
	case ESpellShapeType::eTeleport:
		generateSpeed += TELEPORT_GENERATE_SPEED;
		idleTime += TELEPORT_IDLE_TIME;
		break;
	case ESpellShapeType::eShield:
		generateSpeed += SHIELD_GENERATE_SPEED;
		idleTime += SHIELD_IDLE_TIME;
		break;
	case ESpellShapeType::eReflector:
		generateSpeed += REFLECTOR_GENERATE_SPEED;
		idleTime += REFLECTOR_IDLE_TIME;
		break;
	}

	mGenerateSpeed = generateSpeed;
	mIdleTime = idleTime;
}

// �r���J�n
bool CSpellCaster::CastStart(ESpellElementalType elemental
	,ESpellShapeType shape)
{
	// �r�����Ȃ̂ŊJ�n���s
	if (mIsSpellCasting) return false;

	// �r���J�n
	mIsSpellCasting = true;
	mSpellElemental = elemental;
	mSpellShape = shape;
	mElapsedTime = 0.0f;
	mStep = 0;
	mGenerateNum = 0;
	mSpells.clear();

	// �������x�Ƒҋ@���Ԃ����߂Đݒ肷��
	SetTime(elemental, shape);
}

// �ΐ푊���ݒ�
void CSpellCaster::SetOpponent(CObjectBase* opponent)
{
	mpOpponent = opponent;
}
