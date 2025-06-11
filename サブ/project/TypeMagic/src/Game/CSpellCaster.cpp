#include "CSpellCaster.h"
#include "SpellDefine.h"
#include "CBall.h"
#include "CBolt.h"
#include "CBreath.h"
#include "CTeleport.h"
#include "CShield.h"
#include "CReflector.h"

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
	, mGenerateInterval(0.0f)
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
		if (WaitGenerate())
		{
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
		if (mGenerateNum >= BALL_MAX_GENERATE_NUM)
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
		if (WaitShoot())
		{
			mStep++;
		}
		break;

		// ���Ԃɔ��ˏ�Ԃ�
	case 3:
		// �S�Ĕ��ˏ�Ԃɂ����̂�
		if (Shoot())
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
	switch (mStep)
	{
		// �����Ԋu�̑ҋ@
	case 0:
		if (WaitGenerate())
		{
			mStep++;
		}
		break;

		// ��������
	case 1:
	{
		// �������W
		CVector offsetPos = BOLT_OFFSET_POS;
		// ������̕����Őݒ�
		offsetPos =
			mpOwner->VectorX() * offsetPos.X() +
			mpOwner->VectorY() * offsetPos.Y() +
			mpOwner->VectorZ() * offsetPos.Z();

		// �{���g�����𐶐�
		CBolt* bolt = new CBolt(mSpellElemental
			, mpOwner, mpOpponent);
		// ���W��ݒ�
		bolt->Position(mpOwner->Position() + offsetPos);
		// ���X�g�ɒǉ�
		mSpells.push_back(bolt);
		// ���������v���X
		mGenerateNum++;

		// ����
		mStep++;

		break;
	}

		// ���ˑҋ@
	case 2:
		if (WaitShoot())
		{
			mStep++;
		}
		break;

		// ���Ԃɔ��ˏ�Ԃ�
	case 3:
		// �S�Ĕ��ˏ�Ԃɂ����̂�
		if (Shoot())
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

		// �r���I��
	case 4:
		mIsSpellCasting = false;
		break;
	}
}

// �u���X�����̉r��
void CSpellCaster::CastBreath()
{
	switch (mStep)
	{
		// �����Ԋu�̑ҋ@
	case 0:
		if (WaitGenerate())
		{
			mStep++;
		}
		break;

		// ��������
	case 1:
	{
		// �������W
		CVector offsetPos = BREATH_OFFSET_POS;
		// ������̕����Őݒ�
		offsetPos =
			mpOwner->VectorX() * offsetPos.X() +
			mpOwner->VectorY() * offsetPos.Y() +
			mpOwner->VectorZ() * offsetPos.Z();

		// �u���X�����𐶐�
		CBreath* breath = new CBreath(mSpellElemental
			, mpOwner, mpOpponent);
		// ���W��ݒ�
		breath->Position(mpOwner->Position() + offsetPos);
		// ���X�g�ɒǉ�
		mSpells.push_back(breath);
		// ���������v���X
		mGenerateNum++;

		// �ő吔����������
		if (mGenerateNum >= BREATH_MAX_GENERATE_NUM)
		{
			// ����
			mStep++;
		}
		// �܂��Ȃ�
		else
		{
			// �O��
			mStep--;
		}

		break;
	}

		// �r���I��
	case 2:
		mIsSpellCasting = false;
		break;
	}
}

// �e���|�[�g�����̉r��
void CSpellCaster::CastTeleport()
{
	switch (mStep)
	{
		// �����Ԋu�̑ҋ@
	case 0:
		if (WaitGenerate())
		{
			mStep++;
		}
		break;

		// ��������
	case 1:
	{
		// �e���|�[�g�����𐶐�
		CTeleport* teleport = new CTeleport(mSpellElemental
			, mpOwner, mpOwner);
		// ���W��ݒ�
		teleport->Position(mpOwner->Position());
		// ���X�g�ɒǉ�
		mSpells.push_back(teleport);
		// ����
		mStep++;
		break;
	}

		// �r���I��
	case 2:
		// �폜�t���O���o���Ă�����
		if (mSpells[0]->IsKill())
		{
			mIsSpellCasting = false;
		}
		break;
	}
}

// �V�[���h�����̉r��
void CSpellCaster::CastShield()
{
	switch (mStep)
	{
		// �����Ԋu�̑ҋ@
	case 0:
		if (WaitGenerate())
		{
			mStep++;
		}
		break;

		// ��������
	case 1:
	{
		// �V�[���h�����𐶐�
		CShield* shield = new CShield(mSpellElemental
			, mpOwner, mpOwner);
		// ���X�g�ɒǉ�
		mSpells.push_back(shield);
		// �������𑝉�
		mGenerateNum++;

		// �ő吔����������
		if (mGenerateNum >= SHIELD_MAX_GENERATE_NUM)
		{
			// ����
			mStep++;
		}
		// ���ĂȂ��Ȃ�
		else
		{
			// �O��
			mStep--;
		}
		break;
	}

	// �r���I��
	case 2:
		mIsSpellCasting = false;
		break;
	}
}

