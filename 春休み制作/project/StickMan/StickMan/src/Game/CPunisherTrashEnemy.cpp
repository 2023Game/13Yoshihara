#include "CPunisherTrashEnemy.h"
#include "CGaugeUI3D.h"
#include "CDebugFieldOfView.h"
#include "CColliderCapsule.h"
#include "Maths.h"

// �傫��
#define SCALE CVector(0.5f,0.5f,0.5f)

#define BODY_RADIUS 12.5f	// �{�̂̃R���C�_�\�̔��a
#define BODY_HEIGHT 25.0f	// �{�̂̃R���C�_�\�̍���
#define BODY_WIDTH 50.0f	// �{�̂̃R���C�_�\�̕�

// �U���R���C�_�\
#define ATTACK_COL_RADIUS	10.0f	// ���a
#define ATTACK_COL_HEIGHT	25.0f	// ����
#define ATTACK_COL_WIDTH	70.0f*2.5f	// ��
// �I�t�Z�b�g���W
#define ATTACK_COL_OFFSET_POS CVector(0.0f,-10.0f,80.0f)

// �N���e�B�J���U���R���C�_�[
#define CRITICAL_COL_RADIUS		15.0f		// ���a
#define CRITICAL_COL_HEIGHT		25.0f		// ����
#define CRITICAL_COL_WIDTH		140.0f*2.0f		// ��
// �I�t�Z�b�g���W
#define CRITICAL_COL_OFFSET_POS CVector(0.0f,-10.0f,140.0f)

#define ATTACK_RANGE 90.0f	// �U�����鋗��

// Hp�Q�[�W�̃I�t�Z�b�g���W
#define GAUGE_OFFSET_POS CVector(0.0f,37.5f,0.0f)

// �R���X�g���N�^
CPunisherTrashEnemy::CPunisherTrashEnemy()
	:CTrashEnemy(true, true)
{
	// �傫���̐ݒ�
	Scale(SCALE);
	// �S�~�܂̐��̏����l��ݒ�
	SetTrashBag(GetDefaultBagNum());
	SetGoldTrashBag(GetDefaultGoldBagNum());

	// �R���C�_�[���쐬
	CreateCol();
	
	// �ŏ��͖���
	mpHpGauge->SetEnable(false);
	mpHpGauge->SetShow(false);
#if _DEBUG
	mpDebugFov->SetEnable(false);
	mpDebugFov->SetShow(false);
#endif
}

// �f�X�g���N�^
CPunisherTrashEnemy::~CPunisherTrashEnemy()
{
}

// �R���C�_�[���쐬����
void CPunisherTrashEnemy::CreateCol()
{
	// �傫���̎擾
	float scale = SCALE.X();
	// �{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(BODY_WIDTH - BODY_RADIUS / scale, BODY_HEIGHT, 0.0f),
		CVector(-BODY_WIDTH + BODY_RADIUS / scale, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	// �n�`�A�v���C���[�A�G�A������A�U���A�ԗ��A�L�����̒T�m�p�A�S�~��
	// �ƏՓ˔��������
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy, ETag::eVehicle, ETag::eTrashBag });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,ELayer::eCharaSearch,
		ELayer::ePlayer, ELayer::eEnemy,ELayer::eCollector, ELayer::eAttackCol, ELayer::eVehicle, ELayer::eTrashBag });

	// �U���R���C�_�[
	mpAttackCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, ATTACK_COL_HEIGHT, ATTACK_COL_WIDTH - ATTACK_COL_RADIUS * 10),
		CVector(0.0f, ATTACK_COL_HEIGHT, -ATTACK_COL_WIDTH + ATTACK_COL_RADIUS * 10),
		ATTACK_COL_RADIUS
	);
	// �N���e�B�J���U���R���C�_�[
	mpCriticalCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, CRITICAL_COL_HEIGHT, CRITICAL_COL_WIDTH - CRITICAL_COL_RADIUS * 10),
		CVector(0.0f, CRITICAL_COL_HEIGHT, -CRITICAL_COL_WIDTH + CRITICAL_COL_RADIUS * 10),
		CRITICAL_COL_RADIUS
	);

	// �v���C���[�ƏՓ˔��肷��悤�ɐݒ�
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->SetCollisionLayers({ ELayer::ePlayer });
	mpCriticalCol->SetCollisionTags({ ETag::ePlayer });
	mpCriticalCol->SetCollisionLayers({ ELayer::ePlayer });

	// �����̑O�Ɉʒu����
	mpAttackCol->Position(ATTACK_COL_OFFSET_POS);
	mpCriticalCol->Position(CRITICAL_COL_OFFSET_POS);

	// �U���R���C�_�[�͍ŏ��̓I�t�ɂ��Ă���
	mpAttackCol->SetEnable(false);
	mpCriticalCol->SetEnable(false);
}

// �����Ԃ��瑼�̏�Ԃֈڍs����������`�F�b�N
// ���d�u���p�͂����ǂ�������
bool CPunisherTrashEnemy::ChangePatrolToOther()
{
	ChangeState(EState::eChase);
	return true;
}

// �ǐՏ�Ԃ��瑼�̏�Ԃֈڍs����������`�F�b�N
// ���d�u���p�͒ǂ�����������
bool CPunisherTrashEnemy::ChangeChaseToOther()
{
	// �v���C���[�ɍU���ł���Ȃ�΁A�U����Ԃֈڍs
	if (CanAttackPlayer(ATTACK_RANGE))
	{
		// 1����100�܂ł�100�̐����痐�����擾
		int random = Math::Rand(1, 100);
		// �N���e�B�J���m���ȉ��̒l�Ȃ�N���e�B�J���U��
		if (random <= GetCriticalChance())
		{
			ChangeState(EState::eCriticalStart);
		}
		// ����ȊO�̎��͒ʏ�U��
		else
		{
			ChangeState(EState::eAttackStart);
		}
		return true;
	}
	return false;
}

// Hp�Q�[�W�̍X�V
void CPunisherTrashEnemy::UpdateHpGauge()
{
	mpHpGauge->Position(Position() + GAUGE_OFFSET_POS);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());
}
