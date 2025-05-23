#include "CDeliveryItem.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemy.h"
#include "CSound.h"

#define ITEM_HEIGHT	2.5f	// 高さ
#define ITEM_WIDTH	4.5f	// 幅
#define ITEM_RADIUS	2.75f	// 半径

// 消滅するZ座標
#define DELETE_POSZ 250.0f
// 消滅までの時間
#define DELETE_TIME 3.0f

#define SE_VOLUME 0.25f

// スケールの倍率
#define SCALE_RATIO 5.0f

// コンストラクタ
CDeliveryItem::CDeliveryItem(CObjectBase* owner)
	: CObjectBase(ETag::eBullet, ETaskPriority::eWeapon,
		0, ETaskPauseType::eGame)
	, CDeliveryItemStatus()
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mpOwner(owner)
	, mOwnerPos(owner->Position())
	, mIsMove(true)
	, mIsPlayer(false)
{
	// プレイヤーが持ち主なら
	if (mpOwner == CDeliveryPlayer::Instance())
	{
		// 発射した数を増やす
		CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(mpOwner);
		player->IncreaseShotNum();
		// プレイヤーが持ち主
		mIsPlayer = true;
	}
	Scale(Scale() * SCALE_RATIO);
	// ゴール音を取得
	mpGoalSE = CResourceManager::Get<CSound>("GoalSE");
	// ヒット音を取得
	mpHitSE = CResourceManager::Get<CSound>("HitSE");
	// ダメージ音を取得
	mpDamageSE = CResourceManager::Get<CSound>("DamageSE");
	// モデル取得
	mpModel = CResourceManager::Get<CModel>("DeliveryItem");
	
	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CDeliveryItem::~CDeliveryItem()
{
	SAFE_DELETE(mpBodyCol);
}

// 更新
void CDeliveryItem::Update()
{
	// 移動するなら
	if (mIsMove)
	{
		Position(Position() + mMoveSpeed);
	}
	else
	{
		CVector move = mpOwner->Position() - mOwnerPos;
		Position(Position() + move);
		mOwnerPos = mpOwner->Position();
	}

	// 消滅するZ座標より手前にいるなら
	if (Position().Z() > DELETE_POSZ)
	{
		// 削除
		Kill();
	}

	// 消滅までの時間が経過していないなら
	if (mElapsedTime < DELETE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	// 消滅までの時間が経過したら
	else
	{
		// 削除
		Kill();
	}
}

// 描画
void CDeliveryItem::Render()
{
	mpModel->Render(Matrix());
}

// 衝突処理
void CDeliveryItem::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// プレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// コライダーと自分の持ち主が同じなら処理しない
			if (mpOwner == other->Owner()) return;
			// プレイヤー取得
			CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(other->Owner());
			player->TakeDamage(GetDamage(), this);
			// ダメージ音を再生
			mpDamageSE->Play(SE_VOLUME, true);
			// 消滅
			Kill();
		}
		// 敵の場合
		else if (other->Layer() == ELayer::eEnemy)
		{
			// コライダーと自分の持ち主が同じなら処理しない
			if (mpOwner == other->Owner()) return;
			// 敵取得
			CDeliveryEnemy* enemy = dynamic_cast<CDeliveryEnemy*>(other->Owner());
				enemy->TakeDamage(GetDamage(), this);
			// プレイヤーが持ち主なら
			if (mIsPlayer)
			{
				// 当たった数を増やす
				CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(mpOwner);
				player->IncreaseHitNum();
			}
			// ダメージ音を再生
			mpDamageSE->Play(SE_VOLUME, true);
			// 消滅
			Kill();
		}
		// ゴールの場合
		else if (other->Layer() == ELayer::eGoal)
		{
			// 持ち主がプレイヤー
			if (mIsPlayer)
			{
				// 配達した数を1増やす
				CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(mpOwner);
				player->IncreaseDeliveryNum();
				// 当たった数を増やす
				player->IncreaseHitNum();
				// ゴール音を再生
				mpGoalSE->Play(SE_VOLUME, true);
			}
			// 動きを停止
			mIsMove = false;
			// 衝突判定を停止
			SetEnableCol(false);
			// 持ち主をフィールドにする
			mpOwner = other->Owner();
			// フィールドの座標を取得
			mOwnerPos = mpOwner->Position();
		}
		// 壁の場合
		else if (other->Layer() == ELayer::eWall)
		{
			// 動きを停止
			mIsMove = false;
			// 衝突判定を停止
			SetEnableCol(false);
			// 持ち主をフィールドにする
			mpOwner = other->Owner();
			// フィールドの座標を取得
			mOwnerPos = mpOwner->Position();
			// ヒット音を再生
			mpHitSE->Play(SE_VOLUME, true);
		}
		// 発射物の場合
		else if (other->Layer() == ELayer::eAttackCol)
		{
			// 発射する配達物の取得
			CDeliveryItem* item = dynamic_cast<CDeliveryItem*>(other->Owner());

			// 持ち主が同じなら処理しない
			if (mpOwner == item->GetOwner()) return;
			// プレイヤーが持ち主なら
			if (mIsPlayer)
			{
				// 当たった数を増やす
				CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(mpOwner);
				player->IncreaseHitNum();
			}

			// ヒット音を再生
			mpHitSE->Play(SE_VOLUME, true);
			// 消滅
			Kill();
		}
	}
}

// 移動を設定
void CDeliveryItem::SetMoveSpeed(CVector moveSpeed)
{
	mMoveSpeed = moveSpeed;
}

// 持ち主を取得する
CObjectBase* CDeliveryItem::GetOwner()
{
	return mpOwner;
}

// コライダ―を生成
void CDeliveryItem::CreateCol()
{
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, ITEM_HEIGHT, ITEM_WIDTH - ITEM_RADIUS),
		CVector(0.0f, ITEM_HEIGHT, -ITEM_WIDTH + ITEM_RADIUS),
		ITEM_RADIUS * Scale().X()
	);
	// プレイヤー、敵、ゴール、発射物、壁、
	// と衝突判定
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,
		ETag::eField,ETag::eBullet });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eGoal,ELayer::eAttackCol,ELayer::eWall });
}
