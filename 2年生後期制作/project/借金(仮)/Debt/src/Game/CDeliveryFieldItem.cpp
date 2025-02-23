#include "CDeliveryFieldItem.h"
#include "CDeliveryField.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemy.h"
#include "CColliderSphere.h"
#include "CSound.h"

// 本体コライダ―の半径
#define BODY_RADIUS 5.0f
// コライダ―のオフセット座標
#define BODY_OFFSET_POS CVector(0.0f, BODY_RADIUS / 2, 0.0f)

// スケールの最小
#define SCALE_MIN 3.0f
// スケールの最大
#define SCALE_MAX 4.0f
// スケール変化速度
#define SCALE_CHANGE_SPEED CVector(1.0f,1.0f,1.0f)

// 回転速度
#define ROT_SPEED CVector(0.0f, -90.0f, 0.0f)

// 拾って得られる個数
#define NUM 1

#define SE_VOLUME 0.5f

// コンストラクタ
CDeliveryFieldItem::CDeliveryFieldItem()
	: CObjectBase(ETag::eItem, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mIsBig(false)
	, mRoadType(ERoadType::eLeft1)
{
	mpGetSE = CResourceManager::Get<CSound>("GetSE");
	// スケールを設定
	Scale(Scale() * SCALE_MIN);
	// モデル取得
	mpModel = CResourceManager::Get<CModel>("DeliveryItem");
	// コライダーを生成
	CreateCol();
}

// デストラクタ
CDeliveryFieldItem::~CDeliveryFieldItem()
{
	SAFE_DELETE(mpBodyCol);
}

// 更新
void CDeliveryFieldItem::Update()
{
	// 強制移動の速度分、手前へ移動
	float moveSpeed = FORCE_MOVE_SPEED * Times::DeltaTime();
	Position(Position() + CVector::forward * moveSpeed);
	// 回転する
	Rotation(EulerAngles() + ROT_SPEED * Times::DeltaTime());

	// 大きい場合
	if (mIsBig)
	{
		// 小さくしていく
		CVector scale = Scale() - SCALE_CHANGE_SPEED * Times::DeltaTime();
		Scale(scale);
		// 最小より小さくなれば
		if (scale.X() < SCALE_MIN)
		{
			// 小さい
			mIsBig = false;
		}
	}
	// 小さい場合
	else if (!mIsBig)
	{
		// 大きくしていく
		CVector scale = Scale() + SCALE_CHANGE_SPEED * Times::DeltaTime();
		Scale(scale);
		// 最大より大きくなれば
		if (scale.X() > SCALE_MAX)
		{
			// 大きい
			mIsBig = true;
		}
	}
}

// 描画
void CDeliveryFieldItem::Render()
{
	mpModel->Render(Matrix());
}

// 衝突処理
void CDeliveryFieldItem::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// プレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// プレイヤー取得
			CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(other->Owner());
			// HPを増やす
			player->SetHp(NUM);
			// 獲得音声再生
			mpGetSE->Play(SE_VOLUME, true);
			// 無効
			SetEnable(false);
			SetShow(false);
		}
		// 敵の場合
		else if (other->Layer() == ELayer::eEnemy)
		{
			// プレイヤー取得
			CDeliveryEnemy* enemy = dynamic_cast<CDeliveryEnemy*>(other->Owner());
			// 死んでいたら処理しない
			if (enemy->IsDeath()) return;
			// HPを増やす
			enemy->SetHp(NUM);
			// 獲得音声再生
			mpGetSE->Play(SE_VOLUME, true);
			// 無効
			SetEnable(false);
			SetShow(false);
		}
		// 攻撃判定の場合
		else if (other->Layer() == ELayer::eAttackCol)
		{
			// 無効
			SetEnable(false);
			SetShow(false);
		}
		// 障害物の場合
		else if (other->Layer() == ELayer::eObstruction)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);

		}
	}
}

// どの道にいるかを取得する
ERoadType CDeliveryFieldItem::GetRoadType() const
{
	return mRoadType;
}

// どの道にいるか設定する
void CDeliveryFieldItem::SetRoadType(ERoadType roadType)
{
	mRoadType = roadType;
}

// コライダーを生成
void CDeliveryFieldItem::CreateCol()
{
	mpBodyCol = new CColliderSphere
	(
		this,ELayer::eItem,
		BODY_RADIUS
	);
	// プレイヤー、敵、発射物、障害物、探知
	// と衝突判定
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,
		ETag::eBullet,ETag::eObstruction });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eAttackCol,ELayer::eObstruction,ELayer::eSearch });
	// 座標を設定
	mpBodyCol->Position(BODY_OFFSET_POS);
}
