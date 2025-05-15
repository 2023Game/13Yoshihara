#include "CPortal.h"
#include "CSceneManager.h"

// ��]���x
#define ROT_SPEED 200.0f

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

// �Փˏ���
void CPortal::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpCol)
	{
		// �v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			// ���̃X�e�[�W��
			CSceneManager::Instance()->LoadScene(EScene::eGameTest);
		}
	}
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
