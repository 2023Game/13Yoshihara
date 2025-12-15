#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "CModel.h"
#include "CConnectPointManager.h"
#include "CPlayer.h"
#include "CConnectPoint.h"
#include "CWand.h"

constexpr float THRESHOLD =			0.1f;

// 移動前後のレイが衝突したときのマージン
constexpr float MARGIN =			50.0f;

// 減速する速度
constexpr float DECREASE_SPEED =	1.0f;
// 加速する速度
constexpr float INCREASE_SPEED =	0.5f;

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
{
}

// デストラクタ
CConnectObject::~CConnectObject()
{
	// 接続部管理クラス
	auto* pointMgr = CConnectPointManager::Instance();

	// 全てのターゲットを削除
	for (int i = 0; i < mTargets.size(); i++)
	{
		// 接続されていれば解除
		pointMgr->DisableConnect(mTargets[i]);
		mTargets[i]->SetConnectObj(nullptr);
		mTargets[i]->Kill();
	}
	
	// 配列を空にする
	mTargets.clear();
	SAFE_DELETE(mpCol);
}

// オブジェクト削除を伝える関数
void CConnectObject::DeleteObject(CObjectBase* obj)
{
	int num = mTargets.size();
	for (int i = 0; i < num; i++)
	{
		// 一致したら
		if (mTargets[i] == obj)
		{
			// 配列から取り除く
			mTargets.erase(mTargets.begin() + i);
			return;
		}
	}
}

// 更新
void CConnectObject::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	if (mIsGravity)
	{
		mMoveSpeedY -= GRAVITY;
	}
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	if (!mIsConnectAir)
	{
		mMoveSpeed = CVector::zero;
	}

	// 移動
	Position(Position() + moveSpeed);

	mIsGrounded = false;
}

// 描画
void CConnectObject::Render()
{
	if (mpModel != nullptr)
		mpModel->Render(Matrix());
}

// 衝突処理
void CConnectObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpCol)
	{
		// 衝突した相手が地面の場合
		if (other->Layer() == ELayer::eGround)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);

			// 衝突した地面が床か天井かを内積で判定
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// 内積の結果がプラスであれば、床と衝突した
			if (dot >= 0.0f)
			{
				// 落下などで床に上から衝突したとき（下移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
				// 接地
				mIsGrounded = true;
			}
			// 内積の結果がマイナスであれば、天井と衝突した
			else if (dot < 0.0f)
			{
				// 天井にしたから衝突したとき（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// 衝突した相手が壁の場合
		else if (other->Layer() == ELayer::eWall)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
		}
		// 衝突した相手がオブジェクトの場合
		else if (other->Layer() == ELayer::eObject)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);

			// 衝突した面が上か下かを内積で判定
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// 内積の結果がプラスであれば、上面と衝突した
			if (dot >= THRESHOLD)
			{
				// 落下などで上から衝突したとき（下移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
			// 内積の結果がマイナスであれば、下面と衝突した
			else if (dot < 0.0f)
			{
				// ジャンプなどで下から衝突したとき（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
	}
}

// 繋がったときの処理
void CConnectObject::Connect(CConnectPoint* otherPoint, bool isWand)
{
	// 接続部管理クラス
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();

	// 空中か
	bool isAir = false;
	// 杖じゃないなら
	if (!isWand)
	{
		CConnectObject* otherObj = otherPoint->GetConnectObj();
		// 相手が空中OBJならtrue
		if (otherObj->GetConnectObjTag() == EConnectObjTag::eAir)
		{
			isAir = true;
		}
	}

	if (isAir)
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
		// オブジェクトからターゲットポイントへのベクトル
		//CVector vec = selfPos - Position();
		// 新しい座標を求める
		CVector newPos = CPlayer::Instance()->Position() + cameraDir * pointMgr->GetConnectDistance();
		// 今のままだとターゲットポイントとの座標の差分ずれるので
		// 差を消す
		//newPos = newPos - vec;
		newPos.Y(Position().Y());
		// 線形補間で、いきなりワープしないようにする
		newPos = CVector::Lerp(Position(), newPos, 0.1f);
		Position(newPos);
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
	CConnectTarget* target = new CConnectTarget(this);
	// 親子設定
	target->SetParent(this);
	// 位置設定
	target->Position(pos);
	// リストに追加
	mTargets.push_back(target);
}

// 接続ターゲットを取得
std::vector<CConnectTarget*> CConnectObject::GetTargets()
{
	return mTargets;
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
