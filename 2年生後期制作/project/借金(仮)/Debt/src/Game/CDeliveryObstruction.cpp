#include "CDeliveryObstruction.h"
#include "CDeliveryField.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemy.h"
#include "CSound.h"

// スケール倍率
#define SCALE 2.0f

// 本体コライダ―
#define BODY_RADIUS 10.0f
// コライダーのオフセット座標
#define BODY_OFFSET_POS CVector(0.0f, BODY_RADIUS / 2, 0.0f)

// ダメージ
#define DAMAGE 1

#define SE_VOLUME 0.5f

// コンストラクタ
CDeliveryObstruction::CDeliveryObstruction()
	: CObjectBase(ETag::eObstruction, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mRoadType(ERoadType::eLeft1)
{
	mpHitSE = CResourceManager::Get<CSound>("DamageSE");
	// スケールを設定
	Scale(Scale() * 2.0f);
	// モデル取得
	mpModel = CResourceManager::Get<CModel>("Obstruction");
	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CDeliveryObstruction::~CDeliveryObstruction()
{
	SAFE_DELETE(mpBodyCol);
}

// 更新
void CDeliveryObstruction::Update()
{
	// 強制移動の速度分、手前へ移動
	float moveSpeed = FORCE_MOVE_SPEED * Times::DeltaTime();
	Position(Position() + CVector::forward * moveSpeed);
}

// 描画
void CDeliveryObstruction::Render()
{
	mpModel->Render(Matrix());
}

// 衝突処理
void CDeliveryObstruction::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// プレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// プレイヤー取得
			CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(other->Owner());
			// ダメージを与える
			player->TakeDamage(DAMAGE, this);
			// 衝突音声再生
			mpHitSE->Play(SE_VOLUME, true);
			// 無効
			SetEnable(false);
			SetShow(false);
		}
		// 敵の場合
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 敵取得
			CDeliveryEnemy* enemy = dynamic_cast<CDeliveryEnemy*>(other->Owner());
			// ダメージを与える
			enemy->TakeDamage(DAMAGE, this);
			// 衝突音声再生
			mpHitSE->Play(SE_VOLUME, true);
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
	}
}

// どの道にいるかを取得する
ERoadType CDeliveryObstruction::GetRoadType() const
{
	return mRoadType;
}

// どの道にいるか設定する
void CDeliveryObstruction::SetRoadType(ERoadType roadType)
{
	mRoadType = roadType;
}

// コライダ―を生成
void CDeliveryObstruction::CreateCol()
{
	// 本体コライダ―
	mpBodyCol = new CColliderSphere
	(
		this, ELayer::eObstruction,
		BODY_RADIUS,
		true
	);
	// プレイヤー、敵、発射物、アイテム、探知
	// と衝突判定
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,
		ETag::eBullet,ETag::eItem });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eAttackCol,ELayer::eItem,ELayer::eSearch });
	mpBodyCol->Position(BODY_OFFSET_POS);
}
