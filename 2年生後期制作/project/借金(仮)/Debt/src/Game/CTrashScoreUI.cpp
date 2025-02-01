#include "CTrashScoreUI.h"
#include "CTextUI2D.h"
#include "CTrashPlayer.h"
#include "CTrashBagStatus.h"
#include "CScoreManager.h"
#include "CText.h"

// スコアUIのオフセット座標
#define SCORE_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.35f,WINDOW_HEIGHT - 100.0f,0.0f)

// スコア表示UIのインスタンス
CTrashScoreUI* CTrashScoreUI::spInstance = nullptr;

// スコア表示UIのインスタンスを取得
CTrashScoreUI* CTrashScoreUI::Instance()
{
	return spInstance;
}

// コンストラクタ
CTrashScoreUI::CTrashScoreUI()
	: mScore(0)
{
	// インスタンスを設定
	spInstance = this;
	// スコアのUIを生成
	mpScoreUI = new CTextUI2D();
	// 文字の揃いの基準を設定
	mpScoreUI->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// 座標を設定
	mpScoreUI->Position(SCORE_UI_OFFSET_POS);

	// ゴミ袋の得点取得用
	CTrashBagStatus* trashBagStatus = new CTrashBagStatus(false);
	// ゴミ袋の得点を設定
	mTrashBagScore = trashBagStatus->GetPoint(false);
	mGoldTrashBagScore = trashBagStatus->GetPoint(true);
	// 不要なので削除
	delete trashBagStatus;
}

// デストラクタ
CTrashScoreUI::~CTrashScoreUI()
{
	SAFE_DELETE(spInstance);
}

// 更新
void CTrashScoreUI::Update()
{
	// 描画する文字列を設定
	mpScoreUI->ChangeToStr("Score：%d\n", GetScore());
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
	int score = trashBagNum * mTrashBagScore;
	score += goldTrashBagNum * mGoldTrashBagScore;
	return score;
}
