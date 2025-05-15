#include "CRespawnArea.h"
#include "CColliderSphere.h"
#include "CPlayer.h"

// コンストラクタ
CRespawnArea::CRespawnArea(CVector respawnPos, float radius)
	: CObjectBase(ETag::eRespawnArea, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
{
	// 座標を設定
	Position(respawnPos);
	// コライダーを生成
	CreateCol(radius);
}

// デストラクタ
CRespawnArea::~CRespawnArea()
{
	SAFE_DELETE(mpRespawnCol);
}

// 衝突処理
void CRespawnArea::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (mpRespawnCol == self)
	{
		// プレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// プレイヤーのリスポーン地点に設定
			CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
			player->SetRespawnPos(Position());
		}
	}
}

// コライダーを生成
void CRespawnArea::CreateCol(float radius)
{
	mpRespawnCol = new CColliderSphere
	(
		this, ELayer::eRespawnArea,
		radius, true
	);
	// プレイヤーとだけ衝突判定
	mpRespawnCol->SetCollisionLayers({ ELayer::ePlayer });
}
