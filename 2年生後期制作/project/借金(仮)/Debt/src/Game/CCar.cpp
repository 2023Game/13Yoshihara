#include "CCar.h"
#include "CColliderCapsule.h"
#include "CModel.h"
#include "CNavNode.h"
#include "CTrashPlayer.h"
#include "CTrashEnemy.h"

#define CAR_HEIGHT		9.0f	// 車の高さ
#define CAR_WIDTH		25.0f	// 車の幅
#define CAR_RADIUS		10.0f	// 車の半径

// ノードの座標
#define NODE_POS0	CVector( 15.0f,0.0f, 30.0f)
#define NODE_POS1	CVector(-15.0f,0.0f, 30.0f)
#define NODE_POS2	CVector(-15.0f,0.0f,-30.0f)
#define NODE_POS3	CVector( 15.0f,0.0f,-30.0f)

CCar::CCar(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road)
	: CVehicleBase(model, pos, rotation, road)
	, CVehicleStatus()
{
	// プレイヤー、敵、生成場所、車両、地形
	// と衝突判定する本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eVehicle,
		CVector(0.0f, CAR_HEIGHT, CAR_WIDTH - CAR_RADIUS),
		CVector(0.0f, CAR_HEIGHT, -CAR_WIDTH + CAR_RADIUS),
		CAR_RADIUS, true
	);
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,ETag::eSpawnZone,
		ETag::eVehicle,ETag::eField, });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eSpawnZone,ELayer::eVehicle,
		ELayer::eField,ELayer::eWall,ELayer::eObject });

	// 経路探索用のコライダ―作成
	mpNavCol = new CColliderCapsule
	(
		this, ELayer::eNone,
		CVector(0.0f, CAR_HEIGHT, CAR_WIDTH * 1.2f - CAR_RADIUS),
		CVector(0.0f, CAR_HEIGHT, -CAR_WIDTH * 1.2f + CAR_RADIUS),
		CAR_RADIUS, true
	);
}

CCar::~CCar()
{

}

void CCar::Update()
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
	case EState::eChangeRoad:	UpdateChangeRoad(); break;
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
	CDebugPrint::Print("CarState:%s\n", GetStateStr(mState).c_str());
#endif
}

// 衝突処理
void CCar::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CVehicleBase::Collision(self, other, hit);

	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// 衝突した相手がプレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// 移動していたら
			if (IsMove())
			{
				// プレイヤークラスを取得
				CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(other->Owner());

				// 自分から相手の方向
				CVector direction = player->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// 相手が受けるノックバック速度に、
				// 自分が与えるノックバック速度を自分から相手の方向に設定
				player->SetKnockbackReceived(direction * GetKnockbackDealt());
				// 攻撃力分のダメージを与える
				player->TakeDamage(GetAttackPower(), this);

				// 壊れた状態に変更
				ChangeState(EState::eBroken);
			}
		}
		// 衝突した相手が敵の場合
		else if (other->Layer() == ELayer::eEnemy)
		{	
			// 移動していたら
			if (IsMove())
			{
				// 敵クラスを取得
				CTrashEnemy* enemy = dynamic_cast<CTrashEnemy*>(other->Owner());

				// 自分から相手の方向
				CVector direction = enemy->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// 相手が受けるノックバック速度に、
				// 自分が与えるノックバック速度を自分から相手の方向に設定
				enemy->SetKnockbackReceived(direction * enemy->GetKnockbackDealt());
				// 攻撃力分のダメージを与える
				enemy->TakeDamage(GetAttackPower(), this);

				// 壊れた状態に変更
				ChangeState(EState::eBroken);
			}
		}
	}
}

// 移動処理
void CCar::UpdateMove()
{
	// 動いている
	mIsMove = true;
	// 正面へ移動
	mMoveSpeed = VectorZ() * GetBaseMoveSpeed();
}

// 停止処理
void CCar::UpdateStop()
{
	// 動いていない
	mIsMove = false;
	// 移動速度をゼロにする
	mMoveSpeed = CVector::zero;
}

// 壊れた処理
void CCar::UpdateBroken()
{
	// 動いていない
	mIsMove = false;
	// 移動速度をゼロにする
	mMoveSpeed = CVector::zero;

	// 消滅するまでの時間をカウントダウン
	CountDeleteTime();

	// 消滅までの時間が経過したら
	if (IsElapsedDeleteTime())
	{
		// 消滅までの時間を初期値に戻す
		SetDeleteTime();
		// 状態を移動に戻しておく
		ChangeState(EState::eMove);

		// 非表示
		SetEnable(false);
		SetShow(false);
	}
}
