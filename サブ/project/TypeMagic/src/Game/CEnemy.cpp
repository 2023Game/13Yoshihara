#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CBall.h"
#include "CPlayer.h"

// �̂̔��a
#define BODY_RADIUS 4.0f

// �r�������̃I�t�Z�b�g���W
#define SPELL_TEXT_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.25f, WINDOW_HEIGHT * 0.2f, 0.0f)

// �r�������̃T�C�Y
#define SPELL_TEXT_SIZE 32

// �R���X�g���N�^
CEnemy::CEnemy()
	: CEnemyBase(0.0f, 0.0f, {}, 0.0f)
	, CCastSpellStr(this, ECastType::eBasic, {}, SPELL_TEXT_UI_OFFSET_POS, SPELL_TEXT_SIZE)
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
	// �r�������w��N���X�̍X�V
	CCastSpellStr::Update();

	mElapsedTime += Times::DeltaTime();
	if (mElapsedTime > 2.0f)
	{
		CastStart(ESpellElementalType::eThunder, ESpellShapeType::eBreath);
		mElapsedTime = 0.0f;
	}

#if _DEBUG
	CDebugPrint::Print("EnemyHit:%d\n", mHitCount);
#endif
}

// ���C��������ݒ�
void CEnemy::SetMainElemental(ESpellElementalType elemental)
{
	mMainElement = elemental;
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

// �ҋ@���
void CEnemy::UpdateIdle()
{
}

// �ړ�����
void CEnemy::UpdateMove()
{
}

// �r����
void CEnemy::UpdateCast()
{
}

// �ǂ�������
void CEnemy::UpdateChase()
{
}

// ���
void CEnemy::UpdateDodge()
{
}

// ������
void CEnemy::UpdateRun()
{
}

// ��Ԑ؂�ւ�
void CEnemy::ChangeState(EState state)
{
}
