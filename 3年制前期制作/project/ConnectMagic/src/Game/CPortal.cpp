#include "CPortal.h"

// ��]���x
#define ROT_SPEED 60.0f * 2.0f

// �R���C�_�[�̔��a
#define RADIUS 5.0f

// �R���X�g���N�^
CPortal::CPortal()
	: CBillBoardImage("Effect/Portal.png", ETag::ePortal
		, ETaskPauseType::eGame)
{
	// �R���C�_�[�𐶐�
	CreateCol();

	// ��]����
	SetRotate(true);
	// ��]���x��ݒ�
	SetRotSpeed(ROT_SPEED);
}

// �f�X�g���N�^
CPortal::~CPortal()
{
	SAFE_DELETE(mpCol);
}

// �R���C�_�[�𐶐�
void CPortal::CreateCol()
{
	mpCol = new CColliderSphere
	(
		this, ELayer::ePortal,
		RADIUS, true
	);
	// �v���C���[�Ƃ����Փ˔���
	mpCol->SetCollisionLayers({ ELayer::ePlayer });
}
