#include "CPhysicsManager.h"
#include "CTaskManager.h"
#include "CPhysicsDebugDraw.h"
#include "CPlayer.h"
#include "btBulletDynamicsCommon.h"
#include "CollisionData.h"
#include "PhysicsMaterial.h"
#include "CDebugInput.h"

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
	, mIsShowCollider(false)
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
			obj->CTransform::Position(pos);
			// 回転を設定
			obj->CTransform::Rotation(rot);
		}
	}
}

#if _DEBUG
// 描画
void CPhysicsManager::Render()
{
	// 「SHIFT」+「9」でコライダー表示機能オンオフ
	if (CDebugInput::Key(VK_SHIFT) && CDebugInput::PushKey('9'))
	{
		mIsShowCollider = !mIsShowCollider;
	}
	// コライダー表示フラグがオフなら、以降処理しない
	if (!mIsShowCollider) return;

	if (mpDebugDraw != nullptr && mpDynamicsWorld != nullptr)
	{
		// デバッグモードを設定
		mpDebugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints);
		mpDynamicsWorld->debugDrawWorld();
	}
}
#endif

int CPhysicsManager::ToBit(ELayer layer)
{
	if (layer == ELayer::eNone) return 0;
	return 1 << static_cast<int>(layer);
}

int CPhysicsManager::ToMask(Layers layers)
{
	int mask = 0;
	for (ELayer layer : layers)
	{
		mask |= ToBit(layer);
	}
	return mask;
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
	const PhysicsMaterial& material,
	const CVector& halfExtents, 
	const CVector& initialPos, 
	const CQuaternion& initialRot,
	ELayer myLayer,
	Layers collisionLayers)
{
	// 衝突形状の作成
	btCollisionShape* shape = new btBoxShape(
		btVector3(halfExtents.X(), halfExtents.Y(), halfExtents.Z()));

	// 初期トランスフォーム
	btTransform startTrans;
	// 回転
	startTrans.setRotation(
		btQuaternion(initialRot.X(), initialRot.Y(), initialRot.Z(), initialRot.W()));
	// 座標(高さの半分上に位置調整)
	startTrans.setOrigin(
		btVector3(initialPos.X(), initialPos.Y() + halfExtents.Y(), initialPos.Z()));

	// 慣性の計算
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (material.mass != 0.0f)
	{
		// 質量がある場合のみ慣性を計算
		shape->calculateLocalInertia(material.mass, localInertia);
	}

	// モーションステートの作成
	btDefaultMotionState* motionState = new btDefaultMotionState(startTrans);

	// 剛体の作成
	btRigidBody::btRigidBodyConstructionInfo
		rbInfo(material.mass, motionState, shape, localInertia);
	// 摩擦を設定
	rbInfo.m_friction = material.friction;
	// 反発係数を設定
	rbInfo.m_restitution = material.restitution;
	btRigidBody* body = new btRigidBody(rbInfo);

	// 減衰設定
	body->setDamping(material.linDamping, material.angDamping);

	// 剛体の設定
	// 質量0の場合、静的オブジェクトフラグを設定
	if (material.mass == 0.0f)
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	}

	// XZ軸の回転をロック
	if (material.mass > 0.0f)
	{
		body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	}

	// CObjectBaseに設定
	owner->SetRigidBody(body, shape, motionState, halfExtents.Y());

	// レイヤーをビットへ変換
	int group = ToBit(myLayer);
	int mask = ToMask(collisionLayers);

	// ワールド追加(衝突相手を設定)
	mpDynamicsWorld->addRigidBody(body, group, mask);

	// オブジェクトにユーザポインタを設定
	body->setUserPointer(owner);

	return body;
}

btRigidBody* CPhysicsManager::CreateSphereRigidBody(
	CObjectBase* owner,
	const PhysicsMaterial& material,
	float radius, 
	const CVector& initPos,
	ELayer myLayer,
	Layers collisionLayers)
{
	// 衝突形状の作成
	btCollisionShape* shape = new btSphereShape(radius);

	// 初期トランスフォーム
	btTransform startTrans;
	startTrans.setIdentity();
	// 座標(半径分上に位置調整)
	startTrans.setOrigin(
		btVector3(initPos.X(), initPos.Y() + radius, initPos.Z()));

	// 慣性の計算
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (material.mass > 0.0f)
	{
		shape->calculateLocalInertia(material.mass, localInertia);
	}

	// モーションステートの作成
	btDefaultMotionState* motionState = new btDefaultMotionState(startTrans);

	// 剛体の作成
	btRigidBody::btRigidBodyConstructionInfo
		rbInfo(material.mass, motionState, shape, localInertia);
	// 摩擦を設定
	rbInfo.m_friction = material.friction;
	// 反発係数を設定
	rbInfo.m_restitution = material.restitution;
	btRigidBody* body = new btRigidBody(rbInfo);

	// 減衰設定
	body->setDamping(material.linDamping, material.angDamping);

	// 剛体の設定
	if (material.mass == 0.0f)
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	}

	// XZ軸の回転をロック
	if (material.mass > 0.0f)
	{
		body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	}

	// CObjectBaseに設定
	owner->SetRigidBody(body, shape, motionState, radius);

	// レイヤーをビットへ変換
	int group = ToBit(myLayer);
	int mask = ToMask(collisionLayers);

	// ワールド追加(衝突相手を設定)
	mpDynamicsWorld->addRigidBody(body, group, mask);

	// オブジェクトにユーザポインタを設定
	body->setUserPointer(owner);

	return body;
}

