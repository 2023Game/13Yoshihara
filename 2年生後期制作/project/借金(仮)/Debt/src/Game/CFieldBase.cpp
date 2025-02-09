#include "CFieldBase.h"
#include "CSceneManager.h"
#include <assert.h>

// フィールドのインスタンス
CFieldBase* CFieldBase::spInstance = nullptr;

// フィールドのインスタンスを返す
CFieldBase* CFieldBase::Instance()
{
	return spInstance;
}

// コンストラクタ
CFieldBase::CFieldBase()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpSkyModel(nullptr)
	, mpGroundColliderMesh(nullptr)
	, mpWallColliderMesh(nullptr)
	, mpObjectColliderMesh(nullptr)
{
	//assert(spInstance == nullptr);
	spInstance = this;
}

// デストラクタ
CFieldBase::~CFieldBase()
{
	SAFE_DELETE(mpGroundColliderMesh);
	SAFE_DELETE(mpWallColliderMesh);
	SAFE_DELETE(mpObjectColliderMesh);

	// インスタンスと削除しているフィールドが同一なら削除
	if (spInstance == this)
	{
		spInstance = nullptr;
	}
}

//レイとフィールドオブジェクトの衝突判定
bool CFieldBase::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// 衝突情報保存用
	CHitInfo tHit;
	// 衝突したかどうかフラグ
	bool isHit = false;
	// フィールドの地面との衝突判定
	if (CCollider::CollisionRay(mpGroundColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = true;
	}
	// フィールドの壁との衝突判定
	if (CCollider::CollisionRay(mpWallColliderMesh, start, end, &tHit))
	{
		// まだ他に衝突していない場合か、
		// 既に衝突しているコライダ―より近い場合は、
		if (!isHit || tHit.dist < hit->dist)
		{
			// 衝突情報を更新
			*hit = tHit;
			isHit = true;
		}
	}
	// フィールドのオブジェクトとの衝突判定
	if (CCollider::CollisionRay(mpObjectColliderMesh, start, end, &tHit))
	{
		// まだ他に衝突していない場合か、
		// 既に衝突しているコライダ―より近い場合は、
		if (!isHit || tHit.dist < hit->dist)
		{
			// 衝突情報を更新
			*hit = tHit;
			isHit = true;
		}
	}
	
	return isHit;
}

// 更新
void CFieldBase::Update()
{
}

// 描画
void CFieldBase::Render()
{
	mpModel->Render(Matrix());
}

// 地面のコライダ―取得
CCollider* CFieldBase::GetGroundCol()
{
	return mpGroundColliderMesh;
}

// 壁のコライダ―取得
CCollider* CFieldBase::GetWallCol()
{
	return mpWallColliderMesh;
}

// オブジェクトのコライダ―取得
CCollider* CFieldBase::GetObjCol()
{
	return mpObjectColliderMesh;
}

// 経路探索用のノードを作成
void CFieldBase::CreateNavNodes()
{
}
