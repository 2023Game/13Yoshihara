#include "CEnemyBase.h"
#include "CDebugFieldOfView.h"
#include "CPlayerBase.h"
#include "Maths.h"

#define GRAVITY 0.0625f
#define ROTATE_SPEED 6.0f	// 回転速度
#define ATTACK_RANGE 18.0f	// 攻撃範囲

// コンストラクタ
CEnemyBase::CEnemyBase(float fovAngle, float fovLength,
	std::vector<CVector> patrolPoints)
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
	, mLostPlayerPos(CVector::zero)
	, mNextPatrolIndex(-1)
	, mPatrolPoints(patrolPoints)
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

// オブジェクトが視界範囲内に入ったかどうか
bool CEnemyBase::IsFoundObject(CObjectBase* obj) const
{
	// オブジェクト座標を取得
	CVector objectPos = obj->Position();
	// 自分自身の座標を取得
	CVector pos = Position();
	// 自分からオブジェクトまでのベクトルを求める
	CVector vec = objectPos - pos;
	vec.Y(0.0f);	// オブジェクトとの高さの差を考慮しない

	// ①視野角度内か求める
	// ベクトルを正規化して長さを1にする
	CVector dir = vec.Normalized();
	// 自身の正面方向ベクトルを取得
	CVector forward = VectorZ();
	// オブジェクトまでのベクトルと
	// 自身の正面方向ベクトルの内積を求めて角度を出す
	float dot = CVector::Dot(dir, forward);
	// 視野範囲のラジアンを取得
	float angleR = Math::DegreeToRadian(mFovAngle);
	// 求めた内積と、視野角度で比較し、視野範囲内か判断する
	if (dot < cosf(angleR)) return false;

	// ②視野距離内か求める
	// オブジェクトまでの距離と視野距離で、視野範囲内か判断する
	float dist = vec.Length();
	if (dist > mFovLength) return false;

	// TODO：オブジェクトとの間に遮蔽物がないかチェックする

	// 全ての条件をクリアしたので、視野範囲内である
	return true;
}

// プレイヤーが視野範囲内に入ったかどうか
bool CEnemyBase::IsFoundPlayer() const
{
	// プレイヤーが存在しない場合は、視野範囲外とする
	CPlayerBase* player = CPlayerBase::Instance();
	if (player == nullptr) return false;

	// プレイヤーが視界内に入ったかどうか
	return IsFoundObject(player);
}

// プレイヤーを攻撃できるかどうか
bool CEnemyBase::CanAttackPlayer() const
{
	// プレイヤーがいない場合は、攻撃できない
	CPlayerBase* player = CPlayerBase::Instance();
	if (player == nullptr) return false;

	// プレイヤーまでの距離が攻撃範囲外であれば、攻撃できない
	CVector playerPos = player->Position();
	CVector vec = playerPos - Position();
	vec.Y(0.0f);
	float dist = vec.Length();
	if (dist > ATTACK_RANGE) return false;

	// 全ての条件をクリアしたので、攻撃できる
	return true;
}

// 指定した位置まで移動する
bool CEnemyBase::MoveTo(const CVector& targetPos, float speed)
{
	// 目的地までのベクトルを求める
	CVector pos = Position();
	CVector vec = targetPos - pos;
	vec.Y(0.0f);
	// 移動方向ベクトルを求める
	CVector moveDir = vec.Normalized();

	// 徐々に移動方向へ向ける
	CVector forward = CVector::Slerp
	(
		VectorZ(),	// 現在の正面方向
		moveDir,	// 移動方向
		ROTATE_SPEED * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

	// 今回の移動距離を求める
	float moveDist = speed * Times::DeltaTime();
	// 目的地までの残りの距離を求める
	float remainDist = vec.Length();
	// 残りの距離が移動距離より短い
	if (remainDist <= moveDist)
	{
		pos = CVector(targetPos.X(), pos.Y(), targetPos.Z());
		Position(pos);
		return true;	// 目的地に到着したので、trueを返す
	}

	// 残りの距離が移動距離より長い場合は
	// 移動距離分目的地へ移動
	pos += moveDir * moveDist;
	Position(pos);

	// 目的地には到着しなかった
	return false;
}

// 次に巡回するポイントを変更
void CEnemyBase::ChangePatrolPoint()
{
	// 巡回ポイントが設定されていない場合は、処理しない
	int size = mPatrolPoints.size();
	if (size == 0) return;

	// 巡回開始時であれば、一番近い巡回ポイントを選択
	if (mNextPatrolIndex == -1)
	{
		int nearIndex = -1;	// 一番近い巡回ポイントの番号
		float nearDist = 0.0f;	// 一番近い巡回ポイントまでの距離
		// 全ての巡回ポイントの距離を調べ、一番近い巡回ポイントを探す
		for (int i = 0; i < size; i++)
		{
			CVector point = mPatrolPoints[i];
			CVector vec = point - Position();
			vec.Y(0.0f);
			float dist = vec.Length();
			// 一番最初の巡回ポイントもしくは、
			// 現在一番近い巡回ポイントよりさらに近い場合は、
			// 巡回ポイントの番号を置き換える
			if (nearIndex < 0 || dist < nearDist)
			{
				nearIndex = i;
				nearDist = dist;
			}
		}
		mNextPatrolIndex = nearIndex;
	}
	// 巡回中だった場合、次の巡回ポイントを指定
	else
	{
		mNextPatrolIndex++;
		if (mNextPatrolIndex >= size) mNextPatrolIndex -= size;
	}
}
