#pragma once
#include "CollisionLayer.h"

class CPhysicsDebugDraw;
struct CollisionData;
struct PhysicsMaterial;
// Bullet関連
class btCollisionObject;
class btRigidBody;
class btManifoldPoint;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btVector3;
class btQuaternion;
class btTransform;
class btPoint2PointConstraint;
class btTypedConstraint;

class CPhysicsManager
{
public:
	// インスタンスを取得
	static CPhysicsManager* Instance();
	// インスタンスを破棄
	static void ClearInstance();

	// コンストラクタ
	CPhysicsManager();
	// デストラクタ
	~CPhysicsManager();

	// 剛体をワールドから削除
	void RemoveRigidBody(btRigidBody* body);
	// センサーをリストから外し、ワールドから削除
	void RemoveSensor(btCollisionObject* sensor);

	// 更新
	void Update();
	// 後更新
	void LateUpdate();

#if _DEBUG
	// 描画
	void Render();
#endif

	/// <summary>
	/// ボックス剛体を生成、ワールドへ追加
	/// </summary>
	/// <param name="owner">持ち主</param>
	/// <param name="material">物理設定</param>
	/// <param name="halfExtents">ボックスの半分の範囲</param>
	/// <param name="initialPos">初期位置</param>
	/// <param name="initialRot">初期回転</param>
	/// <param name="myLayer">自分の所属レイヤー</param>
	/// <param name="collisionLayers">衝突判定をするレイヤー</param>
	/// <returns></returns>
	btRigidBody* CreateBoxRigidBody(
		CObjectBase* owner,				
		const PhysicsMaterial& material,				
		const CVector& halfExtents,		
		const CVector& initPos,		
		const CQuaternion& initRot,
		ELayer myLayer,
		Layers collisionLayers
	);

	/// <summary>
	/// 球体剛体を生成、ワールドへ追加
	/// </summary>
	/// <param name="owner">持ち主</param>
	/// <param name="material">物理設定</param>
	/// <param name="radius">半径</param>
	/// <param name="initPos">初期位置</param>
	/// <param name="myLayer">自分の所属レイヤー</param>
	/// <param name="collisionLayers">衝突判定をするレイヤー</param>
	/// <returns></returns>
	btRigidBody* CreateSphereRigidBody(
		CObjectBase* owner,
		const PhysicsMaterial& material,
		float radius,
		const CVector& initPos,
		ELayer myLayer,
		Layers collisionLayers
	);

	/// <summary>
	/// カプセル剛体を生成、ワールドへ追加
	/// </summary>
	/// <param name="owner">持ち主</param>
	/// <param name="material">物理設定</param>
	/// <param name="radius">半径</param>
	/// <param name="height">円柱部分の高さ</param>
	/// <param name="initPos">初期位置</param>
	/// <param name="initRot">初期回転</param>
	/// <param name="myLayer">自分の所属レイヤー</param>
	/// <param name="collisionLayers">衝突判定をするレイヤー</param>
	/// <returns></returns>
	btRigidBody* CreateCapsuleRigidBody(
		CObjectBase* owner,
		const PhysicsMaterial& material,
		float radius,
		float height,
		const CVector& initPos,
		const CQuaternion& initRot,
		ELayer myLayer,
		Layers collisionLayers
	);

	/// <summary>
	/// メッシュ剛体を生成、ワールドへ追加
	/// </summary>
	/// <param name="owner">持ち主</param>
	/// <param name="vertexArray">頂点座標の配列</param>
	/// <param name="numVertices">頂点数</param>
	/// <param name="indexArray">インデックスの配列</param>
	/// <param name="numTriangles">三角形の数</param>
	/// <param name="initialPos">初期位置</param>
	/// <param name="myLayer">自分の所属レイヤー</param>
	/// <param name="collisionLayers">衝突判定をするレイヤー</param>
	/// <returns></returns>
	btRigidBody* CreateMeshRigidBody(
		CObjectBase* owner,			
		const float* vertexArray,
		int numVertices,			
		const int* indexArray,
		int numTriangles,			
		const CVector& initPos,
		ELayer myLayer,
		Layers collisionLayers
	);