btRigidBody* CPhysicsManager::CreateCapsuleRigidBody(
	CObjectBase* owner,
	const PhysicsMaterial& material,
	float radius, 
	float height, 
	const CVector& initPos, 
	const CQuaternion& initRot,
	ELayer myLayer,
	Layers collisionLayers)
{
	// 衝突形状の作成
	btCollisionShape* shape = new btCapsuleShape(radius, height);

	// 高さの半分
	float halfHeight = radius + height / 2;
	// 初期トランスフォーム
	btTransform startTrans;
	// 回転
	startTrans.setRotation(
		btQuaternion(initRot.X(), initRot.Y(), initRot.Z(), initRot.W()));
	// 座標(高さの半分上に位置調整)
	startTrans.setOrigin(
		btVector3(initPos.X(), initPos.Y() + halfHeight, initPos.Z()));

	// 慣性の計算
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (material.mass > 0.0f)
	{
		shape->calculateLocalInertia(material.mass, localInertia);
	}

	// モーションステートの作成
	btDefaultMotionState* motionState = new btDefaultMotionState(startTrans);

	// 剛体の作成
	btRigidBody::btRigidBodyConstructionInfo
		rbInfo(material.mass, motionState, shape, localInertia);
	// 摩擦を設定
	rbInfo.m_friction = material.friction;
	// 反発係数を設定
	rbInfo.m_restitution = material.restitution;
	btRigidBody* body = new btRigidBody(rbInfo);

	// 減衰設定
	body->setDamping(material.linDamping, material.angDamping);

	// 剛体の設定
	if (material.mass == 0.0f)
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	}

	// XZ軸の回転をロック
	if (material.mass > 0.0f)
	{
		body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	}

	// CObjectBaseに設定
	owner->SetRigidBody(body, shape, motionState, halfHeight);

	// レイヤーをビットへ変換
	int group = ToBit(myLayer);
	int mask = ToMask(collisionLayers);

	// ワールド追加(衝突相手を設定)
	mpDynamicsWorld->addRigidBody(body, group, mask);

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
	const CVector& initialPos,
	ELayer myLayer,
	Layers collisionLayers)
{
	btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray(
		numTriangles,
		(int*)indexArray,
		3 * sizeof(int),	// 3つのインデックスで1三角形
		numVertices,
		(btScalar*)vertexArray,
		3 * sizeof(float)	// 頂点は (X, Y, Z) の float 3つ
	);

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

	// レイヤーをビットへ変換
	int group = ToBit(myLayer);
	int mask = ToMask(collisionLayers);

	// ワールド追加(衝突相手を設定)
	mpDynamicsWorld->addRigidBody(body, group, mask);

	// オブジェクトにユーザポインタを設定
	body->setUserPointer(owner);

	return body;
}

btCollisionObject* CPhysicsManager::CreateBoxSensor(
	CObjectBase* owner,
	const CVector& halfExtents, 
	ELayer myLayer, 
	Layers collisionLayers)
{
	btCollisionObject* colObj = new btCollisionObject();
	// 形をボックスに設定
	colObj->setCollisionShape(new btBoxShape(btVector3(halfExtents.X(), halfExtents.Y(), halfExtents.Z())));
	// 押し戻しをしないフラグ設定
	colObj->setUserPointer(owner);
	// スリープさせない
	colObj->setActivationState(DISABLE_DEACTIVATION);

	// センサーの設定
	owner->SetSensor(colObj);

	// レイヤーをビットへ変換
	int group = ToBit(myLayer);
	int mask = ToMask(collisionLayers);

	// ワールド追加（衝突相手を設定）
	mpDynamicsWorld->addCollisionObject(colObj, group, mask);

	// リストに追加
	mSensorList.push_back(colObj);

	return colObj;
}

btCollisionObject* CPhysicsManager::CreateSphereSensor(
	CObjectBase* owner,
	float radius,
	ELayer myLayer,
	Layers collisionLayers)
{
	btCollisionObject* colObj = new btCollisionObject();
	// 形を球に設定
	colObj->setCollisionShape(new btSphereShape(radius));
	// 押し戻しをしないフラグ設定
	colObj->setCollisionFlags(colObj->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	// オブジェクトにユーザポインタを設定
	colObj->setUserPointer(owner);
	// スリープさせない
	colObj->setActivationState(DISABLE_DEACTIVATION);

	// センサーの設定
	owner->SetSensor(colObj);

	// レイヤーをビットへ変換
	int group = ToBit(myLayer);
	int mask = ToMask(collisionLayers);

	// ワールド追加(衝突相手を設定)
	mpDynamicsWorld->addCollisionObject(colObj, group, mask);

	// リストに追加
	mSensorList.push_back(colObj);

	return colObj;
}

void CPhysicsManager::SetFriction(btRigidBody* body, float friction)
{
	if (body)
	{
		// 摩擦を設定
		body->setFriction(friction);
	}
}

void CPhysicsManager::SetRestitution(btRigidBody* body, float restitution)
{
	if (body)
	{
		// 反発係数を設定
		body->setRestitution(restitution);
	}
}

void CPhysicsManager::SetDamping(btRigidBody* body, float linDamping, float angDamping)
{
	if (body)
	{
		// 減衰を設定
		body->setDamping(linDamping, angDamping);
	}
}

void CPhysicsManager::UpdateSensorPos()
{
	for (auto* sensor : mSensorList)
	{
		// 親のオブジェクト
		CObjectBase* parentObj = static_cast<CObjectBase*>(sensor->getUserPointer());

		if (parentObj != nullptr)
		{
			// 親の位置と同期
			btTransform trans = sensor->getWorldTransform();
			CVector pos = parentObj->Position();
			trans.setOrigin(btVector3(pos.X(), pos.Y(), pos.Z()));

			sensor->setWorldTransform(trans);
		}
	}
}
