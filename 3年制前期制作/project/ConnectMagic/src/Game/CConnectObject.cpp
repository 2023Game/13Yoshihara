#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "CModel.h"
#include "CConnectPointManager.h"
#include "CPlayer.h"
#include "CConnectPoint.h"
#include "CWand.h"
#include "CollisionLayer.h"
#include "CPhysicsManager.h"
#include "btBulletDynamicsCommon.h"

// 加速度の強さ
constexpr float ACCELERATION_FORCE = 10000.0f;

// コンストラクタ
CConnectObject::CConnectObject(float weight, ETaskPriority prio,
	int sortOrder,ETaskPauseType pause)
	: CObjectBase(ETag::eConnectObject, prio, 0, pause)
	, mWeight(weight)
	, mpModel(nullptr)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mConnectObjTag(EConnectObjTag::eWeight)
	, mIsGrounded(false)
	, mPreOtherPointPos(CVector::zero)
	, mIsMove(true)
	, mpTarget(nullptr)
	, mpJoint(nullptr)
{
}

// デストラクタ
CConnectObject::~CConnectObject()
{
}

// 更新
void CConnectObject::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	mIsGrounded = false;
}

// 描画
void CConnectObject::Render()
{
	if (mpModel != nullptr)
		mpModel->Render(Matrix());
}

// 繋がったときの処理
void CConnectObject::Connect(CConnectPoint* otherPoint, bool isWand)
{
	// 接続部管理クラス
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();

	// 空中か
	bool otherIsAir = false;
	// 杖じゃないなら
	if (!isWand)
	{
		CConnectObject* otherObj = otherPoint->GetConnectObj();
		// 相手が空中OBJならtrue
		if (otherObj->GetConnectObjTag() == EConnectObjTag::eAir)
		{
			otherIsAir = true;
		}
	}

	if (isWand && mIsMove)
	{
		// カメラの方向
		CVector cameraDir = -CCamera::CurrentCamera()->VectorZ();
		// 新しい座標を求める
		CVector newPos = CPlayer::Instance()->Position() + cameraDir * pointMgr->GetConnectDistance();
		// 移動方向を求める
		CVector moveVec = newPos - Position();
		moveVec.Normalize();
		// 力を加える
		AddForce(moveVec * ACCELERATION_FORCE);
	}
}

// 繋げた瞬間の処理
void CConnectObject::JustConnect(CConnectPoint* otherPoint)
{
	if (!otherPoint) return;

	mPreOtherPointPos = otherPoint->Position();
	// 相手が空中オブジェクトならジョイントを作成
	CConnectObject* otherObj = otherPoint->GetConnectObj();
	if (!otherObj) return;
	if (otherObj->GetConnectObjTag() == EConnectObjTag::eAir)
	{
		CreateJoint(otherObj);
	}
}

// 接続解除の処理
void CConnectObject::Disconnect(CConnectPoint* otherPoint)
{
	DeleteJoint();
}

// 接続ターゲットの作成
void CConnectObject::CreateTarget(CVector pos)
{
	// ターゲット生成
	mpTarget = new CConnectTarget(this);
	// 親子設定
	mpTarget->SetParent(this);
	// 位置設定
	mpTarget->Position(Position() + pos);
}

// 接続ターゲットを取得
CConnectTarget* CConnectObject::GetTarget() const
{
	return mpTarget;
}

// 重さを取得
float CConnectObject::GetWeight()
{
	return mWeight;
}

// 接続オブジェクトのタグを設定
void CConnectObject::SetConnectObjTag(EConnectObjTag tag)
{
	mConnectObjTag = tag;
}

// 接続オブジェクトのタグを取得
EConnectObjTag CConnectObject::GetConnectObjTag()
{
	return mConnectObjTag;
}

// 接続したときに移動の処理をするかを設定
void CConnectObject::SetMove(bool enable)
{
	mIsMove = enable;
}

void CConnectObject::CreateJoint(CConnectObject* otherObj)
{
	// 空じゃなければ処理しない
	if (mpJoint != nullptr) return;

	// 接続部管理クラス
	CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
	// 物理管理クラス
	CPhysicsManager* physicsMgr = CPhysicsManager::Instance();
	// 接続線の長さを設定
	connectPointMgr->SetConnectDistance();
	// 相手の剛体
	btRigidBody* targetObjBody = otherObj->GetRigidBody();
	// 相手の座標
	CVector targetPos = otherObj->Position();
	// ジョイントを作成
	mpJoint = physicsMgr->CreateJoint(GetRigidBody(), targetObjBody, Position(), targetPos);
	// XZ軸回転を無効
	GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
}

void CConnectObject::DeleteJoint()
{
	// 既に空なら処理しない
	if (mpJoint == nullptr) return;

	// 物理管理クラス
	CPhysicsManager* physicsMgr = CPhysicsManager::Instance();
	// ジョイントを削除
	physicsMgr->RemoveJoint(mpJoint, GetRigidBody());
	mpJoint = nullptr;
}
