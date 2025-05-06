#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "CModel.h"

// 重力
#define GRAVITY 0.0625f

// コンストラクタ
CConnectObject::CConnectObject(float weight, ETaskPriority prio,
	int sortOrder,ETaskPauseType pause)
	: CObjectBase(ETag::eConnectObject, prio, 0, pause)
	, mWeight(weight)
	, mpCol(nullptr)
	, mpModel(nullptr)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
{
}

// デストラクタ
CConnectObject::~CConnectObject()
{
}

// オブジェクト削除を伝える関数
void CConnectObject::DeleteObject(CObjectBase* obj)
{
}

// 更新
void CConnectObject::Update()
{
	mMoveSpeedY -= GRAVITY /10 * Times::DeltaTime();
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	mMoveSpeed = CVector::zero;

	// 移動
	Position(Position() + moveSpeed);
}

// 描画
void CConnectObject::Render()
{
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
		// 衝突した相手が壁、オブジェクト、プレイヤーの
		// いずれかの場合
		else if (other->Layer() == ELayer::eWall)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
		}
	}
}

// 引っ張られた時の処理
void CConnectObject::Pull(CVector pullDir, float opponentWeight)
{
	// 自身が動かない重さの場合処理しない
	if (mWeight == 1.0f) return;
	// 移動速度
	CVector moveSpeed = CVector::zero;
	// 相手が動かない重さなら
	if (opponentWeight == 1.0f)
	{
		// 引っ張る力はそのまま
		moveSpeed = pullDir * PULL_POWER * Times::DeltaTime();
	}
	// それ以外なら
	else
	{
		// 自分と相手の重さの総量
		float totalWeight = mWeight + opponentWeight;
		// 総量に対する相手の重さで引っ張る力を決める
		moveSpeed = pullDir * (opponentWeight / totalWeight * PULL_POWER) * Times::DeltaTime();
	}

	// 移動速度を設定する
	mMoveSpeedY = moveSpeed.Y();
	moveSpeed.Y(0.0f);
	mMoveSpeed = moveSpeed;
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
