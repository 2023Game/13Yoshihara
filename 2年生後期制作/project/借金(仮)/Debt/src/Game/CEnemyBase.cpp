#include "CEnemyBase.h"
#include "CDebugFieldOfView.h"
#include "CPlayerBase.h"
#include "Maths.h"
#include "CFieldBase.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CVehicleManager.h"

#define GRAVITY 0.0625f

// コンストラクタ
CEnemyBase::CEnemyBase(float fovAngle, float fovLength,
	std::vector<CVector> patrolPoints,
	float eyeHeight)
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mIsWall(false)
	, mpRideObject(nullptr)
	, mIsDamage(false)
	, mpBodyCol(nullptr)
	, mpAttackCol(nullptr)
	, mFovAngle(fovAngle)
	, mFovLength(fovLength)
	, mpDebugFov(nullptr)
	, mNextPatrolIndex(-1)
	, mEyeHeight(eyeHeight)
	, mNextMoveIndex(0)
{
	// 視野範囲のデバッグ表示クラスを作成
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);

	// プレイヤーを見失った位置のノードを作成
	mpLostPlayerNode = new CNavNode(CVector::zero, true);
	
	// 巡回ポイントに経路探索用のノードを配置
	for (CVector point : patrolPoints)
	{
		CNavNode* node = new CNavNode(point, true);
		mPatrolPoints.push_back(node);
	}
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

	// 経路探索用のノードを削除
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
		SAFE_DELETE(mpLostPlayerNode);

		// 巡回ポイントに配置したノードも全て削除
		auto itr = mPatrolPoints.begin();
		auto end = mPatrolPoints.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mPatrolPoints.erase(itr);
			delete del;
		}
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

	// 経路探索用のノードが存在すれば、座標を更新
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

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

// プレイヤーが視野範囲内に入ったかどうか
bool CEnemyBase::IsFoundPlayer() const
{
	// プレイヤーが存在しない場合は、視野範囲外とする
	CPlayerBase* player = CPlayerBase::Instance();
	if (player == nullptr) return false;

	// プレイヤー座標を取得
	CVector playerPos = player->Position();
	// 自分自身の座標を取得
	CVector pos = Position();
	// 自分からプレイヤーまでのベクトルを求める
	CVector vec = playerPos - pos;
	vec.Y(0.0f);	// プレイヤーとの高さの差を考慮しない

	// ①視野角度内か求める
	// ベクトルを正規化して長さを1にする
	CVector dir = vec.Normalized();
	// 自身の正面方向ベクトルを取得
	CVector forward = VectorZ();
	// プレイヤーまでのベクトルと
	// 自身の正面方向ベクトルの内積を求めて角度を出す
	float dot = CVector::Dot(dir, forward);
	// 視野範囲のラジアンを取得
	float angleR = Math::DegreeToRadian(mFovAngle);
	// 求めた内積と、視野角度で比較し、視野範囲内か判断する
	if (dot < cosf(angleR)) return false;

	// ②視野距離内か求める
	// プレイヤーまでの距離と視野距離で、視野範囲内か判断する
	float dist = vec.Length();
	if (dist > mFovLength) return false;

	// プレイヤーとの間に遮蔽物がないかチェックする
	if (!IsLookPlayer()) return false;

	// 全ての条件をクリアしたので、視野範囲内である
	return true;
}

// 現在位置からプレイヤーが見えているかどうか
bool CEnemyBase::IsLookPlayer() const
{
	// プレイヤーが存在しない場合は、見えない
	CPlayerBase* player = CPlayerBase::Instance();
	if (player == nullptr) return false;
	// フィールドが存在しない場合は、遮蔽物がないので見える
	CFieldBase* field = CFieldBase::Instance();
	if (field == nullptr) return true;
	// 車両管理クラスが存在しない場合は、車両がないので見える
	CVehicleManager* vehicleMgr = CVehicleManager::Instance();
	if (vehicleMgr == nullptr) return true;

	CVector offsetPos = CVector(0.0f, mEyeHeight, 0.0f);
	// プレイヤーの座標を取得
	CVector playerPos = player->Position() + offsetPos;
	// 自分自身の座標を取得
	CVector selfPos = Position() + offsetPos;

	CHitInfo hit;
	bool isHit = false;
	// フィールドと車両とのレイ判定を行い、
	// 遮蔽物が存在した場合は、プレイヤーが見えない
	if (isHit = field->CollisionRay(selfPos, playerPos, &hit) ||
		vehicleMgr->CollisionRay(selfPos, playerPos, &hit, isHit)) return false;

	// プレイヤーとの間に遮蔽物がないので、プレイヤーが見えている
	return true;
}

// プレイヤーを攻撃できるかどうか
bool CEnemyBase::CanAttackPlayer(float attackRange) const
{
	// プレイヤーがいない場合は、攻撃できない
	CPlayerBase* player = CPlayerBase::Instance();
	if (player == nullptr) return false;

	// プレイヤーまでの距離が攻撃範囲外であれば、攻撃できない
	CVector playerPos = player->Position();
	CVector vec = playerPos - Position();
	vec.Y(0.0f);
	float dist = vec.Length();
	if (dist > attackRange) return false;

	// 全ての条件をクリアしたので、攻撃できる
	return true;
}

// 指定した位置まで移動する
bool CEnemyBase::MoveTo(const CVector& targetPos, float speed,float rotateSpeed)
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
		rotateSpeed * Times::DeltaTime()
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
void CEnemyBase::ChangePatrolPoint(float nearDist)
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
			CVector point = mPatrolPoints[i]->GetPos();
			CVector vec = point - Position();
			vec.Y(0.0f);
			float dist = vec.Length();
			// 巡回ポイントが近すぎる場合は、スルー
			if (dist < nearDist) continue;
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

	// 次に巡回するポイントが決まった場合
	if (mNextPatrolIndex >= 0)
	{
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			// 巡回ポイントの経路探索ノードの位置を設定しなおすことで、
			// 各ノードへの接続情報を更新
			for (CNavNode* node : mPatrolPoints)
			{
				node->SetPos(node->GetPos());
			}
			// 巡回ポイントまでの最短経路を求める
			if (navMgr->Navigate(mpNavNode, mPatrolPoints[mNextPatrolIndex], mMoveRoute));
			{
				// 次の目的地のインデックスを設定
				mNextMoveIndex = 1;
			}
		}
	}
}
