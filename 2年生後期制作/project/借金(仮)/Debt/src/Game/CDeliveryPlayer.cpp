#include "CDeliveryPlayer.h"
#include "CModel.h"
#include "CColliderCapsule.h"

#define TRUCK_HEIGHT	13.0f	// �g���b�N�̍���
#define TRUCK_WIDTH		32.5f	// �g���b�N�̕�
#define TRUCK_RADIUS	12.5f	// �g���b�N�̔��a
// �{�̃R���C�_�\�̃I�t�Z�b�g���W
#define BODY_COL_OFFSET_POS CVector(0.0f,0.0f,-3.0f)

#define EYE_HEIGHT 5.0f		// ���_�̍���

// �R���X�g���N�^
CDeliveryPlayer::CDeliveryPlayer()
	: CObjectBase(ETag::eVehicle, ETaskPriority::eVehicle)
{
	mpModel = CResourceManager::Get<CModel>("DeliveryPlayer");

	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CDeliveryPlayer::~CDeliveryPlayer()
{
}

// �X�V
void CDeliveryPlayer::Update()
{
}

// �Փˏ���
void CDeliveryPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// �`��
void CDeliveryPlayer::Render()
{
	mpModel->Render(Matrix());
}

// �R���C�_�\�𐶐�
void CDeliveryPlayer::CreateCol()
{
	// �傫���̎擾
	float scale = Scale().X();

	// �{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, TRUCK_HEIGHT, TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS
	);
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// �n�`�A�v���C���[�A��Q���A���˕��̍U������
	// �ƏՓ˔��������
	mpBodyCol->SetCollisionTags({ ETag::eField,ETag::ePlayer,
		ETag::eObstruction,ETag::eBullet });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,ELayer::ePlayer,
		ELayer::eObstruction,ELayer::eAttackCol });
}
