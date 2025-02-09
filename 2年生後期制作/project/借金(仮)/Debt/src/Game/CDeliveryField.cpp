#include "CDeliveryField.h"
#include "CDeliveryHome.h"
#include "Maths.h"

#define HOME_OFFSET_POSX 250.0f
#define HOME_OFFSET_POSY 12.0f

// �����ړ����x
#define FORCE_MOVE_SPEED 7.5f * 60.0f	

// �R���X�g���N�^
CDeliveryField::CDeliveryField()
	: CFieldBase()
	, mHomeOffsetPos(CVector::zero)
{
	// �X�P�[����ݒ�
	Scale(Scale() * SCALE_RATIO);
	mpModel = CResourceManager::Get<CModel>("DeliveryStage");
	mpGroundColliderMesh = new CColliderMesh
	(
		this, ELayer::eGround,
		CResourceManager::Get<CModel>
		(
			"DeliveryStage_Ground_Collision"
		),
		true
	);
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
	// �Ƃ̈ʒu�������_���Ɍ��߂�
	RandomHome();
}

// �f�X�g���N�^
CDeliveryField::~CDeliveryField()
{
	SAFE_DELETE(mpGroundColliderMesh);
}

// �X�V
void CDeliveryField::Update()
{
	float moveSpeed = FORCE_MOVE_SPEED * Times::DeltaTime();
	Position(Position() + CVector::forward * moveSpeed);
	mpHome->Position(Position() + mHomeOffsetPos);
}

// �`��
void CDeliveryField::Render()
{
	CFieldBase::Render();
}

// �Ƃ̈ʒu�������_���Ɍ��߂�
void CDeliveryField::RandomHome()
{
	int random = Math::Rand(0, 1);
	if (random == 0)
	{
		mpHome->Rotation(0.0f, 90.0f, 0.0f);
		mHomeOffsetPos = CVector(HOME_OFFSET_POSX, HOME_OFFSET_POSY, 0.0f);
	}
	else
	{
		mpHome->Rotation(0.0f, -90.0f, 0.0f);
		mHomeOffsetPos = CVector(-HOME_OFFSET_POSX, HOME_OFFSET_POSY, 0.0f);
	}
	random = Math::Rand(-25, 25);
	mHomeOffsetPos.Z((float)random * 10.0f);
}

// �t�B�[���h�̃I�u�W�F�N�g����
void CDeliveryField::CreateFieldObjects()
{
	mpHome = new CDeliveryHome();
	mpHome->Rotation(0.0f, 90.0f, 0.0f);
	mHomeOffsetPos = CVector(HOME_OFFSET_POSX, HOME_OFFSET_POSY, 0.0f);
}