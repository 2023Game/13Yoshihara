#include "CTrashVehicleSpawnZone.h"

CTrashVehicleSpawnZone::CTrashVehicleSpawnZone(float left1, float left2, float right1, float right2)
	: CCharaBase(ETag::eSpawnZone, ETaskPriority::eDefault)
	, mLeft1PosX(left1)
	, mLeft2PosX(left2)
	, mRight1PosX(right1)
	, mRight2PosX(right2)
{
	// �ԗ��̐����ꏊ�̃R���C�_
	mpSpawnZoneColliderMesh = new CColliderMesh(this, ELayer::eSpawnZone, CResourceManager::Get<CModel>("TrashStage_SpawnZone_Collision"), true);
}

CTrashVehicleSpawnZone::~CTrashVehicleSpawnZone()
{
	if (mpSpawnZoneColliderMesh != nullptr)
	{
		delete mpSpawnZoneColliderMesh;
		mpSpawnZoneColliderMesh = nullptr;
	}
}

void CTrashVehicleSpawnZone::Update()
{
	SetLeft1CanPop(false);
	SetLeft2CanPop(false);
	SetRight1CanPop(false);
	SetRight2CanPop(false);
}

void CTrashVehicleSpawnZone::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpSpawnZoneColliderMesh)
	{
		// �Փˑ��肪�ԗ��̏ꍇ
		if (other->Layer() == ELayer::eVehicle)
		{
			// X���W�����ē������
			// �Ή�����bool��true�ɂ���
			// ������1�Ԃ̓��Ȃ�
			if (other->GetParent()->Position().X() == mLeft1PosX)
			{
				SetLeft1CanPop(true);
			}
			// ������2�Ԃ̓��Ȃ�
			else if (other->GetParent()->Position().X() == mLeft2PosX)
			{
				SetLeft2CanPop(true);
			}
			// �E����1�Ԃ̓��Ȃ�
			else if (other->GetParent()->Position().X() == mRight1PosX)
			{
				SetRight1CanPop(true);
			}
			// �E����2�Ԃ̓��Ȃ�
			else
			{
				SetRight2CanPop(true);
			}
		}
	}
}

// �����\���̃f�[�^���擾
CTrashVehicleSpawnZone::IsCanPops CTrashVehicleSpawnZone::GetCanPops() const
{
	return mIsCanPops;
}

/*
�����\����ݒ�
*/
// ������1��
void CTrashVehicleSpawnZone::SetLeft1CanPop(bool canPop)
{
	mIsCanPops.IsLeft1CanPop = canPop;
}
// ������2��
void CTrashVehicleSpawnZone::SetLeft2CanPop(bool canPop)
{
	mIsCanPops.IsLeft2CanPop = canPop;
}
// �E����1��
void CTrashVehicleSpawnZone::SetRight1CanPop(bool canPop)
{
	mIsCanPops.IsRight1CanPop = canPop;
}
// �E����2��
void CTrashVehicleSpawnZone::SetRight2CanPop(bool canPop)
{
	mIsCanPops.IsRight2CanPop = canPop;
}