	/// <summary>
	/// 検知用のボックスセンサーを生成、ワールドへ追加
	/// </summary>
	/// <param name="owner">持ち主</param>
	/// <param name="halfExtents">半分の大きさ</param>
	/// <param name="myLayer">自分の所属レイヤー</param>
	/// <param name="collisionLayers">衝突判定をするレイヤー</param>
	/// <param name="isUpdatePos">座標を更新するか</param>
	/// <returns></returns>
	btCollisionObject* CreateBoxSensor(
		CObjectBase* owner,
		const CVector& halfExtents,
		ELayer myLayer,
		Layers collisionLayers,
		bool isUpdatePos = true
	);

	/// <summary>
	/// 検知用の球状センサーを生成、ワールドへ追加
	/// </summary>
	/// <param name="owner">持ち主</param>
	/// <param name="radius">半径</param>
	/// <param name="myLayer">自分の所属レイヤー</param>
	/// <param name="collisionLayers">衝突判定をするレイヤー</param>
	/// <param name="isUpdatePos">座標を更新するか</param>
	/// <returns></returns>
	btCollisionObject* CreateSphereSensor(
		CObjectBase* owner,
		float radius,
		ELayer myLayer,
		Layers collisionLayers,
		bool isUpdatePos = true
	);

	// レイキャスト
	bool Raycast(const CVector& start, const CVector& end, 
		CollisionData* collisionData, Layers collisionLayers);
	// スフィアキャスト
	bool SphereCast(const CVector& start, const CVector& end,
		float radius, CollisionData* collisionData, Layers collisionLayers);

	// 支点と剛体を繋ぐジョイントを作成
	btPoint2PointConstraint* CreateJoint(
		btRigidBody* myBody,
		btRigidBody* otherBody,
		const CVector& myPos,
		const CVector& anchorPos);
	// ジョイントを削除
	void RemoveJoint(btTypedConstraint* joint, btRigidBody* body);
	// ジョイントをすべて削除
	void RemoveAllJoint();

	// 摩擦を設定
	void SetFriction(btRigidBody* body, float friction);
	// 反発係数を設定
	void SetRestitution(btRigidBody* body, float restitution);
	// 減衰を設定
	void SetDamping(btRigidBody* body, float linDamping, float angDamping);
	// センサーの座標を設定
	void SetSensorPos(btCollisionObject* sensor, const CVector& pos);

	// ELayerをビットフラグに変換
	int ToBit(ELayer layer);
	// 複数のレイヤーをまとめてビットマスクにする
	int ToMask(Layers layers);

	// btVector3をCVectorにする
	CVector ToCVector(const btVector3& vec);
	// CVectorをbtVector3にする
	btVector3 ToBtVector(const CVector& vec);
	// btQuaternionをCQuaternionにする
	CQuaternion ToCQuaternion(const btQuaternion& rot);
	// CQuaternionをbtQuaternionにする
	btQuaternion ToBtQuaternion(const CQuaternion& rot);
	// 座標と回転をbtTransformにする
	btTransform ToBtTransform(const CVector& pos, const CQuaternion& rot);
	// 座標をbtTransformにする
	btTransform ToBtTransform(const CVector& pos);

	// ワールドを取得
	btDiscreteDynamicsWorld* GetDynamicsWorld() const;
	// ディスパッチャーを取得
	btCollisionDispatcher* GetDispatcher() const; 

private:
	// インスタンス
	static CPhysicsManager* spInstance;

	// 衝突データを更新
	void UpdateCollisionData();

	// センサーの座標更新
	void UpdateSensorPos();
	// センサーのリスト
	std::list<btCollisionObject*> mSensorList;

	// ジョイントのリスト
	std::list<btTypedConstraint*> mJointList;

	// デバッグ描画
	CPhysicsDebugDraw* mpDebugDraw;

	// 衝突の設定
	btDefaultCollisionConfiguration* mpCollisionConfiguration;
	// 衝突の判定
	btCollisionDispatcher* mpDispatcher;
	// 衝突の可能性があるペアを絞り込む
	btBroadphaseInterface* mpBroadPhase;
	// 衝突したオブジェクトの挙動の計算
	btSequentialImpulseConstraintSolver* mpSolver;
	// 物理世界
	btDiscreteDynamicsWorld* mpDynamicsWorld;

	// コライダーを表示するか
	bool mIsShowCollider;
};