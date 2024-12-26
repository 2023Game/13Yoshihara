#include "CTrashVehicleSpawnZone.h"
#include "CVehicleBase.h"

// コンストラクタ
CTrashVehicleSpawnZone::CTrashVehicleSpawnZone()
	: CObjectBase(ETag::eSpawnZone, ETaskPriority::eSpawnZone, 0, ETaskPauseType::eGame)
{
	// 車両とだけ衝突する
	// 車両の生成場所のコライダ
	mpSpawnZoneColliderMesh = new CColliderMesh
	(
		this, ELayer::eSpawnZone,
		CResourceManager::Get<CModel>("TrashStage_SpawnZone_Collision"),
		true
	);
	mpSpawnZoneColliderMesh->SetCollisionTags({ ETag::eVehicle });
	mpSpawnZoneColliderMesh->SetCollisionLayers({ ELayer::eVehicle });

}

// デストラクタ
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
	// 生成場所のコライダ
	if (self == mpSpawnZoneColliderMesh)
	{
		// 衝突相手が車両の場合
		if (other->Layer() == ELayer::eVehicle)
		{
			// 車両クラスを取得
			CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());
			
			// 相手がいる道に生成できないのでfalse
			// 左から1番の道なら
			if (vehicle->GetRoadType() == CVehicleBase::ERoadType::eLeft1)
			{
				SetLeft1CanPop(false);
			}
			// 左から2番の道なら
			else if (vehicle->GetRoadType() == CVehicleBase::ERoadType::eLeft2)
			{
				SetLeft2CanPop(false);
			}
			// 右から1番の道なら
			else if (vehicle->GetRoadType() == CVehicleBase::ERoadType::eRight1)
			{
				SetRight1CanPop(false);
			}
			// 右から2番の道なら
			else
			{
				SetRight2CanPop(false);
			}
		}
	}
}

// 生成可能かのデータを取得
CTrashVehicleSpawnZone::IsCanPops CTrashVehicleSpawnZone::GetCanPops() const
{
	return mIsCanPops;
}

/*
生成可能かを設定
*/
// 左から1番
void CTrashVehicleSpawnZone::SetLeft1CanPop(bool canPop)
{
	mIsCanPops.IsLeft1CanPop = canPop;
}
// 左から2番
void CTrashVehicleSpawnZone::SetLeft2CanPop(bool canPop)
{
	mIsCanPops.IsLeft2CanPop = canPop;
}
// 右から1番
void CTrashVehicleSpawnZone::SetRight1CanPop(bool canPop)
{
	mIsCanPops.IsRight1CanPop = canPop;
}
// 右から2番
void CTrashVehicleSpawnZone::SetRight2CanPop(bool canPop)
{
	mIsCanPops.IsRight2CanPop = canPop;
}
