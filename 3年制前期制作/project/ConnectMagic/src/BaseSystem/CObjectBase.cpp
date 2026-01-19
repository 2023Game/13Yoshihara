#include "CObjectBase.h"
#include "Maths.h"
#include "CPhysicsManager.h"
#include "btBulletDynamicsCommon.h"
#include "CollisionData.h"
#include "CColor.h"

// コンストラクタ
CObjectBase::CObjectBase(ETag tag,
	ETaskPriority prio, int sortOrder,
	ETaskPauseType pause,
	bool dontDelete, bool addTaskList)
	: CTask(prio, sortOrder, pause, dontDelete, addTaskList)
	, mTag(tag)
	, mIsEnableCol(true)
	, mDepth(0.0f)
	, mColor(CColor::white)
	, mIsDamage(false)
	, mpRigidBody(nullptr)
	, mpCollisionShape(nullptr)
	, mpMotionState(nullptr)
	, mHalfHeightY(0.0f)
	, mpIndexVertexArray(nullptr)
	, mpSensorCol(nullptr)
	, mBodyLayer(ELayer::eNone)
	, mBodyCollisionLayers({ ELayer::eNone })
	, mSensorLayer(ELayer::eNone)
	, mSensorCollisionLayers({ ELayer::eNone })
{
}

// デストラクタ
CObjectBase::~CObjectBase()
{
	// 物理管理クラスを取得
	CPhysicsManager* physicsMgr = CPhysicsManager::Instance();

	if (physicsMgr != nullptr)
	{
		// 物理ワールドから剛体を取り除く
		if (mpRigidBody != nullptr)
		{
			physicsMgr->RemoveRigidBody(mpRigidBody);
		}

		// 物理ワールドからセンサーを取り除く
		if (mpSensorCol != nullptr)
		{
			physicsMgr->RemoveSensor(mpSensorCol);
		}
	}

	// 関連するBulletオブジェクトを解放
	SAFE_DELETE(mpRigidBody);
	SAFE_DELETE(mpCollisionShape);
	SAFE_DELETE(mpMotionState);
	SAFE_DELETE(mpIndexVertexArray);
	SAFE_DELETE(mpSensorCol);
}

// オブジェクト削除を伝える関数
void CObjectBase::DeleteObject(CObjectBase* obj)
{
}

// オブジェクトタグを取得
ETag CObjectBase::Tag() const
{
	return mTag;
}

// オブジェクトタグを設定
void CObjectBase::SetTag(ETag tag)
{
	mTag = tag;
}

void CObjectBase::SetEnable(bool enable)
{
	CTask::SetEnable(enable);
	SetEnableCol(enable);
}

// 衝突判定を行うか設定
void CObjectBase::SetEnableCol(bool isEnable)
{
	mIsEnableCol = isEnable;
	if (!mpRigidBody) return;

	if (mIsEnableCol) {
		// 有効化
		// フラグを動的に戻す
		// 押し返しを有効化
		int flags = mpRigidBody->getCollisionFlags();
		flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
		flags &= ~btCollisionObject::CF_NO_CONTACT_RESPONSE;
		mpRigidBody->setCollisionFlags(flags);

		// 重力計算を復活させる
		mpRigidBody->setActivationState(ACTIVE_TAG);

		// 強制的に起こす
		mpRigidBody->activate(true);
	}
	else {
		// 無効化
		// Kinematicに設定
		// 押し返しを無効化
		int flags = mpRigidBody->getCollisionFlags();
		flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
		flags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
		mpRigidBody->setCollisionFlags(flags);

		// 速度を完全に殺す
		mpRigidBody->setLinearVelocity(btVector3(0, 0, 0));
		mpRigidBody->setAngularVelocity(btVector3(0, 0, 0));
		mpRigidBody->clearForces();
	}
}

// 衝突判定を行うかどうか
bool CObjectBase::IsEnableCol() const
{
	return mIsEnableCol && IsEnable();
}

// カラーを設定
void CObjectBase::SetColor(const CColor& color)
{
	mColor = color;
}

// カラーを取得
const CColor& CObjectBase::GetColor() const
{
	return mColor;
}

// アルファ値設定
void CObjectBase::SetAlpha(float alpha)
{
	mColor.A(Math::Clamp01(alpha));
}

// アルファ値取得
float CObjectBase::GetAlpha() const
{
	return mColor.A();
}

// カメラまでの距離を計算
void CObjectBase::CalcDepth()
{
	CCamera* camera = CCamera::CurrentCamera();
	if (camera == nullptr) return;

	CVector camPos = camera->Position();
	CVector pos = Position();

	mDepth = (camPos - pos).LengthSqr();
}

// カメラからの距離を取得
float CObjectBase::GetDepth() const
{
	return mDepth;
}

// レイとオブジェクトの衝突判定
bool CObjectBase::CollisionRay(const CVector& start, const CVector& end)
{
	return false;
}

// 攻撃中か
bool CObjectBase::IsAttacking() const
{
	return false;
}

// 攻撃開始
void CObjectBase::AttackStart()
{
	// 攻撃がヒットしたオブジェクトのリストを初期化
	mAttackHitObjects.clear();
}

// 攻撃終了
void CObjectBase::AttackEnd()
{
}

