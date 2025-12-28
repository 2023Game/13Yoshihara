#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "CModel.h"
#include "CConnectPointManager.h"
#include "CPlayer.h"
#include "CConnectPoint.h"
#include "CWand.h"
#include "CollisionLayer.h"
#include "CCollider.h"
#include "btBulletDynamicsCommon.h"

constexpr float THRESHOLD =			0.1f;

// 移動前後のレイが衝突したときのマージン
constexpr float MARGIN =			50.0f;

// 減速する速度
constexpr float DECREASE_SPEED =	1.0f;
// 加速する速度
constexpr float INCREASE_SPEED =	0.5f;
// 加速度の強さ
constexpr float ACCELERATION_FORCE = 100.0f;

// コンストラクタ
CConnectObject::CConnectObject(float weight, ETaskPriority prio,
	int sortOrder,ETaskPauseType pause)
	: CObjectBase(ETag::eConnectObject, prio, 0, pause)
	, mWeight(weight)
	, mpCol(nullptr)
	, mpModel(nullptr)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mIsGravity(true)
	, mConnectObjTag(EConnectObjTag::eWeight)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
	, mPreOtherPointPos(CVector::zero)
	, mIsConnectAir(false)
	, mIsMove(true)
	, mpTarget(nullptr)
{
}

// デストラクタ
CConnectObject::~CConnectObject()
{
	SAFE_DELETE(mpCol);
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

	if (otherIsAir)
	{
		CConnectObject* otherObj = otherPoint->GetConnectObj();
		CVector selfPos = Position();
		CVector otherPos = otherObj->Position();

		mIsConnectAir = true;
		// 相手から自身への方向
		CVector dir = selfPos - otherPos;
		dir.Normalize();

		// 重力の方向
		CVector gravity = CVector(0.0f, -GRAVITY, 0.0f);
		// 線を引っ張る重力を除外
		gravity = gravity - dir * gravity.Dot(dir);

		// 移動
		CVector moveDir = otherPos - mPreOtherPointPos;
		float moveDist = moveDir.Length();

		if (moveDist > 0.0001f)
		{
			// 移動を線の垂直面に投影
			moveDir = dir.Cross(moveDir.Cross(dir));
			moveDir.Normalize();

			// 移動速度
			mMoveSpeed += moveDir * INCREASE_SPEED * Times::DeltaTime();
		}
		// 重力を加える
		// 振り切る時の減速と戻る時の加速のため
		mMoveSpeed += gravity;

		// 少しずつ減速していく
		mMoveSpeed *= (1.0f - DECREASE_SPEED * Times::DeltaTime());

		// 線方向の速度を削除
		// ターゲットの真下での急な減速を防ぐため
		mMoveSpeed -= dir * mMoveSpeed.Dot(dir);
		// 座標に追加
		selfPos += mMoveSpeed;

		// 新しいプレイヤー座標への方向
		dir = selfPos - otherPos;
		dir.Normalize();
		// プレイヤーの座標を線から一定の距離に保つ
		selfPos = otherPos +
			dir * pointMgr->GetConnectDistance();

		// 座標を設定
		Position(selfPos);

		// 相手の接続部の座標を前回座標に保存
		mPreOtherPointPos = otherPos;
	}
	else if (isWand && mIsMove)
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
void CConnectObject::JustConnect(CVector otherPointPos)
{
	// 重りなら重力オフ
	if (mConnectObjTag == EConnectObjTag::eWeight)
	{
		SetGravity(false);
	}
	mPreOtherPointPos = otherPointPos;
}

// 接続解除の処理
void CConnectObject::Disconnect()
{
	if (mConnectObjTag == EConnectObjTag::eWeight)
	{
		SetGravity(true);
		mIsConnectAir = false;
	}
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

// 重力を掛けるかを設定
void CConnectObject::SetGravity(bool isGravity)
{
	mIsGravity = isGravity;
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
