#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"

// コンストラクタ
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
{
}

CEnemy::~CEnemy()
{
}

//更新処理
void CEnemy::Update()
{
	// 現在の状態に合わせて更新処理を切り替え
	switch (mState)
	{
	case EState::eIdle:				UpdateIdle();			break;
	case EState::ePatrol:			UpdatePatrol();			break;
	case EState::eChase:			UpdateChase();			break;
	case EState::eLost:				UpdateLost();			break;
	case EState::eDamageStart:		UpdateDamageStart();	break;
	case EState::eDamage:			UpdateDamage();			break;
	case EState::eDamageEnd:		UpdateDamageEnd();		break;
	case EState::eJumpStart:		UpdateJumpStart();		break;
	case EState::eJump:				UpdateJump();			break;
	case EState::eJumpEnd:			UpdateJumpEnd();		break;
	case EState::eAttackStart:		UpdateAttackStart();	break;
	case EState::eAttack:			UpdateAttack();			break;
	case EState::eAttackEnd:		UpdateAttackEnd();		break;
	case EState::eCriticalStart:	UpdateCriticalStart();	break;
	case EState::eCritical:			UpdateCritical();		break;
	case EState::eCriticalEnd:		UpdateCriticalEnd();	break;
	case EState::eOpenClose:		UpdateOpenClose();		break;
	}

	// キャラクターの更新
	CXCharacter::Update();

	CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
}

// 衝突処理
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// 待機状態
void CEnemy::UpdateIdle()
{
}

// 巡回処理
void CEnemy::UpdatePatrol()
{
}

// 追跡処理
void CEnemy::UpdateChase()
{
}

// プレイヤーを見失った処理
void CEnemy::UpdateLost()
{
}

// 被弾開始
void CEnemy::UpdateDamageStart()
{
}

// 被弾ノックバック
void CEnemy::UpdateDamage()
{
}

// 被弾終了
void CEnemy::UpdateDamageEnd()
{
}

// ジャンプ開始
void CEnemy::UpdateJumpStart()
{
}

// ジャンプ中
void CEnemy::UpdateJump()
{

}

// ジャンプ終了
void CEnemy::UpdateJumpEnd()
{
}

// 攻撃開始
void CEnemy::UpdateAttackStart()
{
}

// 攻撃中
void CEnemy::UpdateAttack()
{
}

// 攻撃終了
void CEnemy::UpdateAttackEnd()
{
}

// クリティカル攻撃開始
void CEnemy::UpdateCriticalStart()
{
}

// クリティカル攻撃中
void CEnemy::UpdateCritical()
{
}

// クリティカル攻撃終了
void CEnemy::UpdateCriticalEnd()
{
}

// 蓋を開閉する
void CEnemy::UpdateOpenClose()
{
}

// アニメーション切り替え
void CEnemy::ChangeAnimation(EAnimType type)
{
}

// ステータス切り替え
void CEnemy::ChangeState(EState state)
{
	// 既に同じ状態であれば、処理しない
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// 状態の文字列を取得
std::string CEnemy::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return "待機";
	case EState::ePatrol:			return "巡回中";
	case EState::eChase:			return "追跡";
	case EState::eLost:				return "見失う";
	case EState::eDamageStart:		return "被弾開始";
	case EState::eDamage:			return "被弾中";
	case EState::eDamageEnd:		return "被弾終了";
	case EState::eJumpStart:		return "ジャンプ開始";
	case EState::eJump:				return "ジャンプ中";
	case EState::eJumpEnd:			return "ジャンプ終了";
	case EState::eAttackStart:		return "攻撃開始";
	case EState::eAttack:			return "攻撃中";
	case EState::eAttackEnd:		return "攻撃終了";
	case EState::eCriticalStart:	return "クリティカル攻撃開始";
	case EState::eCritical:			return "クリティカル攻撃中";
	case EState::eCriticalEnd:		return "クリティカル攻撃終了";
	case EState::eOpenClose:		return "開閉中";
	}
	return "";
}
