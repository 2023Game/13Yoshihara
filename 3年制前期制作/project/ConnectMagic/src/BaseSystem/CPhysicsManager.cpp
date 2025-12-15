#include "CPhysicsManager.h"

// コンストラクタ
CPhysicsManager::CPhysicsManager()
{
	// 基礎コンポーネントの作成
	mpCollisionConfiguration = new btDefaultCollisionConfiguration();
	mpDispatcher = new btCollisionDispatcher(mpCollisionConfiguration);
	mpBroadPhase = new btDbvtBroadphase();
	mpSolver = new btSequentialImpulseConstraintSolver;

	// 物理ワールドの作成
	mpDynamicsWorld = new btDiscreteDynamicsWorld(mpDispatcher, mpBroadPhase, 
		mpSolver, mpCollisionConfiguration);
	
	// 重力の設定
	mpDynamicsWorld->setGravity(btVector3(0.0f, GRAVITY, 0.0f));

	// 地面となる静的剛体を作成し、ワールドに追加
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0.0f);
	btTransform groundTransform;
	groundTransform.setIdentity();
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo 
		groundRbInfo(0.0f, groundMotionState, groundShape, btVector3(0.0f, 0.0f, 0.0f));
	btRigidBody* groundBody = new btRigidBody(groundRbInfo);
	mpDynamicsWorld->addRigidBody(groundBody);
}

// デストラクタ
CPhysicsManager::~CPhysicsManager()
{
}

// メインループから呼ばれ、シミュレーションを進める
void CPhysicsManager::StepSimulation(float deltaTime)
{
	mpDynamicsWorld->stepSimulation(deltaTime, 10.0f);
}

// 剛体を作成し、ワールドに追加する
btRigidBody* CPhysicsManager::CreateRigidBody(float mass, 
	const btTransform& startTransform, btCollisionShape* shape)
{
	// 慣性の計算
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (mass != 0.0f)
	{
		shape->calculateLocalInertia(mass, localInertia);
	}

	// モーションステート
	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);

	// 剛体の構築とワールドへの追加
	btRigidBody::btRigidBodyConstructionInfo
		rbInfo(mass, motionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	mpDynamicsWorld->addRigidBody(body);

	return body;
}
