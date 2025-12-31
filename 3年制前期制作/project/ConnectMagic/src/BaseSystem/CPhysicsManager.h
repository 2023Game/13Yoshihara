#pragma once
#include "CollisionLayer.h"

class CTask;
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

class CPhysicsManager : public CTask
{
public:
	// インスタンスを取得
	static CPhysicsManager* Instance();

	// コンストラクタ
	CPhysicsManager();
	// デストラクタ
	~CPhysicsManager();

	// 剛体をワールドから削除
	void RemoveRigidBody(btRigidBody* body);
	// センサーをリストから外し、ワールドから削除
	void RemoveSensor(btCollisionObject* sensor);

	// 更新
	void Update() override;
	// 後更新
	void LateUpdate() override;

#if _DEBUG
	// 描画
	void Render() override;
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
	/// <param name="owner"></param>
	/// <param name="halfExtents"></param>
	/// <param name="myLayer"></param>
	/// <param name="collisionLayers"></param>
	/// <returns></returns>
	btCollisionObject* CreateBoxSensor(
		CObjectBase* owner,
		const CVector& halfExtents,
		ELayer myLayer,
		Layers collisionLayers
	);

	/// <summary>
	/// 検知用の球状センサーを生成、ワールドへ追加
	/// </summary>
	/// <param name="owner">持ち主</param>
	/// <param name="radius">半径</param>
	/// <param name="myLayer">自分の所属レイヤー</param>
	/// <param name="collisionLayers">衝突判定をするレイヤー</param>
	/// <returns></returns>
	btCollisionObject* CreateSphereSensor(
		CObjectBase* owner,
		float radius,
		ELayer myLayer,
		Layers collisionLayers
	);

	// レイキャスト
	bool Raycast(const CVector& start, const CVector& end, 
		CVector* hitPos, Layers collisionLayers);

	// 摩擦を設定
	void SetFriction(btRigidBody* body, float friction);
	// 反発係数を設定
	void SetRestitution(btRigidBody* body, float restitution);
	// 減衰を設定
	void SetDamping(btRigidBody* body, float linDamping, float angDamping);

	// ELayerをビットフラグに変換
	int ToBit(ELayer layer);
	// 複数のレイヤーをまとめてビットマスクにする
	int ToMask(Layers layers);
	// ワールドを取得
	btDiscreteDynamicsWorld* GetDynamicsWorld() const;
	// ディスパッチャーを取得
	btCollisionDispatcher* GetDispatcher() const;
	// 
private:
	// インスタンス
	static CPhysicsManager* spInstance;

	// btVector3をCVectorにする
	CVector ToCVector(const btVector3& vec);
	// CVectorをbtVector3にする
	btVector3 ToBtVector(const CVector& vec);
	// btQuaternionをCQuaternionにする
	CQuaternion ToCQuaternion(const btQuaternion& rot);
	// CQuaternionをbtQuaternionにする
	btQuaternion ToBtQuaternion(const CQuaternion& rot);

	// 衝突データを更新
	void UpdateCollisionData();

	// センサーの座標更新
	void UpdateSensorPos();
	// センサーのリスト
	std::list<btCollisionObject*> mSensorList;

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