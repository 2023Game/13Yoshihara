#include "CDeliveryGameResultUI.h"
#include "CText.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CScoreManager.h"
#include "CDeliveryItemStatus.h"
#include "CDeliveryEnemyStatus.h"
#include "CExpandButton.h"

// 背景画像のパス
#define BACKGROUND_PATH "UI/delivery_game_result_bg.png"
// 画像のパス
#define DELIVERY_ITEM_PATH "UI/deliveryItem.png"

// 得点のテキストのオフセット座標
#define SCORE_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.0f,   WINDOW_HEIGHT * 0.2f)
// ゴミ袋の数のテキストのオフセット座標
#define NUM_OFFSET_POS					CVector2(WINDOW_WIDTH * 0.3f,   WINDOW_HEIGHT * 0.4f)
// ゴミ袋の値段のテキストのオフセット座標
#define PRICE_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.475f, WINDOW_HEIGHT * 0.4f)
// ゴミ袋の値段のテキストのオフセット座標
#define TOTAL_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.7f,   WINDOW_HEIGHT * 0.4f)
// 単位のテキストのオフセット座標
#define UNIT_OFFSET_POS					CVector2(WINDOW_WIDTH * 0.35f,  WINDOW_HEIGHT * 0.4f)
// 記号のテキストのオフセット座標
#define SYMBOL_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.42f,  WINDOW_HEIGHT * 0.4f)
// 一番下の計算の説明テキストのオフセット座標
#define DESCRIPTION_OFFSET_POS			CVector2(WINDOW_WIDTH * 0.15f, WINDOW_HEIGHT * 0.76f)
// ボーナスの説明テキストのオフセット座標
#define BONUS_DESCRIPTION_OFFSET_POS	CVector2(WINDOW_WIDTH * 0.475f, WINDOW_HEIGHT * 0.83f)

// 画像のサイズの倍率
#define SIZE_RATE 0.25f
// 配達物の画像のオフセット座標
#define DELIVERY_ITEM_OFFEST_POS	CVector2(WINDOW_WIDTH * 0.225f, WINDOW_HEIGHT * 0.38f)
// ゴールドのゴミ袋の画像のオフセット座標
#define GOLD_TRASH_BAG_OFFEST_POS	CVector2(WINDOW_WIDTH * 0.225f, WINDOW_HEIGHT * 0.56f)


// ボタンのオフセット座標
#define BUTTON_OFFSET_POS CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.25f)

