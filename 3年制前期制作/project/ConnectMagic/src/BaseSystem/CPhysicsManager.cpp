#include "CPhysicsManager.h"
#include "CTaskManager.h"
#include "CPhysicsDebugDraw.h"
#include "CPlayer.h"
#include "btBulletDynamicsCommon.h"
#include "CollisionData.h"

// 最大分割数
constexpr int MAX_SUB_STEPS =				 10;
// 重力
constexpr float PHYSICS_GRAVITY =			-98.0f;
// 接触距離の許容範囲
constexpr float CONTACT_DISTANCE_THRESHOLD = 0.0f;

// インスタンス
CPhysicsManager* CPhysicsManager::spInstance = nullptr;

// インスタンスを取得
CPhysicsManager* CPhysicsManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CPhysicsManager();
	}
	
	return spInstance;
}

// コンストラクタ
CPhysicsManager::CPhysicsManager()
	: CTask(ETaskPriority::eNone, 0, ETaskPauseType::eGame)
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
	mpDynamicsWorld->setGravity(btVector3(0.0f, PHYSICS_GRAVITY, 0.0f));

	mpDebugDraw = new CPhysicsDebugDraw();
	mpDynamicsWorld->setDebugDrawer(mpDebugDraw);
}

// デストラクタ
CPhysicsManager::~CPhysicsManager()
{
	// ワールド内のすべての剛体
	if (mpDynamicsWorld != nullptr)
	{
		for (int i = mpDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = mpDynamicsWorld->getCollisionObjectArray()[i];

			// ワールドから削除
			mpDynamicsWorld->removeCollisionObject(obj);
		}
	}

	SAFE_DELETE(mpDynamicsWorld);
	SAFE_DELETE(mpSolver);
	SAFE_DELETE(mpBroadPhase);
	SAFE_DELETE(mpDispatcher);
	SAFE_DELETE(mpCollisionConfiguration);
	SAFE_DELETE(mpDebugDraw);

	if (spInstance != nullptr)
	{
		spInstance = nullptr;
	}
}

// 剛体をワールドから削除
void CPhysicsManager::RemoveRigidBody(btRigidBody* body)
{
	mpDynamicsWorld->removeRigidBody(body);
}

void CPhysicsManager::RemoveSensor(btCollisionObject* sensor)
{
	// リストから削除
	auto it = std::find(mSensorList.begin(), mSensorList.end(), sensor);
	if (it != mSensorList.end())
	{
		mSensorList.erase(it);
	}

	// 物理世界から削除
	mpDynamicsWorld->removeCollisionObject(sensor);
}

// 更新
void CPhysicsManager::Update()
{
	mpDynamicsWorld->stepSimulation(Times::DeltaTime(), MAX_SUB_STEPS);

	// 衝突データを更新
	UpdateCollisionDataList();

	// センサーの座標を更新
	UpdateSensorPos();
}

// 後更新
void CPhysicsManager::LateUpdate()
{
	// タスクマネージャーから、すべての3dタスクを取得する
	const std::list<CTask*>& all3dTasks = CTaskManager::Instance()->Get3dTasks();

	// 剛体を持つオブジェクトを探して同期処理を行う
	for (CTask* task : all3dTasks)
	{
		CObjectBase* obj = dynamic_cast<CObjectBase*>(task);

		// オブジェクトでなければ次へ
		if (obj == nullptr)
		{
			continue;
		}

		// objがBulletの剛体を持っているか
		btRigidBody* rigidBody = obj->GetRigidBody();
		if (rigidBody != nullptr && rigidBody->getMotionState())
		{
			// 最新の位置と回転をBulletから取得
			btTransform btTrans;
			rigidBody->getMotionState()->getWorldTransform(btTrans);

			// 位置
			btVector3 btPos = btTrans.getOrigin();
			CVector pos = CVector(btPos.getX(), btPos.getY(), btPos.getZ());
			pos.Y(pos.Y() - obj->GetHalfHeight());

			// 回転
			btQuaternion btRot = btTrans.getRotation();
			// 逆回転に設定
			btRot = btRot.inverse();
			CQuaternion rot = CQuaternion(btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW());

			// 位置を設定
			obj->Position(pos);
			// 回転を設定
			obj->Rotation(rot);
		}
	}
}

