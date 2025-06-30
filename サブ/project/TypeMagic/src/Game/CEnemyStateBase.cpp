#include "CEnemyStateBase.h"
#include "CEnemy.h"

// ‰r¥‚Ì‘¬“x
#define CAST_SPEED 60.0f / 200.0f

// ŠJŽnŽž‚Ìˆ—
void CEnemyStateBase::Start(CEnemy* enemy)
{
	enemy->SetStateStep(0);
	enemy->SetElapsedTime(0.0f);
}

// Žô•¶‰r¥‚Ìˆ—
void CEnemyStateBase::Cast(CEnemy* enemy, bool atkSpell)
{	
	// Œo‰ß‚³‚¹‚é
	enemy->AddElapsedTime(Times::DeltaTime());

	// ‰r¥ŽžŠÔ‚ªŒo‰ß‚µ‚½‚ç
	if (enemy->GetElapsedTime() > CAST_SPEED)
	{
		// ‰r¥—\’è‚Ì•¶Žš—ñ
		std::string str;
		// ‰r¥Ï‚Ý‚Ì•¶Žš—ñ‚ðŽæ“¾
		std::string castedStr = enemy->GetNewStr();

		// ‰r¥‘¬“x•ªŒ¸­
		enemy->AddElapsedTime(-CAST_SPEED);

		// ƒXƒeƒbƒv‚ðŽæ“¾
		int step = enemy->GetStateStep();
		switch (step)
		{
			// ‘®«‚Ì‰r¥
		case 0:
			// ‰r¥‚·‚é•¶Žš—ñ‚ðŽæ“¾
			str = enemy->GetMainElementalStr();
			// –³‘®«‚È‚çŽŸ‚Ö
			if (str == "neutral")
			{
				// ŽŸ‚ÌƒXƒeƒbƒv‚Ö
				enemy->SetStateStep(step + 1);
				return;
			}
			break;

			// Œ`‚Ì‰r¥
		case 1:
			// ‰r¥‚·‚é•¶Žš—ñ‚ðŽæ“¾
			str = enemy->GetCastShapeStr();
			break;

			// ‰r¥
		case 2:
			enemy->CastSpell();
			// ‰Šú‰»‚µ‚Ä‚¨‚­
			CEnemyStateBase::Start(enemy);
			// Å“K‚Ès“®‚É•Ï‰»
			enemy->ChangeBestState();
			return;
			break;
		}

		// ‰r¥—\’è‚Ì•¶Žš—ñ‚ªŽc‚Á‚Ä‚¢‚½‚ç
		if (str.size() > castedStr.size())
		{
			// ‰r¥Ï‚Ý‚ÌŽŸ‚Ì•¶Žš
			char c = str[castedStr.size()];
			// ‰r¥Ï‚Ý‚Ì•¶Žš—ñ‚É’Ç‰Á
			enemy->BasicCastSpell(std::string(1, c));
		}
		// ‰r¥—\’è‚Ì•¶Žš—ñ‚ªŽc‚Á‚Ä‚¢‚È‚¢
		else
		{
			// ŽŸ‚ÌƒXƒeƒbƒv‚Ö
			enemy->SetStateStep(step + 1);
			// ƒXƒy[ƒX‚ð’Ç‰Á‚·‚é
			enemy->BasicCastSpell(" ");
			// UŒ‚Žô•¶‚È‚ç
			if (atkSpell)
			{
				// Žg‚¤Žô•¶‚ðÄŒˆ’è‚·‚é
				enemy->SetCastShapeStr(CEnemyContext::Instance()->ScoreAtkSpell(enemy->GetContext()));
			}
			// UŒ‚Žô•¶‚Å‚È‚¢‚È‚ç
			else
			{
				// Žg‚¤Žô•¶‚ðÄŒˆ’è‚·‚é
				enemy->SetCastShapeStr(CEnemyContext::Instance()->ScoreSpSpell(enemy->GetContext()));
			}
		}
	}
}