// 攻撃を受けているかどうか
bool CObjectBase::IsDamaging() const
{
	return mIsDamage;
}

// 剛体を取得
btRigidBody* CObjectBase::GetRigidBody() const
{
	return mpRigidBody;
}

// 剛体を設定
void CObjectBase::SetRigidBody(btRigidBody* body,
	btCollisionShape* shape,
	btDefaultMotionState* ms,
	float halfHeight,
	btTriangleIndexVertexArray* indexVertexArray)
{
	mpRigidBody = body;
	mpCollisionShape = shape;
	mpMotionState = ms;
	SetHalfHeight(halfHeight);
	mpIndexVertexArray = indexVertexArray;
}

btCollisionObject* CObjectBase::GetSensor() const
{
	return mpSensorCol;
}

void CObjectBase::SetSensor(btCollisionObject* sensor)
{
	mpSensorCol = sensor;
}

// 力を加える（加速する動き）
void CObjectBase::AddForce(const CVector& force)
{
	// 動的剛体である
	if (mpRigidBody && mpRigidBody->getInvMass() > 0.0f)
	{
		// 剛体を強制的にアクティブにする
		if (mpRigidBody->getActivationState() == ISLAND_SLEEPING)
		{
			// ACTIVATE状態に設定
			mpRigidBody->activate(true);
		}
		mpRigidBody->applyCentralForce(btVector3(force.X(), force.Y(), force.Z()));
	}
}

// インパルスを加える（瞬発的な動き）
void CObjectBase::AddImpulse(const CVector& impulse)
{
	// 動的剛体である
	if (mpRigidBody && mpRigidBody->getInvMass() > 0.0f)
	{
		// 剛体を強制的にアクティブにする
		if (mpRigidBody->getActivationState() == ISLAND_SLEEPING)
		{
			// ACTIVATE状態に設定
			mpRigidBody->activate(true);
		}
		mpRigidBody->applyCentralImpulse(btVector3(impulse.X(), impulse.Y(), impulse.Z()));
	}
}

void CObjectBase::ResetForce()
{
	if (mpRigidBody)
	{
		// 加わっている力と回転力をゼロにする
		mpRigidBody->clearForces();
		// 現在の移動速度をゼロにする
		mpRigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		// 現在の回転速度をゼロにする
		mpRigidBody->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
	}
}

void CObjectBase::SetBodyPos(const CVector& pos)
{
	// 剛体を取得
	btRigidBody* body = GetRigidBody();
	// 剛体がないなら処理しない
	if (!body) return;

	// トランスフォームを取得
	btTransform trans = body->getWorldTransform();
	// 座標設定
	trans.setOrigin(btVector3(pos.X(), pos.Y() + mHalfHeightY, pos.Z()));

	// 物理ワールドに伝える
	body->getMotionState()->setWorldTransform(trans);
	body->setWorldTransform(trans);
}

void CObjectBase::SetBodyRot(const CQuaternion& rot)
{
	// 剛体を取得
	btRigidBody* body = GetRigidBody();
	// 剛体がないなら処理しない
	if (!body) return;

	// トランスフォームを取得
	btTransform trans = body->getWorldTransform();
	// 回転を設定
	trans.setRotation(btQuaternion(rot.X(), rot.Y(), rot.Z(), rot.W()));

	// 物理ワールドに伝える
	body->getMotionState()->setWorldTransform(trans);
	body->setWorldTransform(trans);
}

void CObjectBase::SaveBodyLayer(ELayer myLayer, Layers collisionLayers)
{
	mBodyLayer = myLayer;
	mBodyCollisionLayers = collisionLayers;
}

void CObjectBase::SaveSensorLayer(ELayer myLayer, Layers collisionLayers)
{
	mSensorLayer = myLayer;
	mSensorCollisionLayers = collisionLayers;
}

void CObjectBase::Position(const CVector& pos)
{
	btRigidBody* body = GetRigidBody();
	if (body)
	{
		SetBodyPos(pos);
	}
	else
	{
		CTransform::Position(pos);
	}
}

void CObjectBase::Rotation(const CQuaternion& rot)
{
	btRigidBody* body = GetRigidBody();
	if (body)
	{
		SetBodyRot(rot);
	}
	else
	{
		CTransform::Rotation(rot);
	}
}

// 剛体の半分の高さを取得
float CObjectBase::GetHalfHeight() const
{
	return mHalfHeightY;
}

// 剛体の半分の高さを設定
void CObjectBase::SetHalfHeight(float halfHeight)
{
	mHalfHeightY = halfHeight;
}

void CObjectBase::OnCollision(const CollisionData& data)
{
}

void CObjectBase::OnSensorEnter(const CollisionData& data)
{
}

// すべて削除
void CObjectBase::Delete()
{
	Kill();
}

// コライダーを作成
void CObjectBase::CreateCol()
{
}

// 攻撃がヒットしたオブジェクトを追加
void CObjectBase::AddAttackHitObj(CObjectBase* obj)
{
	mAttackHitObjects.push_back(obj);
}

// 既に攻撃がヒットしているオブジェクトかどうか
bool CObjectBase::IsAttackHitObj(CObjectBase* obj) const
{
	auto find = std::find
	(
		mAttackHitObjects.begin(),
		mAttackHitObjects.end(),
		obj
	);
	return find != mAttackHitObjects.end();
}