// 描画
void CPhysicsManager::Render()
{
	if (mpDebugDraw != nullptr && mpDynamicsWorld != nullptr)
	{
		// デバッグモードを設定
		mpDebugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints);
		mpDynamicsWorld->debugDrawWorld();
	}
}

void CPhysicsManager::UpdateCollisionDataList()
{
	// 衝突リストをクリア
	mCollisionDataList.clear();

	// 接触情報を取得
	btDispatcher* dispatcher = mpDynamicsWorld->getDispatcher();
	int numManifolds = dispatcher->getNumManifolds();

	for (int i = 0; i < numManifolds; i++)
	{
		// 2つの剛体間の接触を保持する構造体
		btPersistentManifold* contactManifold = dispatcher->getManifoldByIndexInternal(i);

		const btCollisionObject* objA = contactManifold->getBody0();
		const btCollisionObject* objB = contactManifold->getBody1();

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			// 接触距離が許容範囲内の場合
			if (pt.getDistance() <= CONTACT_DISTANCE_THRESHOLD)
			{
				// A → BのCollisionDataを作成
				mCollisionDataList.push_back(
					CreateCollisionData(objA, objB, pt)
				);

				// B → AのCollisionDataを作成
				mCollisionDataList.push_back(
					CreateCollisionData(objB, objA, pt)
				);
			}
		}
	}
}

std::list<CollisionData> CPhysicsManager::GetCollisionDataList() const
{
	return mCollisionDataList;
}

CollisionData CPhysicsManager::CreateCollisionData(const btCollisionObject* objA, const btCollisionObject* objB, const btManifoldPoint& pt)
{
	CollisionData data;
	// 自身の剛体
	data.selfBody = (btCollisionObject*)objA;
	// 相手の剛体
	data.otherBody = (btCollisionObject*)objB;
	// 法線ベクトル
	data.contactNormal = pt.m_normalWorldOnB;

	return data;
}

btRigidBody* CPhysicsManager::CreateBoxRigidBody(
	CObjectBase* owner, 
	float mass, 
	const CVector& halfExtents, 
	const CVector& initialPos, 
	const CQuaternion& initialRot)
{
	// 衝突形状の作成
	btCollisionShape* shape = new btBoxShape(
		btVector3(halfExtents.X(), halfExtents.Y(), halfExtents.Z()));

	// 初期トランスフォーム
	btTransform startTrans;
	// 回転
	startTrans.setRotation(
		btQuaternion(initialRot.X(), initialRot.Y(), initialRot.Z(), initialRot.W()));
	// 座標
	startTrans.setOrigin(
		btVector3(initialPos.X(), initialPos.Y(), initialPos.Z()));

	// 慣性の計算
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (mass != 0.0f)
	{
		// 質量がある場合のみ慣性を計算
		shape->calculateLocalInertia(mass, localInertia);
	}

	// モーションステートの作成
	btDefaultMotionState* motionState = new btDefaultMotionState(startTrans);

	// 剛体の作成
	btRigidBody::btRigidBodyConstructionInfo
		rbInfo(mass, motionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// 剛体の設定
	// 質量0の場合、静的オブジェクトフラグを設定
	if (mass == 0.0f)
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	}

	// XZ軸の回転をロック
	if (mass > 0.0f)
	{
		body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	}

	// CObjectBaseに設定
	owner->SetRigidBody(body, shape, motionState, halfExtents.Y());

	// ワールドに追加
	mpDynamicsWorld->addRigidBody(body);

	// オブジェクトにユーザポインタを設定
	body->setUserPointer(owner);

	return body;
}

