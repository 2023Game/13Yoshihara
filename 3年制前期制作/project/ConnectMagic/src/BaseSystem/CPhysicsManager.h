#pragma once

class CTask;
class CPhysicsDebugDraw;
struct CollisionData;
// Bullet関連
class btCollisionObject;
class btRigidBody;
class btManifoldPoint;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

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
	// 描画
	void Render() override;

	// 衝突データを取得する
	std::list<CollisionData> GetCollisionDataList() const;

	/// <summary>
	/// ボックス剛体を生成、ワールドへ追加
	/// </summary>
	/// <param name="owner">持ち主</param>
	/// <param name="mass">質量</param>
	/// <param name="halfExtents">ボックスの半分の範囲</param>
	/// <param name="initialPos">初期位置</param>
	/// <param name="initialRot">初期回転</param>
	/// <returns></returns>
	btRigidBody* CreateBoxRigidBody(
		CObjectBase* owner,				
		float mass,				
		const CVector& halfExtents,		
		const CVector& initPos,		
		const CQuaternion& initRot
	);

	/// <summary>
	/// カプセル剛体を生成、ワールドへ追加
	/// </summary>
	/// <param name="owner">持ち主</param>
	/// <param name="mass">質量</param>
	/// <param name="radius">半径</param>
	/// <param name="height">円柱部分の高さ</param>
	/// <param name="initPos">初期位置</param>
	/// <param name="initRot">初期回転</param>
	/// <returns></returns>
	btRigidBody* CreateCapsuleRigidBody(
		CObjectBase* owner,
		float mass,
		float radius,
		float height,
		const CVector& initPos,
		const CQuaternion& initRot
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
	/// <returns></returns>
	btRigidBody* CreateMeshRigidBody(
		CObjectBase* owner,			
		const float* vertexArray,
		int numVertices,			
		const int* indexArray,
		int numTriangles,			
		const CVector& initPos	
	);

	/// <summary>
	/// 検知用のセンサーを生成、ワールドへ追加
	/// </summary>
	/// <param name="owner">持ち主</param>
	/// <param name="radius">半径</param>
	/// <returns></returns>
	btCollisionObject* CreateSensor(
		CObjectBase* owner,
		float radius
	);

private:
	// インスタンス
	static CPhysicsManager* spInstance;

	// 衝突データを更新
	void UpdateCollisionDataList();
	// 衝突データ作成
	CollisionData CreateCollisionData(
		const btCollisionObject* objA,
		const btCollisionObject* objB,
		const btManifoldPoint& pt);
	// 衝突データのリスト
	std::list<CollisionData> mCollisionDataList;

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
};