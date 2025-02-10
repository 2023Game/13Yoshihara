#include "CDeliveryItem.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemy.h"
#include "CSound.h"

#define ITEM_HEIGHT	2.5f	// 高さ
#define ITEM_WIDTH	4.5f	// 幅
#define ITEM_RADIUS	2.75f	// 半径

// 消滅までの時間
#define DELETE_TIME 3.0f

#define SE_VOLUME 0.5f

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
	mpGoalSE = CResourceManager::Get<CSound>("GetSE");
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
			// 消滅
			Kill();
		}
		// ゴールの場合
		else if (other->Layer() == ELayer::eGoal)
		{
			// 持ち主がプレイヤー
			CDeliveryPlayer* owner = dynamic_cast<CDeliveryPlayer*>(mpOwner);
			if (owner != nullptr)
			{
				// 配達した数を1増やす
				owner->IncreaseDeliveryNum();
				// 当たった数を増やす
				owner->IncreaseHitNum();
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
		}
	}
}

// 移動を設定
void CDeliveryItem::SetMoveSpeed(CVector moveSpeed)
{
	mMoveSpeed = moveSpeed;
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
}
