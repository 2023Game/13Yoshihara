#include "CEnemyStateBase.h"
#include "CEnemy.h"

// 詠唱の速度
#define CAST_SPEED 60.0f / 200.0f

// 開始時の処理
void CEnemyStateBase::Start(CEnemy* enemy)
{
	enemy->SetStateStep(0);
	enemy->SetElapsedTime(0.0f);
}

// 終了時の処理
void CEnemyStateBase::End(CEnemy* enemy)
{
}

// 呪文詠唱の処理
void CEnemyStateBase::Cast(CEnemy* enemy, bool atkSpell)
{	
	// 経過させる
	enemy->AddElapsedTime(Times::DeltaTime());

	// 詠唱時間が経過したら
	if (enemy->GetElapsedTime() > CAST_SPEED)
	{
		// 詠唱予定の文字列
		std::string str;
		// 詠唱済みの文字列を取得
		std::string castedStr = enemy->GetNewStr();

		// 詠唱速度分減少
		enemy->AddElapsedTime(-CAST_SPEED);

		// ステップを取得
		int step = enemy->GetStateStep();
		switch (step)
		{
			// 属性の詠唱
		case 0:
			// 詠唱する文字列を取得
			str = enemy->GetMainElementalStr();
			// 無属性なら次へ
			if (str == "neutral")
			{
				// 次のステップへ
				enemy->SetStateStep(step + 1);
				return;
			}
			break;

			// 形の詠唱
		case 1:
			// 詠唱する文字列を取得
			str = enemy->GetCastShapeStr();
			break;

			// 詠唱
		case 2:
			enemy->CastSpell();
			// 初期化しておく
			CEnemyStateBase::Start(enemy);
			// 最適な行動に変化
			enemy->ChangeBestState();
			return;
			break;
		}

		// 詠唱予定の文字列が残っていたら
		if (str.size() > castedStr.size())
		{
			// 詠唱済みの次の文字
			char c = str[castedStr.size()];
			// 詠唱済みの文字列に追加
			enemy->BasicCastSpell(std::string(1, c));
		}
		// 詠唱予定の文字列が残っていない
		else
		{
			// 次のステップへ
			enemy->SetStateStep(step + 1);
			// スペースを追加する
			enemy->BasicCastSpell(" ");
			// 攻撃呪文なら
			if (atkSpell)
			{
				// 使う呪文を再決定する
				enemy->SetCastShapeStr(CEnemyContext::Instance()->ScoreAtkSpell(enemy->GetContext()));
			}
			// 攻撃呪文でないなら
			else
			{
				// 使う呪文を再決定する
				enemy->SetCastShapeStr(CEnemyContext::Instance()->ScoreSpSpell(enemy->GetContext()));
			}
		}
	}
}
