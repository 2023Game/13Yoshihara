#include "CEnemyIdleState.h"
#include "CEnemy.h"

// インスタンス
CEnemyIdleState* CEnemyIdleState::Instance()
{
	static CEnemyIdleState instance;
	return &instance;
}

// コンストラクタ
CEnemyIdleState::CEnemyIdleState()
{
}

// デストラクタ
CEnemyIdleState::~CEnemyIdleState()
{
}

// 開始時の処理
void CEnemyIdleState::Start(CEnemy* enemy)
{
	// 基底クラスの開始時の処理
	CEnemyStateBase::Start(enemy);
}

// 更新中の処理
void CEnemyIdleState::Update(CEnemy* enemy)
{
	// 最適な行動に変更
	enemy->ChangeBestState();
}

// 終了時の処理
void CEnemyIdleState::End(CEnemy* enemy)
{
	// 基底クラスの終了時の処理
	CEnemyStateBase::End(enemy);
}

// 名前を取得
std::string CEnemyIdleState::GetName() const
{
	return "Idle";
}