btRigidBody* CPhysicsManager::CreateCapsuleRigidBody(
	CObjectBase* owner,
	float mass, 
	float radius, 
	float height, 
	const CVector& initPos, 
	const CQuaternion& initRot)
{
	// 衝突形状の作成
	btCollisionShape* shape = new btCapsuleShape(radius, height);

	// 初期トランスフォーム
	btTransform startTrans;
	// 回転
	startTrans.setRotation(
		btQuaternion(initRot.X(), initRot.Y(), initRot.Z(), initRot.W()));
	// 座標
	startTrans.setOrigin(
		btVector3(initPos.X(), initPos.Y(), initPos.Z()));

	// 慣性の計算
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (mass > 0.0f)
	{
		shape->calculateLocalInertia(mass, localInertia);
	}

	// モーションステートの作成
	btDefaultMotionState* motionState = new btDefaultMotionState(startTrans);

	// 剛体の作成
	btRigidBody::btRigidBodyConstructionInfo
		rbInfo(mass, motionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// 剛体の設定
	if (mass == 0.0f)
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	}

	// XZ軸の回転をロック
	if (mass > 0.0f)
	{
		body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	}

	float halfHeight = radius + height / 2;
	// CObjectBaseに設定
	owner->SetRigidBody(body, shape, motionState, halfHeight);

	// ワールドに追加
	mpDynamicsWorld->addRigidBody(body);

	// オブジェクトにユーザポインタを設定
	body->setUserPointer(owner);

	return body;
}

btRigidBody* CPhysicsManager::CreateMeshRigidBody(
	CObjectBase* owner, 
	const float* vertexArray, 
	int numVertices, 
	const int* indexArray, 
	int numTriangles, 
	const CVector& initialPos)
{
	// Bulletのメッシュ構造体を作成
	btIndexedMesh mesh;
	mesh.m_numTriangles = numTriangles;
	mesh.m_triangleIndexBase = (const unsigned char*)indexArray;
	mesh.m_triangleIndexStride = 3 * sizeof(int); // 3つのインデックスで1三角形
	mesh.m_numVertices = numVertices;
	mesh.m_vertexBase = (const unsigned char*)vertexArray;
	mesh.m_vertexStride = sizeof(float) * 3; // 頂点は (X, Y, Z) の float 3つ

	btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray();
	indexVertexArray->addIndexedMesh(mesh);

	// BVH (Bounding Volume Hierarchy) 構造を持つ衝突形状の作成
	btCollisionShape* shape = new btBvhTriangleMeshShape(
		indexVertexArray,
		true // 高速化
	);

	// 静的オブジェクトに設定
	float mass = 0.0f; // 静的剛体
	btVector3 localInertia(0.0f, 0.0f, 0.0f);

	// トランスフォームの設定
	btTransform startTrans;
	startTrans.setIdentity();
	startTrans.setOrigin(btVector3(initialPos.X(), initialPos.Y(), initialPos.Z()));

	// モーションステートの作成
	btDefaultMotionState* motionState = new btDefaultMotionState(startTrans);
	// 剛体の作成
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// 静的オブジェクトのフラグ設定
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);

	// 剛体の設定
	owner->SetRigidBody(body, shape, motionState, 0.0f, indexVertexArray);
	// ワールドへの追加
	mpDynamicsWorld->addRigidBody(body);

	// オブジェクトにユーザポインタを設定
	body->setUserPointer(owner);

	return body;
}

btCollisionObject* CPhysicsManager::CreateSensor(CObjectBase* owner, float radius)
{
	btCollisionObject* colObj = new btCollisionObject();
	// 形を球に設定
	colObj->setCollisionShape(new btSphereShape(radius));
	// 押し戻しをしないフラグ設定
	colObj->setCollisionFlags(colObj->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	// オブジェクトにユーザポインタを設定
	colObj->setUserPointer(owner);

	// センサーの設定
	owner->SetSensor(colObj);
	// ワールド追加
	mpDynamicsWorld->addCollisionObject(colObj);

	// リストに追加
	mSensorList.push_back(colObj);

	return colObj;
}

void CPhysicsManager::UpdateSensorPos()
{
	for (auto* sensor : mSensorList)
	{
		// 親のオブジェクト
		CObjectBase* parentObj = static_cast<CObjectBase*>(sensor->getUserPointer());

		if (parentObj != nullptr)
		{
			// 親の剛体の位置と同期
			btTransform trans = parentObj->GetRigidBody()->getWorldTransform();
			sensor->setWorldTransform(trans);
		}
	}
}
