#include "CEnemyDodgeState.h"
#include "CEnemy.h"
#include "Maths.h"

// インスタンス
CEnemyDodgeState* CEnemyDodgeState::Instance()
{
	static CEnemyDodgeState instance;
	return &instance;
}

// コンストラクタ
CEnemyDodgeState::CEnemyDodgeState()
{
}

// デストラクタ
CEnemyDodgeState::~CEnemyDodgeState()
{
}

// 開始時の処理
void CEnemyDodgeState::Start(CEnemy* enemy)
{
	// 基底クラスの開始時の処理
	CEnemyStateBase::Start(enemy);
}

// 更新中の処理
void CEnemyDodgeState::Update(CEnemy* enemy)
{
	// 相手への方向
	CVector dir = enemy->GetOpponent()->Position() - enemy->Position();
	dir.Normalize();
	// 移動の方向
	CVector moveDir;
	// 呪文の移動方向
	CVector spellMoveDir = enemy->GetComingSpellMoveDir();
	// 移動方向を設定
	moveDir = CVector(-spellMoveDir.Z(), 0.0f, spellMoveDir.X());

	moveDir.Normalize();
	// 移動方向を設定
	enemy->SetMoveDir(moveDir);
	// 移動を設定
	CVector moveSpeed = enemy->GetMoveDir() * enemy->GetBaseMoveSpeed() * Times::DeltaTime();
	enemy->SetMoveSpeed(moveSpeed);
}

// 終了時の処理
void CEnemyDodgeState::End(CEnemy* enemy)
{
}

// 名前を取得
std::string CEnemyDodgeState::GetName() const
{
	return "Dodge";
}
