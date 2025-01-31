#include "CTrashGameResultUI.h"
#include "CText.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CScoreManager.h"
#include "CTrashBagStatus.h"

// 背景画像のパス
#define BACKGROUND_PATH "UI/trash_game_result_bg.png"

// 得点のテキストのオフセット座標
#define SCORE_OFFSET_POS	CVector(WINDOW_WIDTH * 0.48f, WINDOW_HEIGHT * 0.2f, 0.0f)
// ゴミ袋の数のテキストのオフセット座標
#define NUM_OFFSET_POS		CVector(WINDOW_WIDTH * 0.4f, WINDOW_HEIGHT * 0.4f, 0.0f)
// ゴミ袋の値段のテキストのオフセット座標
#define PRICE_OFFSET_POS	CVector(WINDOW_WIDTH * 0.55f, WINDOW_HEIGHT * 0.4f, 0.0f)
// 単位のテキストのオフセット座標
#define UNIT_OFFSET_POS		CVector(WINDOW_WIDTH * 0.45f, WINDOW_HEIGHT * 0.4f, 0.0f)
// ×のテキストのオフセット座標
#define X_OFFSET_POS		CVector(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.4f, 0.0f)

// ゴミ袋の画像のサイズの倍率
#define SIZE_RATE 0.25f
// ゴミ袋の画像のオフセット座標
#define TRASH_BAG_OFFEST_POS		CVector2(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.38f)
// ゴールドのゴミ袋の画像のオフセット座標
#define GOLD_TRASH_BAG_OFFEST_POS	CVector2(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.56f)

CTrashGameResultUI::CTrashGameResultUI()
	: CResultUIBase(BACKGROUND_PATH)
{
	// ゴミ袋の得点取得用
	CTrashBagStatus* trashBagStatus = new CTrashBagStatus(false);
	// ゴミ袋の得点を記憶
	int trashBagPoint = trashBagStatus->GetPoint(false);
	int goldTrashBagPoint = trashBagStatus->GetPoint(true);
	// 不要なので削除
	delete trashBagStatus;

	// 得点管理クラスを取得
	CScoreManager* scoreMgr = CScoreManager::Instance();
	// ゴミ拾いのスコアデータを取得
	CScoreManager::TrashGameScoreData* scoreData =
		scoreMgr->GetTrashGameScoreData();

	// 得点のテキストを生成
	mpScorePointText = new CTextUI2D();
	// テキストを設定
	mpScorePointText->ChangeToStr("%d\n", scoreData->mScore);
	// 座標を設定
	mpScorePointText->Position(SCORE_OFFSET_POS);
	// フォントサイズを設定
	mpScorePointText->SetFontSize(128);

	// ゴミ袋の数のテキストを生成
	mpNumText = new CTextUI2D();
	// テキストを設定
	mpNumText->ChangeToStr("%d\n　\n　\n%d\n", scoreData->mTrashBagNum, scoreData->mGoldTrashBagNum);
	// 座標を設定
	mpNumText->Position(NUM_OFFSET_POS);

	// ゴミ袋の値段のテキストを生成
	mpPriceText = new CTextUI2D();
	// テキストを設定
	mpPriceText->ChangeToStr("%d\n　\n　\n%d\n", trashBagPoint, goldTrashBagPoint);
	// 座標を設定
	mpPriceText->Position(PRICE_OFFSET_POS);

	// 単位のテキストを生成
	mpUnitText = new CTextUI2D();
	// テキストを設定
	mpUnitText->SetStr("コ　　　　　円\n　\n　\nコ　　　　　円\n");
	// 座標を設定
	mpUnitText->Position(UNIT_OFFSET_POS);

	// ×のテキストを生成
	mpXText= new CTextUI2D();
	// テキストを設定
	mpXText->SetStr("×\n　\n　\n×\n");
	// 座標を設定
	mpXText->Position(X_OFFSET_POS);

	mpTrashBagImg = new CImage
	(
		"UI/trashBag.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	// サイズの取得
	CVector2 trashBagSize = mpTrashBagImg->GetSize();
	// サイズの倍率を掛けた値を設定
	mpTrashBagImg->SetSize(trashBagSize * SIZE_RATE);
	// 座標を設定
	mpTrashBagImg->SetPos(TRASH_BAG_OFFEST_POS);

	mpGoldTrashBagImg = new CImage
	(
		"UI/gold_trashBag.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	// サイズの取得
	trashBagSize = mpGoldTrashBagImg->GetSize();
	// サイズの倍率を掛けた値を設定
	mpGoldTrashBagImg->SetSize(trashBagSize * SIZE_RATE);
	// 座標を設定
	mpGoldTrashBagImg->SetPos(GOLD_TRASH_BAG_OFFEST_POS);
}

CTrashGameResultUI::~CTrashGameResultUI()
{
	SAFE_DELETE(mpTrashBagImg);
	SAFE_DELETE(mpGoldTrashBagImg);
}

// 更新
void CTrashGameResultUI::Update()
{
	CResultUIBase::Update();
	mpTrashBagImg->Update();
	mpGoldTrashBagImg->Update();
}

// 描画
void CTrashGameResultUI::Render()
{
	CResultUIBase::Render();
	mpTrashBagImg->Render();
	mpGoldTrashBagImg->Render();
}
