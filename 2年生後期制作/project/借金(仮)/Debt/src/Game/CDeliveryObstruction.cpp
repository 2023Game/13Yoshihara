#include "CDeliveryObstruction.h"
#include "CDeliveryField.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemy.h"
#include "CSound.h"

// �X�P�[���{��
#define SCALE 2.0f

// �{�̃R���C�_�\
#define BODY_RADIUS 10.0f
// �R���C�_�[�̃I�t�Z�b�g���W
#define BODY_OFFSET_POS CVector(0.0f, BODY_RADIUS / 2, 0.0f)

// �_���[�W
#define DAMAGE 1

#define SE_VOLUME 0.5f

// �R���X�g���N�^
CDeliveryObstruction::CDeliveryObstruction()
	: CObjectBase(ETag::eObstruction, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mRoadType(ERoadType::eLeft1)
{
	mpHitSE = CResourceManager::Get<CSound>("DamageSE");
	// �X�P�[����ݒ�
	Scale(Scale() * 2.0f);
	// ���f���擾
	mpModel = CResourceManager::Get<CModel>("Obstruction");
	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CDeliveryObstruction::~CDeliveryObstruction()
{
	SAFE_DELETE(mpBodyCol);
}

// �X�V
void CDeliveryObstruction::Update()
{
	// �����ړ��̑��x���A��O�ֈړ�
	float moveSpeed = FORCE_MOVE_SPEED * Times::DeltaTime();
	Position(Position() + CVector::forward * moveSpeed);
}

// �`��
void CDeliveryObstruction::Render()
{
	mpModel->Render(Matrix());
}

// �Փˏ���
void CDeliveryObstruction::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �{�̃R���C�_�\
	if (self == mpBodyCol)
	{
		// �v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			// �v���C���[�擾
			CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(other->Owner());
			// �_���[�W��^����
			player->TakeDamage(DAMAGE, this);
			// �Փˉ����Đ�
			mpHitSE->Play(SE_VOLUME, true);
			// ����
			SetEnable(false);
			SetShow(false);
		}
		// �G�̏ꍇ
		else if (other->Layer() == ELayer::eEnemy)
		{
			// �G�擾
			CDeliveryEnemy* enemy = dynamic_cast<CDeliveryEnemy*>(other->Owner());
			// �_���[�W��^����
			enemy->TakeDamage(DAMAGE, this);
			// �Փˉ����Đ�
			mpHitSE->Play(SE_VOLUME, true);
			// ����
			SetEnable(false);
			SetShow(false);
		}
		// �U������̏ꍇ
		else if (other->Layer() == ELayer::eAttackCol)
		{
			// ����
			SetEnable(false);
			SetShow(false);
		}
	}
}

// �ǂ̓��ɂ��邩���擾����
ERoadType CDeliveryObstruction::GetRoadType() const
{
	return mRoadType;
}

// �ǂ̓��ɂ��邩�ݒ肷��
void CDeliveryObstruction::SetRoadType(ERoadType roadType)
{
	mRoadType = roadType;
}

// �R���C�_�\�𐶐�
void CDeliveryObstruction::CreateCol()
{
	// �{�̃R���C�_�\
	mpBodyCol = new CColliderSphere
	(
		this, ELayer::eObstruction,
		BODY_RADIUS,
		true
	);
	// �v���C���[�A�G�A���˕��A�A�C�e���A�T�m
	// �ƏՓ˔���
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,
		ETag::eBullet,ETag::eItem });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eAttackCol,ELayer::eItem,ELayer::eSearch });
	mpBodyCol->Position(BODY_OFFSET_POS);
}
