#include "CTrashVehicleSpawnZone.h"

CTrashVehicleSpawnZone::CTrashVehicleSpawnZone(float left1, float left2, float right1, float right2)
	: CCharaBase(ETag::eSpawnZone, ETaskPriority::eDefault)
	, mLeft1PosX(left1)
	, mLeft2PosX(left2)
	, mRight1PosX(right1)
	, mRight2PosX(right2)
{
	// 車両の生成場所のコライダ
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
		// 衝突相手が車両の場合
		if (other->Layer() == ELayer::eVehicle)
		{
			// X座標を見て道を特定
			// 対応するboolをtrueにする
			// 左から1番の道なら
			if (other->GetParent()->Position().X() == mLeft1PosX)
			{
				SetLeft1CanPop(true);
			}
			// 左から2番の道なら
			else if (other->GetParent()->Position().X() == mLeft2PosX)
			{
				SetLeft2CanPop(true);
			}
			// 右から1番の道なら
			else if (other->GetParent()->Position().X() == mRight1PosX)
			{
				SetRight1CanPop(true);
			}
			// 右から2番の道なら
			else
			{
				SetRight2CanPop(true);
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
