#include "CDeliveryFieldItem.h"
#include "CDeliveryField.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemy.h"
#include "CColliderSphere.h"
#include "CSound.h"

// �{�̃R���C�_�\�̔��a
#define BODY_RADIUS 5.0f
// �R���C�_�\�̃I�t�Z�b�g���W
#define BODY_OFFSET_POS CVector(0.0f, BODY_RADIUS / 2, 0.0f)

// �X�P�[���̍ŏ�
#define SCALE_MIN 3.0f
// �X�P�[���̍ő�
#define SCALE_MAX 4.0f
// �X�P�[���ω����x
#define SCALE_CHANGE_SPEED CVector(1.0f,1.0f,1.0f)

// ��]���x
#define ROT_SPEED CVector(0.0f, -90.0f, 0.0f)

// �E���ē������
#define NUM 1

#define SE_VOLUME 0.5f

// �R���X�g���N�^
CDeliveryFieldItem::CDeliveryFieldItem()
	: CObjectBase(ETag::eItem, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mIsBig(false)
	, mRoadType(ERoadType::eLeft1)
{
	mpGetSE = CResourceManager::Get<CSound>("GetSE");
	// �X�P�[����ݒ�
	Scale(Scale() * SCALE_MIN);
	// ���f���擾
	mpModel = CResourceManager::Get<CModel>("DeliveryItem");
	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CDeliveryFieldItem::~CDeliveryFieldItem()
{
	SAFE_DELETE(mpBodyCol);
}

// �X�V
void CDeliveryFieldItem::Update()
{
	// �����ړ��̑��x���A��O�ֈړ�
	float moveSpeed = FORCE_MOVE_SPEED * Times::DeltaTime();
	Position(Position() + CVector::forward * moveSpeed);
	// ��]����
	Rotation(EulerAngles() + ROT_SPEED * Times::DeltaTime());

	// �傫���ꍇ
	if (mIsBig)
	{
		// ���������Ă���
		CVector scale = Scale() - SCALE_CHANGE_SPEED * Times::DeltaTime();
		Scale(scale);
		// �ŏ���菬�����Ȃ��
		if (scale.X() < SCALE_MIN)
		{
			// ������
			mIsBig = false;
		}
	}
	// �������ꍇ
	else if (!mIsBig)
	{
		// �傫�����Ă���
		CVector scale = Scale() + SCALE_CHANGE_SPEED * Times::DeltaTime();
		Scale(scale);
		// �ő���傫���Ȃ��
		if (scale.X() > SCALE_MAX)
		{
			// �傫��
			mIsBig = true;
		}
	}
}

// �`��
void CDeliveryFieldItem::Render()
{
	mpModel->Render(Matrix());
}

// �Փˏ���
void CDeliveryFieldItem::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �{�̃R���C�_�\
	if (self == mpBodyCol)
	{
		// �v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			// �v���C���[�擾
			CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(other->Owner());
			// HP�𑝂₷
			player->SetHp(NUM);
			// �l�������Đ�
			mpGetSE->Play(SE_VOLUME, true);
			// ����
			SetEnable(false);
			SetShow(false);
		}
		// �G�̏ꍇ
		else if (other->Layer() == ELayer::eEnemy)
		{
			// �v���C���[�擾
			CDeliveryEnemy* enemy = dynamic_cast<CDeliveryEnemy*>(other->Owner());
			// ����ł����珈�����Ȃ�
			if (enemy->IsDeath()) return;
			// HP�𑝂₷
			enemy->SetHp(NUM);
			// �l�������Đ�
			mpGetSE->Play(SE_VOLUME, true);
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
		// ��Q���̏ꍇ
		else if (other->Layer() == ELayer::eObstruction)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);

		}
	}
}

// �ǂ̓��ɂ��邩���擾����
ERoadType CDeliveryFieldItem::GetRoadType() const
{
	return mRoadType;
}

// �ǂ̓��ɂ��邩�ݒ肷��
void CDeliveryFieldItem::SetRoadType(ERoadType roadType)
{
	mRoadType = roadType;
}

// �R���C�_�[�𐶐�
void CDeliveryFieldItem::CreateCol()
{
	mpBodyCol = new CColliderSphere
	(
		this,ELayer::eItem,
		BODY_RADIUS
	);
	// �v���C���[�A�G�A���˕��A��Q���A�T�m
	// �ƏՓ˔���
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,
		ETag::eBullet,ETag::eObstruction });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eAttackCol,ELayer::eObstruction,ELayer::eSearch });
	// ���W��ݒ�
	mpBodyCol->Position(BODY_OFFSET_POS);
}
