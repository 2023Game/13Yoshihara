#include "CEnemyBase.h"
#include "CDebugFieldOfView.h"
#include "CPlayerBase.h"
#include "Maths.h"
#include "CFieldBase.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CGaugeUI3D.h"
#include "CCollider.h"

// コンストラクタ
CEnemyBase::CEnemyBase(float fovAngle, float fovLength,
	std::vector<CVector> patrolPoints,
	float eyeHeight)
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mMoveSpeedY(0.0f)
	, mGravity(GRAVITY)
	, mIsGrounded(false)
	, mIsGravity(true)
	, mIsMoveDir(true)
	, mpRideObject(nullptr)
	, mpBodyCol(nullptr)
	, mpAttackCol(nullptr)
	, mFovAngle(fovAngle)
	, mFovLength(fovLength)
	, mpDebugFov(nullptr)
	, mNextPatrolIndex(-1)
	, mEyeHeight(eyeHeight)
	, mNextMoveIndex(0)
	, mpDamageCauser(nullptr)
	, mpHpGauge(nullptr)
{
	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);

	// プレイヤーを見失った位置のノードを作成
	mpLostPlayerNode = new CNavNode(CVector::zero, true);
	mpLostPlayerNode->SetEnable(false);
	
	// 巡回ポイントに経路探索用のノードを配置
	for (CVector point : patrolPoints)
	{
		CNavNode* node = new CNavNode(point, true);
		mpPatrolPoints.push_back(node);
	}

#if _DEBUG
	// 視野範囲のデバッグ表示クラスを作成
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);
#endif
}

// デストラクタ
CEnemyBase::~CEnemyBase()
{
	// コライダ―削除
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpAttackCol);

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

		// 巡回ポイントをすべて削除
		for (CNavNode* node : mpPatrolPoints)
		{
			delete node;
		}
		// リストをクリア
		mpPatrolPoints.clear();
		mpMoveRoute.clear();
		// 巡回ポイントに配置したノードも全て削除
		//auto itr = mpPatrolPoints.begin();
		//auto end = mpPatrolPoints.end();
		//while (itr != end)
		//{
		//	CNavNode* del = *itr;
		//	itr = mpPatrolPoints.erase(itr);
		//	delete del;
		//}
	}

	// HPゲージが存在したら、一緒に削除する
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
	}
}

// 本体コライダ―を取得
CCollider* CEnemyBase::GetBodyCol() const
{
	return mpBodyCol;
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
	// 削除されたオブジェクトがHpゲージであれば
	// ポインタを空にする
	else if (obj == mpHpGauge)
	{
		mpHpGauge = nullptr;
	}
}

// 更新
void CEnemyBase::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 重力を掛けるなら
	if (mIsGravity)
	{
		mMoveSpeedY -= mGravity;
	}

	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// 移動
	Position(Position() + moveSpeed);

	// 攻撃を受けていない時かつ
	// 移動方向を向く設定がオンの時
	if (!mIsDamage &&
		mIsMoveDir)
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
}

// 描画
void CEnemyBase::Render()
{
	CXCharacter::Render();
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
	// TODO：フィールド管理クラスから取得する
	//// フィールドが存在しない場合は、遮蔽物がないので見える
	//CFieldBase* field = CFieldBase::Instance();
	//if (field == nullptr) return true;

	CVector offsetPos = CVector(0.0f, mEyeHeight, 0.0f);
	// プレイヤーの座標を取得
	CVector playerPos = player->Position() + offsetPos;
	// 自分自身の座標を取得
	CVector selfPos = Position() + offsetPos;

	CHitInfo hit;
	bool isHit = false;
	//// フィールドとのレイ判定を行い、
	//// 遮蔽物が存在した場合は、プレイヤーが見えない
	//if (isHit = field->CollisionRay(selfPos, playerPos, &hit)) return false;

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
void CEnemyBase::ChangePatrolPoint(float patrolNearDist)
{
	// 巡回ポイントが設定されていない場合は、処理しない
	int size = mpPatrolPoints.size();
	if (size == 0) return;

	// 巡回開始時であれば、一番近い巡回ポイントを選択
	if (mNextPatrolIndex == -1)
	{
		int nearIndex = -1;	// 一番近い巡回ポイントの番号
		float nearDist = 0.0f;	// 一番近い巡回ポイントまでの距離
		// 全ての巡回ポイントの距離を調べ、一番近い巡回ポイントを探す
		for (int i = 0; i < size; i++)
		{
			CVector point = mpPatrolPoints[i]->GetPos();
			CVector vec = point - Position();
			vec.Y(0.0f);
			float dist = vec.Length();
			// 巡回ポイントが近すぎる場合は、スルー
			if (dist < patrolNearDist) continue;
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
			// 経路探索用のノードの座標を更新
			mpNavNode->SetPos(Position());

			// 巡回ポイントの経路探索ノードの位置を設定しなおすことで、
			// 各ノードへの接続情報を更新
			for (CNavNode* node : mpPatrolPoints)
			{
				node->SetPos(node->GetPos());
			}
			// 巡回ポイントまでの最短経路を求める
			navMgr->Navigate(mpNavNode, mpPatrolPoints[mNextPatrolIndex], mpMoveRoute);
			// 最短経路が見つからなかった場合
			if (mpMoveRoute.size() < 2)
			{
				// ゴール地点を移動ルートとして設定する
				mpMoveRoute.clear();
				mpMoveRoute.push_back(mpPatrolPoints[mNextPatrolIndex]);
				mNextMoveIndex = 0;
			}
			// 最短経路が見つかった場合
			// 一番最初の地点（現在位置を除いて）を次の移動ポイントとする
			else
			{
				// 次の目的地のインデックスを設定
				mNextMoveIndex = 1;
			}
		}
	}
}
