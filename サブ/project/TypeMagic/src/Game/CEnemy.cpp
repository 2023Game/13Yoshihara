#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CBall.h"
#include "CPlayer.h"

// �̂̔��a
#define BODY_RADIUS 4.0f

// �R���X�g���N�^
CEnemy::CEnemy()
	: CEnemyBase(0.0f, 0.0f, {}, 0.0f)
	, CSpellCaster(this)
	, mElapsedTime(0.0f)
{
	// �d�͖���
	mIsGravity = false;
	// �ړ������������Ȃ�
	mIsMoveDir = false;

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
	if (mElapsedTime > 2.0f)
	{
		//CastStart(ESpellElementalType::eFire, ESpellShapeType::eBall);
		mElapsedTime = 0.0f;
	}

#if _DEBUG
	CDebugPrint::Print("EnemyHit:%d\n", mHitCount);
#endif
}

// �R���C�_�[�𐶐�
void CEnemy::CreateCol()
{
	// �{�̃R���C�_
	mpBodyCol = new CColliderSphere
	(
		this, ELayer::eEnemy,
		BODY_RADIUS
	);
	// �t�B�[���h,�ǁA�I�u�W�F�N�g�ƃv���C���[�ƍU���Ƃ����Փ�
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject,ELayer::ePlayer,
		ELayer::eAttackCol});
}
