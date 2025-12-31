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
	if (!body) return;

	mpDynamicsWorld->removeRigidBody(body);
}

void CPhysicsManager::RemoveSensor(btCollisionObject* sensor)
{
	if (!sensor) return;

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
	UpdateCollisionData();

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
			CVector pos = CVector(ToCVector(btPos));
			pos.Y(pos.Y() - obj->GetHalfHeight());

			// 回転
			btQuaternion btRot = btTrans.getRotation();
			// 逆回転に設定
			btRot = btRot.inverse();
			CQuaternion rot = CQuaternion(ToCQuaternion(btRot));

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

btCollisionDispatcher* CPhysicsManager::GetDispatcher() const
{
	return mpDispatcher;
}

btDiscreteDynamicsWorld* CPhysicsManager::GetDynamicsWorld() const
{
	return mpDynamicsWorld;
}

CVector CPhysicsManager::ToCVector(const btVector3& vec)
{
	return CVector(vec.x(), vec.y(), vec.z());
}

btVector3 CPhysicsManager::ToBtVector(const CVector& vec)
{
	return btVector3(vec.X(), vec.Y(), vec.Z());
}

CQuaternion CPhysicsManager::ToCQuaternion(const btQuaternion& rot)
{
	return CQuaternion(rot.x(), rot.y(), rot.z(), rot.w());
}

btQuaternion CPhysicsManager::ToBtQuaternion(const CQuaternion& rot)
{
	return btQuaternion(rot.X(), rot.Y(), rot.Z(), rot.W());
}

void CPhysicsManager::UpdateCollisionData()
{
	// 接触情報を取得
	btDispatcher* dispatcher = mpDynamicsWorld->getDispatcher();
	int numManifolds = dispatcher->getNumManifolds();

	for (int i = 0; i < numManifolds; i++)
	{
		// 2つの剛体間の接触を保持する構造体
		btPersistentManifold* contactManifold = dispatcher->getManifoldByIndexInternal(i);

		// ユーザポインタからオブジェクトを取得
		const btCollisionObject* objA = contactManifold->getBody0();
		const btCollisionObject* objB = contactManifold->getBody1();

		CObjectBase* ownerA = static_cast<CObjectBase*>(objA->getUserPointer());
		CObjectBase* ownerB = static_cast<CObjectBase*>(objB->getUserPointer());

		// どちらかがNULLなら判定不能なので次へ
		if (!ownerA || !ownerB) continue;
		// 衝突判定を行わないなら次へ
		if (!ownerA->IsEnableCol() || !ownerB->IsEnableCol()) continue;

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			// 接触距離が許容範囲内の場合
			if (pt.getDistance() <= CONTACT_DISTANCE_THRESHOLD)
			{
				// CollisionDataの作成
				CollisionData dataA;
				dataA.selfBody = (btCollisionObject*)objA;
				dataA.otherBody = (btCollisionObject*)objB;
				dataA.otherObj = ownerB;
				dataA.hitPoint = ToCVector(pt.getPositionWorldOnA());
				dataA.contactNormal = ToCVector(pt.m_normalWorldOnB);

				CollisionData dataB;
				dataB.selfBody = (btCollisionObject*)objB;
				dataB.otherBody = (btCollisionObject*)objA;
				dataB.otherObj = ownerA;
				dataB.hitPoint = ToCVector(pt.getPositionWorldOnB());
				dataB.contactNormal = ToCVector(-pt.m_normalWorldOnB);

				// センサーかどうかの判定
				bool isSensorA = objA->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE;
				bool isSensorB = objB->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE;

				// 通知の実行
				if (isSensorA) {
					ownerA->OnSensorEnter(dataA); // AがセンサーならAに通知
				}
				else {
					ownerA->OnCollision(dataA);   // Aが剛体ならAに通知
				}

				if (isSensorB) {
					ownerB->OnSensorEnter(dataB); // BがセンサーならBに通知
				}
				else {
					ownerB->OnCollision(dataB);   // Bが剛体ならBに通知
				}

				break;
			}
		}
	}
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
	owner->SaveBodyLayer(myLayer, collisionLayers);

	// 衝突形状の作成
	btCollisionShape* shape = new btBoxShape(
		btVector3(ToBtVector(halfExtents)));

	// 初期トランスフォーム
	btTransform startTrans;
	// 回転
	startTrans.setRotation(
		btQuaternion(ToBtQuaternion(initialRot)));
	// 座標(高さの半分上に位置調整)
	startTrans.setOrigin(
		btVector3(ToBtVector(initialPos+CVector(0.0f,halfExtents.Y(),0.0f))));

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
	owner->SaveBodyLayer(myLayer, collisionLayers);

	// 衝突形状の作成
	btCollisionShape* shape = new btSphereShape(radius);

	// 初期トランスフォーム
	btTransform startTrans;
	startTrans.setIdentity();
	// 座標(半径分上に位置調整)
	startTrans.setOrigin(
		btVector3(ToBtVector(initPos + CVector(0.0f, radius, 0.0f))));

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
	owner->SaveBodyLayer(myLayer, collisionLayers);

	// 衝突形状の作成
	btCollisionShape* shape = new btCapsuleShape(radius, height);

	// 高さの半分
	float halfHeight = radius + height / 2;
	// 初期トランスフォーム
	btTransform startTrans;
	// 回転
	startTrans.setRotation(
		btQuaternion(ToBtQuaternion(initRot)));
	// 座標(高さの半分上に位置調整)
	startTrans.setOrigin(
		btVector3(ToBtVector(initPos + CVector(0.0f, halfHeight, 0.0f))));

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
	owner->SaveBodyLayer(myLayer, collisionLayers);

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
	startTrans.setOrigin(ToBtVector(initialPos));

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
	owner->SaveSensorLayer(myLayer, collisionLayers);

	btCollisionObject* colObj = new btCollisionObject();
	// 形をボックスに設定
	colObj->setCollisionShape(new btBoxShape(ToBtVector(halfExtents)));
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
	owner->SaveSensorLayer(myLayer, collisionLayers);

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

bool CPhysicsManager::Raycast(const CVector& start, const CVector& end, 
	CVector* hitPos,
	Layers collisionLayers)
{
	btVector3 btStart = ToBtVector(start);
	btVector3 btEnd = ToBtVector(end);

	btCollisionWorld::ClosestRayResultCallback rayCallback(btStart, btEnd);
	
	// すべてのグループから発射
	rayCallback.m_collisionFilterGroup = -1; 
	// 衝突相手を設定
	int mask = ToMask(collisionLayers);
	rayCallback.m_collisionFilterMask = mask; 

	// レイテスト実行
	mpDynamicsWorld->rayTest(btStart, btEnd, rayCallback);

	if (rayCallback.hasHit()) {
		// ポインタがNULLでなければ、当たった座標を設定
		if (hitPos) {
			*hitPos = ToCVector(rayCallback.m_hitPointWorld);
		}
		// 当たった
		return true;
	}

	// 当たらなかった
	return false;
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
