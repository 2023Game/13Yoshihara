#include "CFieldBase.h"
#include "CSceneManager.h"
#include <assert.h>
#include "CPhysicsManager.h"
#include "CModel.h"

// コンストラクタ
CFieldBase::CFieldBase()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpSkyModel(nullptr)
{
}

// デストラクタ
CFieldBase::~CFieldBase()
{
}

//レイとフィールドオブジェクトの衝突判定
bool CFieldBase::CollisionRay(const CVector& start, const CVector& end)
{
	//// 衝突情報保存用
	//CHitInfo tHit;
	//// 衝突したかどうかフラグ
	//bool isHit = false;
	//// フィールドの地面との衝突判定
	//if (CCollider::CollisionRay(mpGroundColliderMesh, start, end, &tHit))
	//{
	//	*hit = tHit;
	//	isHit = true;
	//}
	//// フィールドの壁との衝突判定
	//if (CCollider::CollisionRay(mpWallColliderMesh, start, end, &tHit))
	//{
	//	// まだ他に衝突していない場合か、
	//	// 既に衝突しているコライダ―より近い場合は、
	//	if (!isHit || tHit.dist < hit->dist)
	//	{
	//		// 衝突情報を更新
	//		*hit = tHit;
	//		isHit = true;
	//	}
	//}
	//// フィールドのオブジェクトとの衝突判定
	//if (CCollider::CollisionRay(mpObjectColliderMesh, start, end, &tHit))
	//{
	//	// まだ他に衝突していない場合か、
	//	// 既に衝突しているコライダ―より近い場合は、
	//	if (!isHit || tHit.dist < hit->dist)
	//	{
	//		// 衝突情報を更新
	//		*hit = tHit;
	//		isHit = true;
	//	}
	//}
	//
	//return isHit;

	return false;
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

// 経路探索用のノードを作成
void CFieldBase::CreateNavNodes()
{
}

// コライダ―を生成する
void CFieldBase::CreateCol(std::string fieldCol)
{
	// 空じゃなければ
	if (fieldCol != "")
	{
		CModel* model = CResourceManager::Get<CModel>(fieldCol);

		// 物理用データの取得
		mMeshVertices = model->GetPhysicsVertexPositions();
		mMeshIndices = model->GetPhysicsIndices();

		// 剛体の作成
		CPhysicsManager::Instance()->CreateMeshRigidBody(
			this,                                   // owner
			mMeshVertices.data(),                   // 頂点データ
			(int)mMeshVertices.size() / 3,          // 頂点数
			mMeshIndices.data(),                    // インデックスデータ
			(int)mMeshIndices.size() / 3,           // 三角形数
			Position(),                             // 初期位置
			ELayer::eField,
			{ ELayer::ePlayer,ELayer::eConnectObj, ELayer::eObject }
		);
	}
}