// コンストラクタ
CDeliveryGameResultUI::CDeliveryGameResultUI()
	: CResultUIBase(BACKGROUND_PATH)
{
	// ボタンの座標を設定
	mpReturnButton->SetPos(BUTTON_OFFSET_POS);
	// 配達物の得点取得用
	CDeliveryItemStatus* itemStatus = new CDeliveryItemStatus();
	// 配達物の得点を取得
	int deliveryItemPoint = itemStatus->GetScore();
	// 不要なので削除
	delete itemStatus;

	// 敵を壊した時の得点取得用
	CDeliveryEnemyStatus* enemyStatus = new CDeliveryEnemyStatus();
	// 敵を壊した時の得点を取得
	int destroyEnemyPoint = enemyStatus->GetScore();
	// 不要なので削除
	delete enemyStatus;

	// 得点管理クラスを取得
	CScoreManager* scoreMgr = CScoreManager::Instance();
	// 配達のスコアデータを取得
	CScoreManager::DeliveryGameScoreData* scoreData =
		scoreMgr->GetDeliveryGameScoreData();

	// 配達したスコアの合計
	int deliveryItemTotalScore =
		scoreData->mDeliveryNum * deliveryItemPoint;
	// 壊した敵のスコアの合計
	int destroyEnemyTotalScore =
		scoreData->mDestroyEnemyNum * destroyEnemyPoint;
	// 全てのスコアの合計
	int totalScore = deliveryItemTotalScore + destroyEnemyTotalScore;
	// ボーナスの倍率
	float bonus = scoreData->mAccuracy;
	// ボーナスのスコアを計算
	int bonusScore = totalScore * bonus;
	// 合計スコアに加算する
	totalScore += bonusScore;

	// 得点のテキストを生成
	mpScorePointText = new CTextUI2D(ETaskPauseType::eGame, false);
	// テキストを設定
	mpScorePointText->ChangeToStr("%d円\n", totalScore);
	// 座標を設定
	mpScorePointText->Position(SCORE_OFFSET_POS);
	// フォントサイズを設定
	mpScorePointText->SetFontSize(128);
	// 文字の揃いの基準を設定
	mpScorePointText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);

	// 数値のテキスト1を生成
	mpNumText1 = new CTextUI2D(ETaskPauseType::eGame, false);
	// テキストを設定
	mpNumText1->ChangeToStr("%d\n　\n　\n%d\n　\n　\n%d\n",
		scoreData->mDeliveryNum, 
		scoreData->mDestroyEnemyNum, 
		deliveryItemTotalScore + destroyEnemyTotalScore);
	// 座標を設定
	mpNumText1->Position(NUM_OFFSET_POS);

	// 数値のテキスト2を生成
	mpNumText2 = new CTextUI2D(ETaskPauseType::eGame, false);
	// テキストを設定
	mpNumText2->ChangeToStr("%d\n　\n　\n%d\n　\n　\n%.2f\n",
		deliveryItemPoint,
		destroyEnemyPoint,
		bonus);
	// 座標を設定
	mpNumText2->Position(PRICE_OFFSET_POS);

	// 計算結果のテキストを生成
	mpTotalText = new CTextUI2D(ETaskPauseType::eGame, false);
	// テキストを設定
	mpTotalText->ChangeToStr("%d\n　\n　\n%d\n　\n　\n%d\n",
		deliveryItemTotalScore,
		destroyEnemyTotalScore,
		bonusScore);
	// 座標を設定
	mpTotalText->Position(TOTAL_OFFSET_POS);

	// 単位のテキストを生成
	mpUnitText = new CTextUI2D(ETaskPauseType::eGame, false);
	// テキストを設定
	mpUnitText->SetStr("コ　　　　　　円\n　\n　\nコ　　　　　　円\n");
	// 座標を設定
	mpUnitText->Position(UNIT_OFFSET_POS);

	// ×のテキストを生成
	mpSymbolText = new CTextUI2D(ETaskPauseType::eGame, false);
	// テキストを設定
	mpSymbolText->SetStr("×　　　　　　＝\n　\n　\n×　　　　　　＝\n　\n　\n×　　　　　　＝\n");
	mpSymbolText->SetFontAligment(FTGL::TextAlignment::ALIGN_LEFT);
	// 座標を設定
	mpSymbolText->Position(SYMBOL_OFFSET_POS);

	// 一番下の計算の説明テキスト
	mpDescriptionText = new CTextUI2D(ETaskPauseType::eGame, false);
	// テキストを設定
	mpDescriptionText->SetStr("ボーナス！！");
	// 座標を設定
	mpDescriptionText->Position(DESCRIPTION_OFFSET_POS);
	// 文字の色を設定
	mpDescriptionText->SetFontColor(CColor(0.9f, 0.0f, 0.0f));

	// ボーナスの説明テキスト
	mpBonusDescriptionText = new CTextUI2D(ETaskPauseType::eGame, false, nullptr);
	// テキストを設定
	mpBonusDescriptionText->SetStr("命中率");
	// フォントサイズを設定
	mpBonusDescriptionText->SetFontSize(32);
	// 座標を設定
	mpBonusDescriptionText->Position(BONUS_DESCRIPTION_OFFSET_POS);

	// 配達物の画像を生成
	mpDeliveryItemImg = new CImage
	(
		DELIVERY_ITEM_PATH,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	// サイズの取得
	CVector2 deliveryItemSize = mpDeliveryItemImg->GetSize();
	// サイズの倍率を掛けた値を設定
	mpDeliveryItemImg->SetSize(deliveryItemSize* SIZE_RATE);
	// 座標を設定
	mpDeliveryItemImg->SetPos(DELIVERY_ITEM_OFFEST_POS);
}

// デストラクタ
CDeliveryGameResultUI::~CDeliveryGameResultUI()
{
	SAFE_DELETE(mpScorePointText);
	SAFE_DELETE(mpNumText1);
	SAFE_DELETE(mpNumText2);
	SAFE_DELETE(mpUnitText);
	SAFE_DELETE(mpSymbolText);
	SAFE_DELETE(mpDescriptionText);
	SAFE_DELETE(mpBonusDescriptionText);

	SAFE_DELETE(mpDeliveryItemImg);
}

// 更新
void CDeliveryGameResultUI::Update()
{
	CResultUIBase::Update();
	// テキスト
	mpScorePointText->Update();
	mpNumText1->Update();
	mpNumText2->Update();
	mpTotalText->Update();
	mpUnitText->Update();
	mpSymbolText->Update();
	mpDescriptionText->Update();
	mpBonusDescriptionText->Update();
	// 画像
	mpDeliveryItemImg->Update();
}

// 描画
void CDeliveryGameResultUI::Render()
{
	CResultUIBase::Render();
	// テキスト
	mpScorePointText->Render();
	mpNumText1->Render();
	mpNumText2->Render();
	mpTotalText->Render();
	mpUnitText->Render();
	mpSymbolText->Render();
	mpDescriptionText->Render();
	mpBonusDescriptionText->Render();
	// 画像
	mpDeliveryItemImg->Render();
}
