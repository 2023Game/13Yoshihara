#include "CDeliveryItem.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CPlayerBase.h"

#include "CInput.h"

#define ITEM_HEIGHT	2.5f	// 高さ
#define ITEM_WIDTH	4.5f	// 幅
#define ITEM_RADIUS	2.75f	// 半径

// 消滅までの時間
#define DELETE_TIME 3.0f

// コンストラクタ
CDeliveryItem::CDeliveryItem()
	: CObjectBase(ETag::eBullet, ETaskPriority::eWeapon, 
		0, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
{
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
	Position(Position() + mMoveSpeed);

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
		ITEM_RADIUS
	);
}
