#include "CEnemyCastState.h"
#include "CEnemy.h"

// 詠唱の速度
#define CAST_SPEED 1 / 5.6f

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
}

// 更新中の処理
void CEnemyCastState::Update(CEnemy* enemy)
{
	// 属性を取得
	ESpellElementalType elemental = enemy->GetMainElemental();
	// 形を取得
	ESpellShapeType shape = enemy->GetCastShape();
	// 経過させる
	enemy->AddElapsedTime(Times::DeltaTime());

	// 詠唱時間が経過したら
	if (enemy->GetElapsedTime() > CAST_SPEED)
	{
		// 詠唱速度分減少
		enemy->AddElapsedTime(-CAST_SPEED);
	}
}

// 終了時の処理
void CEnemyCastState::End(CEnemy* enemy)
{
}

// 名前を取得
std::string CEnemyCastState::GetName() const
{
	return "Cast";
}
