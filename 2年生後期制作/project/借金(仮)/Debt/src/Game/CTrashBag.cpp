#include "CTrashBag.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CNavNode.h"
#include "CTrashPlayer.h"
#include "CTrashEnemy.h"

// 基本の重力
#define GRAVITY 0.0625f

// 本体コライダ―
#define BODY_RADIUS 2.5f	// 半径
// オフセット座標
#define BODY_COL_OFFSET_POS CVector(0.0f,1.2f,0.0f)

#define SCALE 2.0f

// コンストラクタ
CTrashBag::CTrashBag(bool gold)
	: CObjectBase(ETag::eTrashBag, ETaskPriority::eTrashBag, 0, ETaskPauseType::eGame)
	, CTrashBagStatus(gold)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mpRideObject(nullptr)
	, mGroundNormal(CVector::zero)
	, mIsGrounded(false)
	, mIsGravity(true)
	, mpAttachMtx(nullptr)
	, mpOwner(nullptr)
{
	Scale(SCALE, SCALE, SCALE);

	SetGold(gold);

	// 本体コライダ―
	mpBodyCol = new CColliderSphere
	(
		this, ELayer::eTrashBag,
		BODY_RADIUS
	);
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// プレイヤー、敵、回収員、地形、ゴミ袋、ゴミ袋探知用、車両
	// と衝突判定する
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,ETag::eField,
		ETag::eTrashBag,ETag::eVehicle });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,ELayer::eCollector,
		ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::eTrashBag,ELayer::eTrashBagSearch,ELayer::eVehicle });

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);
}

// デストラクタ
CTrashBag::~CTrashBag()
{
	SAFE_DELETE(mpBodyCol);
}

// ゴミ袋の持ち主を設定
void CTrashBag::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ゴミ袋の持ち主を取得
CObjectBase* CTrashBag::GetOwner() const
{
	return mpOwner;
}

// 更新
void CTrashBag::Update()
{
	// 重力を掛けるなら
	if (mIsGravity)
	{
		mMoveSpeedY -= GRAVITY;
	}

		CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

		// 移動
		Position(Position() + moveSpeed);

	// 地面についているなら
	if (mIsGrounded)
	{
		// 消滅までをカウント
		CountDeleteTime();
		// 移動をゼロにする
		mMoveSpeed = CVector::zero;
	}

	// 消滅までの時間が経ったら
	if (IsElapsedDeleteTime())
	{
		SetOnOff(false);
	}

	mIsGrounded = false;

#if _DEBUG
	CDebugPrint::Print("TrashBagIsGold:%s\n", GetIsGold() ? "true" : "false");
#endif
}

// 衝突処理
void CTrashBag::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体コライダ―の場合
	if (self == mpBodyCol)
	{
		// 衝突した相手がフィールドの場合
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

				// 接地した
				mIsGrounded = true;
				// 接地した地面の法線を記憶しておく
				mGroundNormal = hit.adjust.Normalized();

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}
			}
			// 内積の結果がマイナスであれば、天井と衝突した
			else if (dot < 0.0f)
			{
				// ジャンプなどで天井にしたから衝突したとき（上移動）のみ
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
		// 衝突した相手がオブジェクトだった場合
		else if (other->Layer() == ELayer::eObject)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
		}
		// 衝突した相手がプレイヤーなら
		else if (other->Layer() == ELayer::ePlayer)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);

			CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
			// 開いているなら
			if (player->GetOpen())
			{
				// 無効にする
				SetOnOff(false);
				// ゴールドじゃない場合
				if (!GetIsGold())
				{
					// 通常のゴミ袋の数を1増やす
					player->SetTrashBag(1);
				}
				// ゴールドの場合
				else
				{
					// ゴールドのゴミ袋の数を1増やす
					player->SetGoldTrashBag(1);
				}
			}
		}
		// 衝突した相手が敵だったら
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);

			CTrashEnemy* enemy = dynamic_cast<CTrashEnemy*>(other->Owner());
			// 開いているなら
			if (enemy->GetOpen())
			{
				// 無効にする
				SetOnOff(false);
				// ゴールドじゃない場合
				if (!GetIsGold())
				{
					// 通常のゴミ袋の数を1増やす
					enemy->SetTrashBag(1);
				}
				// ゴールドの場合
				else
				{
					// ゴールドのゴミ袋の数を1増やす
					enemy->SetGoldTrashBag(1);
				}
			}
		}
		// 衝突した相手がゴミ袋なら
		else if (other->Layer() == ELayer::eTrashBag)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
		}
		// 衝突した相手が車両なら
		else if (other->Layer() == ELayer::eVehicle)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
		}
	}
}

// 描画
void CTrashBag::Render()
{
	mpModel->Render(Matrix());
}

// 有効無効にする
void CTrashBag::SetOnOff(bool isOnOff)
{
	// 有効無効を設定
	SetEnable(isOnOff);
	SetShow(isOnOff);
	// 経路探索用のノードの有効無効を設定
	mpNavNode->SetEnable(isOnOff);

	// 有効にするなら消滅までの時間をリセット
	if (isOnOff) SetDeleteTime();
}

// ゴールド化する
void CTrashBag::SetGold(bool gold)
{
	// ゴールドの場合
	if (gold)
	{
		// ゴールドのモデル
		mpModel = CResourceManager::Get<CModel>("TrashBagGold");		
	}
	// 通常の場合
	else
	{
		// 通常のモデル
		mpModel = CResourceManager::Get<CModel>("TrashBag");
	}
	SetIsGold(gold);
}

// 飛んでいく速度を設定
void CTrashBag::SetThrowSpeed(CVector speed, float speedY)
{
	mMoveSpeed = speed;
	mMoveSpeedY = speedY;
}

// 重力をかけるかを設定
void CTrashBag::SetGravity(bool gravity)
{
	mIsGravity = gravity;
}

// ゴミ袋をアタッチする行列を設定
void CTrashBag::SetAttachMtx(const CMatrix* mtx)
{
	mpAttachMtx = mtx;
}

// 行列を取得
CMatrix CTrashBag::Matrix() const
{
	CMatrix m = CTransform::Matrix();
	// くっつける行列が設定されていれば
	if (mpAttachMtx != nullptr)
	{
		// その行列にくっつける
		m = m * mAttachMtx;
	}
	// 持ち主が設定されていれば
	else if (mpOwner != nullptr)
	{
		// 持ち主の行列に付属
		m = m * mpOwner->Matrix();
	}
	return m;
}

// 行列を更新
void CTrashBag::UpdateMtx()
{
	// くっつける行列が設定されていれば、
	if (mpAttachMtx != nullptr)
	{
		// くっつける行列をメンバ変数にコピー
		mAttachMtx = *mpAttachMtx;
	}
}
