#include "CEnemyBase.h"
#include "CDebugFieldOfView.h"

#define GRAVITY 0.0625f

// コンストラクタ
CEnemyBase::CEnemyBase(float fovAngle, float fovLength)
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mIsWall(false)
	, mpRideObject(nullptr)
	, mIsDamage(false)
	, mpBodyCol(nullptr)
	, mFovAngle(fovAngle)
	, mFovLength(fovLength)
	, mpDebugFov(nullptr)
{
	// 視野範囲のデバッグ表示クラスを作成
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);
}

// デストラクタ
CEnemyBase::~CEnemyBase()
{
	// コライダ―削除
	SAFE_DELETE(mpBodyCol);
	// 視野範囲のデバッグ表示が存在したら、一緒に削除
	if (mpDebugFov != nullptr)
	{
		mpDebugFov->SetOwner(nullptr);
		mpDebugFov->Kill();
	}
}

// オブジェクト削除処理
void CEnemyBase::DeleteObject(CObjectBase* obj)
{
	// 削除されたオブジェクトが視野範囲のデバッグ表示であれば
	// ポインタを空にする
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
}

// 更新
void CEnemyBase::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// 移動
	Position(Position() + moveSpeed);

	// 攻撃を受けていない時は
	if (!mIsDamage)
	{
		// 敵を移動方向へ向ける
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.125f);
		Rotation(CQuaternion::LookRotation(forward));
	}

	// キャラクターの更新
	CXCharacter::Update();

	mIsGrounded = false;
	mIsWall = false;
}

// 描画
void CEnemyBase::Render()
{
	CXCharacter::Render();
}

/*
衝突処理
本体コライダと地形の衝突
*/
void CEnemyBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体コライダとの衝突判定
	if (self == mpBodyCol)
	{
		// 衝突した相手がフィールドの場合
		if (other->Layer() == ELayer::eField)
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
	}
}

// アニメーション切り替え
void CEnemyBase::ChangeAnimation(int type, bool restart)
{
	if (mpAnimData == nullptr) return;
	if (!(0 <= type && type < mpAnimData->size())) return;
	AnimData data = (*mpAnimData)[type];
	CXCharacter::ChangeAnimation
	(
		type,
		data.loop,
		data.frameLength,
		restart
	);
	CXCharacter::SetAnimationSpeed(data.speed);
}
