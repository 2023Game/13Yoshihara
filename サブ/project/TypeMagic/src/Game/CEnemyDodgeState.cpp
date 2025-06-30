#include "CEnemyDodgeState.h"
#include "CEnemy.h"
#include "Maths.h"

// 呪文を使うMPの残量
#define MP_THRESHOLD 0.7f

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
	CEnemyContext::EnemyContext context = enemy->GetContext();
	// MPが潤沢かつ
	// 呪文が飛んできていないか近くない
	if (context.mpRatio > MP_THRESHOLD &&
		(!context.isSpellComing || !CEnemyContext::Instance()->IsNear(context.distanceToSpell)))
	{
		Cast(enemy, false);
	}
	else
	{
		// 文字列が残っていたら詠唱して削除
		enemy->CastSpell();
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

		// 最適な行動に変更
		enemy->ChangeBestState();
	}
}

// 終了時の処理
void CEnemyDodgeState::End(CEnemy* enemy)
{
	// 文字列が残っていたら詠唱して削除
	enemy->CastSpell();
}

// 名前を取得
std::string CEnemyDodgeState::GetName() const
{
	return "Dodge";
}
