#include "CEnemyRunState.h"
#include "CEnemy.h"
#include "SpellDefine.h"

// インスタンス
CEnemyRunState* CEnemyRunState::Instance()
{
	static CEnemyRunState instance;
	return &instance;
}

// コンストラクタ
CEnemyRunState::CEnemyRunState()
{
}

// デストラクタ
CEnemyRunState::~CEnemyRunState()
{
}

// 開始時の処理
void CEnemyRunState::Start(CEnemy* enemy)
{
	// 基底クラスの開始時の処理
	CEnemyStateBase::Start(enemy);
}

// 更新中の処理
void CEnemyRunState::Update(CEnemy* enemy)
{
	// 自分への方向
	CVector moveDir = enemy->Position() - enemy->GetOpponent()->Position();
	moveDir.Normalize();
	enemy->SetMoveDir(moveDir);

	// 移動を設定
	CVector moveSpeed = enemy->GetMoveDir() * enemy->GetBaseMoveSpeed() * Times::DeltaTime();
	enemy->SetMoveSpeed(moveSpeed);

	// 最適な行動に変更
	enemy->ChangeBestState();
}

// 終了時の処理
void CEnemyRunState::End(CEnemy* enemy)
{
}

// 名前を取得
std::string CEnemyRunState::GetName() const
{
	return "Run";
}
