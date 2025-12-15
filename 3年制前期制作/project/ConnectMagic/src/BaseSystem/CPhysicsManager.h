#pragma once
#include <btBulletDynamicsCommon.h>

class CPhysicsManager {
public:
	// コンストラクタ
	CPhysicsManager();
	// デストラクタ
	~CPhysicsManager();

	// メインループから呼ばれ、シミュレーションを進める
	void StepSimulation(float deltaTime);

	// 剛体を作成し、ワールドに追加する
	btRigidBody* CreateRigidBody(float mass, 
		const btTransform& startTransform,
		btCollisionShape* shape);

private:
	btDefaultCollisionConfiguration* mpCollisionConfiguration;
	btCollisionDispatcher* mpDispatcher;
	btBroadphaseInterface* mpBroadPhase;
	btSequentialImpulseConstraintSolver* mpSolver;
	btDiscreteDynamicsWorld* mpDynamicsWorld;
};