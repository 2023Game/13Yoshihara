#include "CRotateFloor.h"
#include "Maths.h"
#include "CPhysicsManager.h"
#include "PhysicsMaterial.h"

constexpr float ROTATE_Y =	1.0f;

// 物理設定
constexpr float MASS =		0.0f;

CRotateFloor::CRotateFloor(CModel* model, const CVector& pos, const CVector& scale, float rotateSpeedY)
	: mpModel(model)
	, mRotateSpeedY(rotateSpeedY)
{
	Position(pos);
	Scale(scale);

	CreateCol();
}

CRotateFloor::~CRotateFloor()
{
}

void CRotateFloor::Update()
{
	Rotate(0.0f, mRotateSpeedY, 0.0f);
}

void CRotateFloor::Render()
{
	mpModel->Render(Matrix());
}

void CRotateFloor::CreateCol()
{
	PhysicsMaterial material;
	material.mass = MASS;

	// 物理用データの取得
	mMeshVertices = mpModel->GetPhysicsVertexPositions();
	mMeshIndices = mpModel->GetPhysicsIndices();

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
