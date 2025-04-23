#include "CWand.h"
#include "CModel.h"
#include "CConnectPoint.h"

// レイを伸ばせる距離
#define RAY_DISTANCE 10.0f

// 接続部のオフセット座標
#define POINT_OFFSET_POSX 0.0f
#define POINT_OFFSET_POSY 10.0f
#define POINT_OFFSET_POSZ 5.0f
// 接続部のスケール
#define POINT_SCALE 0.5f


// コンストラクタ
CWand::CWand(CObjectBase* owner, ETag tag)
	: CWeapon(tag)
	, mpModel(nullptr)
{
	mpOwner = owner;

	// モデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Wand");

	// 接続部分のビルボード
	mpPoint = new CConnectPoint();
	// 最初は非表示
	mpPoint->SetEnable(false);
	mpPoint->SetShow(false);
	// スケール調整
	mpPoint->Scale(mpPoint->Scale() * POINT_SCALE);
}

// デストラクタ
CWand::~CWand()
{
}

// 更新
void CWand::Update()
{
	CVector offsetPos = mpOwner->Position();
	offsetPos += mpOwner->VectorX() * POINT_OFFSET_POSX;
	offsetPos += mpOwner->VectorY() * POINT_OFFSET_POSY;
	offsetPos += mpOwner->VectorZ() * POINT_OFFSET_POSZ;
	mpPoint->Position(offsetPos);
}

// 描画
void CWand::Render()
{
	mpModel->Render(Matrix());
}

// 衝突判定を行うコライダーをリストに追加
void CWand::AddCollider(CCollider* col)
{
	mColliders.push_back(col);
}

// 衝突判定を行うコライダーをリストから取り除く
void CWand::RemoveCollider(CCollider* col)
{
	mColliders.remove(col);
}

// 設定されているコライダーと衝突判定を行う
bool CWand::Ray(CVector& hitPos)
{
	CHitInfo hit;
	// カメラを取得
	CCamera* camera = CCamera::CurrentCamera();
	// 視点から注視点への方向
	CVector dir = camera->GetEyeVec();
	dir.Normalize();
	// レイの開始と終了地点
	CVector rayStart = camera->GetEye();
	CVector rayEnd = dir * RAY_DISTANCE;
	float nearDist = 0.0f;
	bool isHit = false;
	// 設定されているコライダーを順番に調べる
	for (CCollider* c : mColliders)
	{
		// レイとコライダーの衝突判定を行う
		if (CCollider::CollisionRay(c, rayStart, rayEnd, &hit))
		{
			// 交点が不整値でなければ、
			if (hit.cross.LengthSqr() != 0.0f)
			{
				// 衝突位置までの距離で一番近い距離を求める
				if (!isHit)
				{
					nearDist = hit.dist;
					hitPos = hit.cross;
				}
				else if (nearDist > hit.dist)
				{
					nearDist = hit.dist;
					hitPos = hit.cross;
				}
				isHit = true;
			}
		}
	}

	return isHit;
}

// 接続部の有効無効を設定
void CWand::SetConnect(bool isOnOff)
{
	mpPoint->SetEnable(isOnOff);
	mpPoint->SetShow(isOnOff);
}

// 接続されているか
bool CWand::GetConnect()
{
	return mpPoint->IsShow();
}
