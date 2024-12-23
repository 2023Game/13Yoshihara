#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CGaugeUI3D.h"

#define GRAVITY 0.0625f

// コンストラクタ
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mState(0)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mGroundNormal(CVector::up)
	, mpBodyCol(nullptr)
	, mpHpGauge(nullptr)
{
}

// デストラクタ
CEnemy::~CEnemy()
{
	// コライダー削除
	SAFE_DELETE(mpBodyCol);

	// HPゲージが存在したら、一緒に削除する
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
	}
}

// オブジェクト削除を伝える関数
void CEnemy::DeleteObject(CObjectBase* obj)
{
	// 削除されたのがHPゲージであれば、
	// HPゲージのポインタを空にする
	if (obj == mpHpGauge)
	{
		mpHpGauge = nullptr;
	}
}

//更新処理
void CEnemy::Update()
{
	//// 現在の状態に合わせて更新処理を切り替え
	//switch (mState)
	//{
	//case EState::eIdle:				UpdateIdle();			break;
	//case EState::ePatrol:			UpdatePatrol();			break;
	//case EState::eChase:			UpdateChase();			break;
	//case EState::eLost:				UpdateLost();			break;
	//case EState::eDamageStart:		UpdateDamageStart();	break;
	//case EState::eDamage:			UpdateDamage();			break;
	//case EState::eDamageEnd:		UpdateDamageEnd();		break;
	//case EState::eJumpStart:		UpdateJumpStart();		break;
	//case EState::eJump:				UpdateJump();			break;
	//case EState::eJumpEnd:			UpdateJumpEnd();		break;
	//case EState::eAttackStart:		UpdateAttackStart();	break;
	//case EState::eAttack:			UpdateAttack();			break;
	//case EState::eAttackEnd:		UpdateAttackEnd();		break;
	//case EState::eCriticalStart:	UpdateCriticalStart();	break;
	//case EState::eCritical:			UpdateCritical();		break;
	//case EState::eCriticalEnd:		UpdateCriticalEnd();	break;
	//case EState::eOpenClose:		UpdateOpenClose();		break;
	//}

	// 重力
	mMoveSpeedY -= GRAVITY;

	// 移動
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	Position(Position() + moveSpeed);

	// キャラクターの更新
	CXCharacter::Update();

	mIsGrounded = false;

	//CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
}

// 衝突処理
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体が衝突した
	if (self == mpBodyCol)
	{
		// フィールドと衝突した
		if (other->Layer() == ELayer::eField)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// 衝突した地面が床か天井かを内積で判定
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// 内積の結果がプラスであれば、床と衝突した
			if (dot >= 0.0f)
			{
				// 落下などで床に上から衝突した時（下移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// 接地した
				mIsGrounded = true;
				// 接地した地面の法線を記憶しておく
				mGroundNormal = hit.adjust.Normalized();
			}
			// 内積の結果がマイナスであれば、天井と衝突した
			else
			{
				// ジャンプなどで天井に下から衝突した時（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// プレイヤーと衝突した場合
		else if (other->Layer() == ELayer::ePlayer)
		{
			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
	}
}

// 描画
void CEnemy::Render()
{
	CXCharacter::Render();
}

// アニメーション切り替え
void CEnemy::ChangeAnimation(int type, bool restart)
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

// 状態切り替え
void CEnemy::ChangeState(int state)
{
	// 同じ状態の場合は切り替えない
	if (state == mState) return;

	// 状態を変更して、状態関連の変数の初期化
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

//// ステータス切り替え
//void CEnemy::ChangeState(EState state)
//{
//	// 既に同じ状態であれば、処理しない
//	if (state == mState) return;
//
//	mState = state;
//	mStateStep = 0;
//	mElapsedTime = 0.0f;
//}
//
//// 状態の文字列を取得
//std::string CEnemy::GetStateStr(EState state) const
//{
//	switch (state)
//	{
//	case EState::eIdle:				return "待機";
//	case EState::ePatrol:			return "巡回中";
//	case EState::eChase:			return "追跡";
//	case EState::eLost:				return "見失う";
//	case EState::eDamageStart:		return "被弾開始";
//	case EState::eDamage:			return "被弾中";
//	case EState::eDamageEnd:		return "被弾終了";
//	case EState::eJumpStart:		return "ジャンプ開始";
//	case EState::eJump:				return "ジャンプ中";
//	case EState::eJumpEnd:			return "ジャンプ終了";
//	case EState::eAttackStart:		return "攻撃開始";
//	case EState::eAttack:			return "攻撃中";
//	case EState::eAttackEnd:		return "攻撃終了";
//	case EState::eCriticalStart:	return "クリティカル攻撃開始";
//	case EState::eCritical:			return "クリティカル攻撃中";
//	case EState::eCriticalEnd:		return "クリティカル攻撃終了";
//	case EState::eOpenClose:		return "開閉中";
//	}
//	return "";
//}
