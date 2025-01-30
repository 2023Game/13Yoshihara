#include "CTrashScoreUI.h"
#include "CTextUI2D.h"
#include "CTrashPlayer.h"
#include "CTrashBagStatus.h"

// スコアUIのオフセット座標
#define SCORE_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.4f,WINDOW_HEIGHT - 100.0f,0.0f)

// コンストラクタ
CTrashScoreUI::CTrashScoreUI()
	: mScore(0)
{
	mpScoreUI = new CTextUI2D();
	mpTrashBagStatus = new CTrashBagStatus(false);
}

// デストラクタ
CTrashScoreUI::~CTrashScoreUI()
{
}

// 更新
void CTrashScoreUI::Update()
{
	// 描画する文字列を設定
	mpScoreUI->ChangeStr("スコア：%d\n", GetScore());
	// 座標を設定
	mpScoreUI->Position(SCORE_UI_OFFSET_POS);
}

// スコアを取得する
int CTrashScoreUI::GetScore() const
{
	// プレイヤーを取得
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// プレイヤーの持っている通常とゴールドのゴミ袋の数を取得
	int trashBagNum = player->GetTrashBag();
	int goldTrashBagNum = player->GetGoldTrashBag();
	// ゴミ袋ステータスからゴミ袋のポイントを取得して
	// ゴミ袋の数と得点を掛けてスコアを計算
	int score = trashBagNum * mpTrashBagStatus->GetPoint(false);
	score += goldTrashBagNum * mpTrashBagStatus->GetPoint(true);
	return score;
}
