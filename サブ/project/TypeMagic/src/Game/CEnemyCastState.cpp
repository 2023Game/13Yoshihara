#include "CEnemyCastState.h"
#include "CEnemy.h"

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
	// TODO:一文字ずつ詠唱していく
	enemy->CastStart(elemental, shape);
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
