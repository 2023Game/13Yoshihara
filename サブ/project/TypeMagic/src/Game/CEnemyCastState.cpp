#include "CEnemyCastState.h"
#include "CEnemy.h"

// 詠唱の速度
#define CAST_SPEED 60.0f / 200.0f

// インスタンス
CEnemyCastState* CEnemyCastState::Instance()
{
	static CEnemyCastState instance;
	return &instance;
}

// コンストラクタ
CEnemyCastState::CEnemyCastState()
{
}

// デストラクタ
CEnemyCastState::~CEnemyCastState()
{
}

// 開始時の処理
void CEnemyCastState::Start(CEnemy* enemy)
{
	// 基底クラスの開始時の処理
	CEnemyStateBase::Start(enemy);

	// 使う呪文を決定する
	enemy->SetCastShapeStr(CEnemyContext::Instance()->ScoreAtkSpell(enemy->GetContext()));
}

// 更新中の処理
void CEnemyCastState::Update(CEnemy* enemy)
{
	// 呪文の詠唱
	Cast(enemy, true);
}

// 終了時の処理
void CEnemyCastState::End(CEnemy* enemy)
{
	// 文字列が残っていたら詠唱して削除
	enemy->CastSpell();
	// 基底クラスの終了時の処理
	CEnemyStateBase::End(enemy);
}

// 名前を取得
std::string CEnemyCastState::GetName() const
{
	return "Cast";
}
