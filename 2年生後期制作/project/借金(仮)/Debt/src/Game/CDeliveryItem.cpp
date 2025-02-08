#include "CDeliveryItem.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CPlayerBase.h"

#include "CInput.h"

#define ITEM_HEIGHT	2.5f	// ����
#define ITEM_WIDTH	4.5f	// ��
#define ITEM_RADIUS	2.75f	// ���a

// ���ł܂ł̎���
#define DELETE_TIME 3.0f

// �R���X�g���N�^
CDeliveryItem::CDeliveryItem()
	: CObjectBase(ETag::eBullet, ETaskPriority::eWeapon, 
		0, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
{
	mpModel = CResourceManager::Get<CModel>("DeliveryItem");

	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CDeliveryItem::~CDeliveryItem()
{
	SAFE_DELETE(mpBodyCol);
}

// �X�V
void CDeliveryItem::Update()
{
	Position(Position() + mMoveSpeed);

	// ���ł܂ł̎��Ԃ��o�߂��Ă��Ȃ��Ȃ�
	if (mElapsedTime < DELETE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	// ���ł܂ł̎��Ԃ��o�߂�����
	else
	{
		// �폜
		Kill();
	}
}

// �`��
void CDeliveryItem::Render()
{
	mpModel->Render(Matrix());
}

// �ړ���ݒ�
void CDeliveryItem::SetMoveSpeed(CVector moveSpeed)
{
	mMoveSpeed = moveSpeed;
}

// �R���C�_�\�𐶐�
void CDeliveryItem::CreateCol()
{
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, ITEM_HEIGHT, ITEM_WIDTH - ITEM_RADIUS),
		CVector(0.0f, ITEM_HEIGHT, -ITEM_WIDTH + ITEM_RADIUS),
		ITEM_RADIUS
	);
}
