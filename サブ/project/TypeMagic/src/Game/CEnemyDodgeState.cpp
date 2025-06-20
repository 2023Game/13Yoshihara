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

	switch (enemy->GetStateStep())
	{
	case 0:
	{
		// ランダム数値
		int rand = Math::Rand(1, 2);

		// ランダムで左右の回避方向を決定
		switch (rand)
		{
		case 1:
			moveDir = CVector::up.Cross(dir);
			break;
		case 2:
			moveDir = dir.Cross(CVector::up);
			break;
		}

		// ランダムの数値のステップへ進む
		enemy->SetStateStep(rand);
		break;
	}
	// 回避方向の更新
	case 1:
		moveDir = CVector::up.Cross(dir);
		break;

		// 回避方向の更新
	case 2:
		moveDir = dir.Cross(CVector::up);
		break;
	}

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
