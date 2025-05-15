#include "CRespawnArea.h"
#include "CColliderSphere.h"
#include "CPlayer.h"

// �R���X�g���N�^
CRespawnArea::CRespawnArea(CVector respawnPos, float radius)
	: CObjectBase(ETag::eRespawnArea, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
{
	// ���W��ݒ�
	Position(respawnPos);
	// �R���C�_�[�𐶐�
	CreateCol(radius);
}

// �f�X�g���N�^
CRespawnArea::~CRespawnArea()
{
	SAFE_DELETE(mpRespawnCol);
}

// �Փˏ���
void CRespawnArea::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (mpRespawnCol == self)
	{
		// �v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			// �v���C���[�̃��X�|�[���n�_�ɐݒ�
			CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
			player->SetRespawnPos(Position());
		}
	}
}

// �R���C�_�[�𐶐�
void CRespawnArea::CreateCol(float radius)
{
	mpRespawnCol = new CColliderSphere
	(
		this, ELayer::eRespawnArea,
		radius, true
	);
	// �v���C���[�Ƃ����Փ˔���
	mpRespawnCol->SetCollisionLayers({ ELayer::ePlayer });
}
