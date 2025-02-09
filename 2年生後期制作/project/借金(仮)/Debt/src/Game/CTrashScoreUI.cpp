#include "CTrashScoreUI.h"
#include "CTextUI2D.h"
#include "CTrashPlayer.h"
#include "CTrashBagStatus.h"
#include "CScoreManager.h"
#include "CText.h"

// コンストラクタ
CTrashScoreUI::CTrashScoreUI()
	: CScoreUIBase()
{
	// ゴミ袋の得点取得用
	CTrashBagStatus* trashBagStatus = new CTrashBagStatus(false);
	// ゴミ袋の得点を取得
	mTrashBagScore = trashBagStatus->GetPoint(false);
	mGoldTrashBagScore = trashBagStatus->GetPoint(true);
	// 不要なので削除
	delete trashBagStatus;
}

// デストラクタ
CTrashScoreUI::~CTrashScoreUI()
{
}

// 更新
void CTrashScoreUI::Update()
{
	CScoreUIBase::Update();
}

// スコアを計算して取得する
int CTrashScoreUI::GetScore() const
{
	// プレイヤーを取得
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// プレイヤーの持っている通常とゴールドのゴミ袋の数を取得
	int trashBagNum = player->GetTrashBag();
	int goldTrashBagNum = player->GetGoldTrashBag();
	// ゴミ袋の数と得点を掛けてスコアを計算
	int score = trashBagNum * mTrashBagScore;
	score += goldTrashBagNum * mGoldTrashBagScore;
	return score;
}
