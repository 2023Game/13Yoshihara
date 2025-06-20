#include "CEnemyChaseState.h"
#include "CEnemy.h"
#include "SpellDefine.h"

// インスタンス
CEnemyChaseState* CEnemyChaseState::Instance()
{
	static CEnemyChaseState instance;
	return &instance;
}

// コンストラクタ
CEnemyChaseState::CEnemyChaseState()
{
}

// デストラクタ
CEnemyChaseState::~CEnemyChaseState()
{
}

// 開始時の処理
void CEnemyChaseState::Start(CEnemy* enemy)
{
	// 基底クラスの開始時の処理
	CEnemyStateBase::Start(enemy);
}

// 更新中の処理
void CEnemyChaseState::Update(CEnemy* enemy)
{
	// ボール魔法の届く距離
	float dist = BALL_SPEED * BALL_DELETE_TIME;
	// 相手への方向
	CVector moveDir = enemy->GetOpponent()->Position() - enemy->Position();
	moveDir.Normalize();
	// 移動方向を設定
	enemy->SetMoveDir(moveDir);

	// 移動を求める
	CVector moveSpeed = enemy->GetMoveDir() * enemy->GetBaseMoveSpeed() * Times::DeltaTime();
	// 移動を設定
	enemy->SetMoveSpeed(moveSpeed);
}

// 終了時の処理
void CEnemyChaseState::End(CEnemy* enemy)
{
}

// 名前を取得
std::string CEnemyChaseState::GetName() const
{
	return "Chase";
}
