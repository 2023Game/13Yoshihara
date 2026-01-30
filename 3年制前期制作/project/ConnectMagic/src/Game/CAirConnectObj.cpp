#include "CAirConnectObj.h"
#include "CInput.h"
#include "CPhysicsManager.h"
#include "PhysicsMaterial.h"

// 接続ターゲットの座標
const CVector TARGET_POS =	CVector(2.5f, 0.0f, 0.0f);

// 空中の接続オブジェクトの重さ
constexpr float WEIGHT =	1.0f;

// コライダーの半径
constexpr float RADIUS =	5.0f;

// 物理設定
constexpr float MASS = 0.0f;
const CVector HALF_EXTENTS = CVector(5.0f, 5.0f, 5.0f);
constexpr float FRICTION = 0.1f;	// 摩擦（値が高いと停止まで早くなる）
constexpr float LIN_DAMPING = 0.8f;	// 線形減衰(値が高いと滑りが小さくなる)
constexpr float ANG_DAMPING = 0.9f;	// 角減衰(値が高いと微細な回転振動を吸収する）

// コンストラクタ
CAirConnectObj::CAirConnectObj(const CVector& pos)
	: CConnectObject(WEIGHT)
{
	Position(pos);

	// 空中の接続オブジェクトのタグを設定
	SetConnectObjTag(EConnectObjTag::eAir);

	mpModel = CResourceManager::Get<CModel>("AirConnectObj");

	// コライダーを生成
	CreateCol();

	// 接続ターゲットを生成
	CreateTarget(TARGET_POS);
}

// デストラクタ
CAirConnectObj::~CAirConnectObj()
{
}

// つながった時の処理
void CAirConnectObj::Connect(CConnectPoint* otherPoint, bool isWand)
{
}

// コライダーを生成
void CAirConnectObj::CreateCol()
{
	PhysicsMaterial physicsMaterial;
	physicsMaterial.mass = MASS;
	physicsMaterial.friction = FRICTION;
	physicsMaterial.linDamping = LIN_DAMPING;
	physicsMaterial.angDamping = ANG_DAMPING;

	CPhysicsManager* physicsMgr = CPhysicsManager::Instance();
	physicsMgr->CreateBoxRigidBody(
		this,
		physicsMaterial,
		HALF_EXTENTS,
		Position(),
		Rotation(),
		ELayer::eConnectObj,
		{ ELayer::eConnectSearch,ELayer::eConnectObj,ELayer::eObject,
		ELayer::ePlayer }
	);
}