// ���t���N�^�[�����̉r��
void CSpellCaster::CastReflector()
{
	switch (mStep)
	{
		// �����Ԋu�̑ҋ@
	case 0:
		if (WaitGenerate())
		{
			mStep++;
		}
		break;

		// ��������
	case 1:
	{		
		// �������W
		CVector offsetPos = REFLECTOR_OFFSET_POS;
		// ������̕����Őݒ�
		offsetPos =
			mpOwner->VectorX() * offsetPos.X() +
			mpOwner->VectorY() * offsetPos.Y() +
			mpOwner->VectorZ() * offsetPos.Z();

		// ���t���N�^�[�����𐶐�
		CReflector* reflector = new CReflector(mSpellElemental
			, mpOwner, mpOwner);
		// ���W�ݒ�
		reflector->Position(mpOwner->Position() + offsetPos);
		// ���X�g�ɒǉ�
		mSpells.push_back(reflector);
		// �������𑝉�
		mGenerateNum++;

		// ����
		mStep++;

		break;
	}

	// �r���I��
	case 2:
		mIsSpellCasting = false;
		break;
	}
}

// �������x�Ƒҋ@���Ԃ̐ݒ�
void CSpellCaster::SetTime(ESpellElementalType elemental, ESpellShapeType shape)
{
	float generateInterval = 0.0f;	// �����Ԋu
	float idleTime = 0.0f;		// �ҋ@����

	switch (elemental)
	{
	case ESpellElementalType::eFire:
		generateInterval += FIRE_GENERATE_INTERVAL;
		idleTime += FIRE_IDLE_TIME;
		break;
	case ESpellElementalType::eWind:
		generateInterval += WIND_GENERATE_INTERVAL;
		idleTime += WIND_IDLE_TIME;
		break;
	case ESpellElementalType::eEarth:
		generateInterval += EARTH_GENERATE_INTERVAL;
		idleTime += EARTH_IDLE_TIME;
		break;
	case ESpellElementalType::eThunder:
		generateInterval += THUNDER_GENERATE_INTERVAL;
		idleTime += THUNDER_IDLE_TIME;
		break;
	case ESpellElementalType::eWater:
		generateInterval += WATER_GENERATE_INTERVAL;
		idleTime += WATER_IDLE_TIME;
		break;
	case ESpellElementalType::eNeutral:
		generateInterval += NEUTRAL_GENERATE_INTERVAL;
		idleTime += NEUTRAL_IDLE_TIME;
		break;
	}

	switch (shape)
	{
	case ESpellShapeType::eBall:
		generateInterval += BALL_GENERATE_INTERVAL;
		idleTime += BALL_IDLE_TIME;
		break;
	case ESpellShapeType::eBolt:
		generateInterval += BOLT_GENERATE_INTERVAL;
		idleTime += BOLT_IDLE_TIME;
		break;
		// �u���X�̏ꍇ�u���X�̐����Ԋu��ݒ�
	case ESpellShapeType::eBreath:
		generateInterval = BREATH_GENERATE_INTERVAL;
		idleTime += BREATH_IDLE_TIME;
		break;
		// �e���|�[�g�̏ꍇ�e���|�[�g�̑ҋ@���Ԃ�ݒ�
	case ESpellShapeType::eTeleport:
		generateInterval += TELEPORT_GENERATE_INTERVAL;
		idleTime = TELEPORT_IDLE_TIME;
		break;
		// �V�[���h�̏ꍇ�V�[���h�̐����Ԋu��ݒ�
	case ESpellShapeType::eShield:
		generateInterval = SHIELD_GENERATE_INTERVAL;
		idleTime += SHIELD_IDLE_TIME;
		break;
	case ESpellShapeType::eReflector:
		generateInterval += REFLECTOR_GENERATE_INTERVAL;
		idleTime += REFLECTOR_IDLE_TIME;
		break;
	}

	mGenerateInterval = generateInterval;
	mIdleTime = idleTime;
}

// �����ҋ@
bool CSpellCaster::WaitGenerate()
{		
	// ���Ԍo��
	mElapsedTime += Times::DeltaTime();
	// �����Ԋu���߂�����
	if (mElapsedTime > mGenerateInterval)
	{
		mElapsedTime = 0.0f;
		// �ҋ@�I��
		return true;
	}

	// �ҋ@��
	return false;
}

// ���ˑҋ@
bool CSpellCaster::WaitShoot()
{		
	// ���Ԍo��
	mElapsedTime += Times::DeltaTime();
	// �ҋ@���Ԃ𒴂�����
	if (mElapsedTime > mIdleTime)
	{
		mElapsedTime = 0.0f;
		// �ҋ@�I��
		return true;
	}

	// �ҋ@��
	return false;
}

// ����
bool CSpellCaster::Shoot()
{
	// ���ˏ�Ԃ�
	mSpells[mGenerateNum - 1]->ChangeState(CSpellBase::EState::eShooting);

	mGenerateNum--;
	// �S�Ĕ��ˏ�Ԃɂ���
	if (mGenerateNum <= 0)
	{
		// ���ˏI��
		return true;
	}

	// �܂��c���Ă���
	return false;
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

// �ΐ푊����擾
CObjectBase* CSpellCaster::GetOpponent() const
{
	return mpOpponent;
}
