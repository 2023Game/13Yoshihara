#include "CGarbageTruck.h"
#include "CColliderCapsule.h"
#include "CModel.h"

#define TRUCK_HEIGHT	13.0f	// トラックの高さ
#define TRUCK_WIDTH		27.0f	// トラックの幅
#define TRUCK_RADIUS	15.0f	// トラックの半径

// コンストラクタ
CGarbageTruck::CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road)
	: CVehicleBase(model, pos, rotation, road)
{
	// プレイヤー、敵、生成場所、車両、地形
	// と衝突判定する本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eVehicle,
		CVector(0.0f,TRUCK_HEIGHT,TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f,TRUCK_HEIGHT,-TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS, true
	);
	mpBodyCol->Position(0.0f, 0.0f, -2.0f);
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,ETag::eSpawnZone,
		ETag::eVehicle,ETag::eField, });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eSpawnZone,ELayer::eVehicle,
		ELayer::eField,ELayer::eWall,ELayer::eObject });

	// 最初は描画、更新しない
	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CGarbageTruck::~CGarbageTruck()
{
}

// 更新
void CGarbageTruck::Update()
{
	switch (mState)
	{
	// 移動
	case EState::eMove:			UpdateMove();		break;
	// 停止
	case EState::eStop:			UpdateStop();		break;
	// 壊れた
	case EState::eBroken:		UpdateBroken();		break;
	// 車線変更
	case EState::eChangeRoad:	UpdateChangeRoad();	break;

	// 回収
	case EState::eCollect:		UpdateCollect();	break;
	}

	CVehicleBase::Update();

#if _DEBUG
	CDebugPrint::Print("TruckState:%s\n", GetStateStr(mState).c_str());
#endif
}

// 衝突処理
void CGarbageTruck::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CVehicleBase::Collision(self, other, hit);

	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// 衝突した相手がプレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// 状態を停止に変更
			ChangeState(EState::eStop);
		}
	}
}


// 回収処理
void CGarbageTruck::UpdateCollect()
{
}

