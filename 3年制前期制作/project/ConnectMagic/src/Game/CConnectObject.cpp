#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "CModel.h"
#include "CConnectPointManager.h"
#include "CPlayer.h"

#define THRESHOLD 0.1f

// 移動前後のレイが衝突したときのマージン
#define MARGIN 50.0f

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
	, mConnectObjTag(EConnectObjTag::eBox)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
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
	mMoveSpeed = CVector::zero;

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
void CConnectObject::Connect(CVector wandPointPos, CVector targetPointPos)
{
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// カメラの方向
	CVector cameraDir = -CCamera::CurrentCamera()->VectorZ();
	// オブジェクトからターゲットポイントへのベクトル
	CVector vec = targetPointPos - Position();
	// 新しい座標を求める
	CVector newPos = CPlayer::Instance()->Position() + cameraDir * pointMgr->GetConnectDistance();
	// 今のままだとターゲットポイントとの座標の差分ずれるので
	// 差を消す
	newPos = newPos - vec;
	newPos.Y(Position().Y());
	// 線形補間で、いきなりワープしないようにする
	newPos = CVector::Lerp(Position(), newPos, 0.1f);
	Position(newPos);
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
