#include "CBall.h"
#include "CColliderSphere.h"

// �R���C�_�[�̔��a
#define RADIUS 1.0f

// �_������
#define HEIGHT 7.5f

// ����
#define ACCELERATE 1.0f

// �ǐՎ���
#define CHASE_TIME 2.0f

// �R���X�g���N�^
CBall::CBall(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target, int generateNum)
	: CSpellBase(elemental, ESpellShapeType::eBall, owner, target)
	, mGenerateNum(generateNum)
{
	mpModel = CResourceManager::Get<CModel>("FireBall");
	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CBall::~CBall()
{
}

// �X�V����
void CBall::Update()
{
	// ���˒��ȊO�Ȃ玝����ɂ��Ă���
	if (mState != EState::eShooting)
	{
		SetParent(mpOwner);
	}
	// ���˒��Ȃ���Ă����Ȃ�
	else
	{
		SetParent(nullptr);
	}
	// ���N���X�̍X�V����
	CSpellBase::Update();
}

// �Փˏ���
void CBall::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpSpellCol)
	{
		// ���肪�n�`�Ȃ�
		if (other->Layer() == ELayer::eGround ||
			other->Layer() == ELayer::eWall ||
			other->Layer() == ELayer::eObject)
		{
			// �폜
			Kill();
		}
		// ���肪�v���C���[�Ȃ�
		else if (other->Layer() == ELayer::ePlayer)
		{
			// �폜
			Kill();
		}
		// ���肪�G�Ȃ�
		else if (other->Layer() == ELayer::eEnemy)
		{
			// �폜
			Kill();
		}
	}
}

// �R���C�_�[�𐶐�
void CBall::CreateCol()
{
	mpSpellCol = new CColliderSphere(
		this, ELayer::eAttackCol,
		RADIUS
	);
	// �n�`�ƃv���C���[�ƓG�Ƃ����Փ˔���
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy });
}

// ���˒��̍X�V
void CBall::UpdateShooting()
{
	mElapsedTime += Times::DeltaTime();
	// �ǐՎ��Ԃ��I��������
	if (mElapsedTime > CHASE_TIME)
	{
		// �폜
		Kill();
	}

	mMoveSpeed = CVector::zero;
	// ����̈ʒu
	CVector opponentPos = mpTarget->Position();
	opponentPos.Y(opponentPos.Y() + HEIGHT);
	// ����ւ̕���
	CVector dir = opponentPos -Position();
	dir.Normalize();
	// �ړ����x
	float speed = GetSpellStatus().speed;
	// �ړ����x��ݒ�
	mMoveSpeed = dir * GetSpellStatus().speed * Times::DeltaTime();

	// ����
	SetSpeed(speed + ACCELERATE);
}
