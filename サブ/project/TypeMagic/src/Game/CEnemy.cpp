#include "CEnemy.h"
#include "CColliderCapsule.h"
#include "CBall.h"
#include "CPlayer.h"

// �̂̔��a�ƍ���
#define BODY_RADIUS 2.5f
#define BODY_HEIGHT 15.0f

// �R���X�g���N�^
CEnemy::CEnemy()
	: CEnemyBase(0.0f, 0.0f, {}, 0.0f)
	, CSpellCaster(this)
	, mElapsedTime(0.0f)
{
	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CEnemy::~CEnemy()
{
}

// �X�V
void CEnemy::Update()
{
	// �v���C���[�̕���������������
	CVector lookPos = CPlayer::Instance()->Position();
	lookPos.Y(Position().Y());
	LookAt(lookPos);

	// �G�̊��N���X�̍X�V
	CEnemyBase::Update();
	// �����r���N���X�̍X�V
	CSpellCaster::Update();

	mElapsedTime += Times::DeltaTime();
	if (mElapsedTime > 5.0f)
	{
		CastStart(ESpellElementalType::eFire, ESpellShapeType::eBall);
		mElapsedTime = 0.0f;
	}
}

// �R���C�_�[�𐶐�
void CEnemy::CreateCol()
{
	// �{�̃R���C�_
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS / Scale().Y(), 0.0f),
		CVector(0.0f, BODY_HEIGHT / Scale().Y(), 0.0f),
		BODY_RADIUS
	);
	// �t�B�[���h,�ǁA�I�u�W�F�N�g�ƃv���C���[�ƍU���Ƃ����Փ�
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject,ELayer::ePlayer,
		ELayer::eAttackCol});
}
