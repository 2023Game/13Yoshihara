#include "CGarbageTruck.h"
#include "CColliderCapsule.h"
#include "CModel.h"
#include "CNavNode.h"

#define TRUCK_HEIGHT	13.0f	// トラックの高さ
#define TRUCK_WIDTH		27.0f	// トラックの幅
#define TRUCK_RADIUS	15.0f	// トラックの半径

// ノードの座標
#define NODE_POS0	CVector( 20.0f,0.0f, 35.0f)
#define NODE_POS1	CVector(-20.0f,0.0f, 35.0f)
#define NODE_POS2	CVector(-20.0f,0.0f,-35.0f)
#define NODE_POS3	CVector( 20.0f,0.0f,-35.0f)

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

	// 移動しているもしくは、自身が有効でない場合
	if (mIsMove || !IsEnable())
	{
		// ノードを無効にする
		mpNode0->SetEnable(false);
		mpNode1->SetEnable(false);
		mpNode2->SetEnable(false);
		mpNode3->SetEnable(false);
	}
	// 自身が有効で移動していない場合
	else
	{
		// ノードを有効にする
		mpNode0->SetEnable(true);
		mpNode1->SetEnable(true);
		mpNode2->SetEnable(true);
		mpNode3->SetEnable(true);
		// ノードを車両が止まっている座標の周りに設定する
		mpNode0->SetPos(Position() + NODE_POS0);
		mpNode1->SetPos(Position() + NODE_POS1);
		mpNode2->SetPos(Position() + NODE_POS2);
		mpNode3->SetPos(Position() + NODE_POS3);
	}

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
		// 衝突した相手が敵の場合
		if (other->Layer() == ELayer::eEnemy)
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

