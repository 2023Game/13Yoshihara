#include "CTrashVehicleSpawnZone.h"
#include "CVehicleBase.h"

// �R���X�g���N�^
CTrashVehicleSpawnZone::CTrashVehicleSpawnZone()
	: CObjectBase(ETag::eSpawnZone, ETaskPriority::eSpawnZone, 0, ETaskPauseType::eGame)
{
	// �ԗ��Ƃ����Փ˂���
	// �ԗ��̐����ꏊ�̃R���C�_
	mpSpawnZoneColliderMesh = new CColliderMesh
	(
		this, ELayer::eSpawnZone,
		CResourceManager::Get<CModel>("TrashStage_SpawnZone_Collision"),
		true
	);
	mpSpawnZoneColliderMesh->SetCollisionTags({ ETag::eVehicle });
	mpSpawnZoneColliderMesh->SetCollisionLayers({ ELayer::eVehicle });

}

// �f�X�g���N�^
CTrashVehicleSpawnZone::~CTrashVehicleSpawnZone()
{
	SAFE_DELETE(mpSpawnZoneColliderMesh);
}

void CTrashVehicleSpawnZone::Update()
{
	SetLeft1CanPop(true);
	SetLeft2CanPop(true);
	SetRight1CanPop(true);
	SetRight2CanPop(true);
}

void CTrashVehicleSpawnZone::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �����ꏊ�̃R���C�_
	if (self == mpSpawnZoneColliderMesh)
	{
		// �Փˑ��肪�ԗ��̏ꍇ
		if (other->Layer() == ELayer::eVehicle)
		{
			// �ԗ��N���X���擾
			CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());
			
			// ���肪���铹�ɐ����ł��Ȃ��̂�false
			// ������1�Ԃ̓��Ȃ�
			if (vehicle->GetRoadType() == CVehicleBase::ERoadType::eLeft1)
			{
				SetLeft1CanPop(false);
			}
			// ������2�Ԃ̓��Ȃ�
			else if (vehicle->GetRoadType() == CVehicleBase::ERoadType::eLeft2)
			{
				SetLeft2CanPop(false);
			}
			// �E����1�Ԃ̓��Ȃ�
			else if (vehicle->GetRoadType() == CVehicleBase::ERoadType::eRight1)
			{
				SetRight1CanPop(false);
			}
			// �E����2�Ԃ̓��Ȃ�
			else
			{
				SetRight2CanPop(false);
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
