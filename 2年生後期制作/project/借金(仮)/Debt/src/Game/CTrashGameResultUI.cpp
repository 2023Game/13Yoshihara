#include "CTrashGameResultUI.h"
#include "CText.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CScoreManager.h"
#include "CTrashBagStatus.h"
#include "CExpandButton.h"

// 背景画像のパス
#define BACKGROUND_PATH "UI/trash_game_result_bg.png"
// 画像のパス
#define BAG_PATH		"UI/trashBag.png"
#define GOLD_BAG_PATH	"UI/gold_trashBag.png"

// ゴミ袋の画像のサイズの倍率
#define SIZE_RATE 0.25f
// ゴミ袋の画像のオフセット座標
#define TRASH_BAG_OFFEST_POS		CVector2(WINDOW_WIDTH * 0.225f, WINDOW_HEIGHT * 0.38f)
// ゴールドのゴミ袋の画像のオフセット座標
#define GOLD_TRASH_BAG_OFFEST_POS	CVector2(WINDOW_WIDTH * 0.225f, WINDOW_HEIGHT * 0.56f)


CTrashGameResultUI::CTrashGameResultUI()
	: CResultUIBase(BACKGROUND_PATH)
{
	// ゴミ袋の得点取得用
	CTrashBagStatus* trashBagStatus = new CTrashBagStatus(false);
	// ゴミ袋の得点を取得
	int trashBagPoint = trashBagStatus->GetPoint(false);
	int goldTrashBagPoint = trashBagStatus->GetPoint(true);
	// 不要なので削除
	delete trashBagStatus;

	// 得点管理クラスを取得
	CScoreManager* scoreMgr = CScoreManager::Instance();
	// ゴミ拾いのスコアデータを取得
	CScoreManager::TrashGameScoreData* scoreData =
		scoreMgr->GetTrashGameScoreData();

	// それぞれのゴミ袋のスコアの合計を計算
	int trashBagTotalScore = 
		scoreData->mTrashBagNum * trashBagPoint;
	int goldBagTotalScore =
		scoreData->mGoldTrashBagNum * goldTrashBagPoint;
	// 全てのスコアの合計
	int totalScore = trashBagTotalScore + goldBagTotalScore;
	// ボーナスの倍率
	float bonus = scoreData->mHpPer;
	// ボーナスのスコアを計算
	int bonusScore = totalScore * bonus;
	// 合計スコアに加算する
	totalScore += bonusScore;


	// 得点のテキストを設定
	mpScorePointText->ChangeToStr("%d円\n", totalScore);

	// 数値のテキスト1を設定
	mpNumText1->ChangeToStr("%d\n　\n　\n%d\n　\n　\n%d\n",
		scoreData->mTrashBagNum, 
		scoreData->mGoldTrashBagNum, 
		trashBagTotalScore + goldBagTotalScore);
	
	// 数値のテキスト2を設定
	mpNumText2->ChangeToStr("%d\n　\n　\n%d\n　\n　\n%.1f\n",
		trashBagPoint,
		goldTrashBagPoint,
		bonus);
	
	// 計算結果のテキストを設定
	mpTotalText->ChangeToStr("%d\n　\n　\n%d\n　\n　\n%d\n",
		trashBagTotalScore,
		goldBagTotalScore,
		bonusScore);
	
	// ボーナスのテキストを設定
	mpBonusDescriptionText->SetStr("残HP割合");

	// ゴミ袋の画像を生成
	mpTrashBagImg = new CImage
	(
		BAG_PATH,
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

	// ゴールドゴミ袋の画像を生成
	mpGoldTrashBagImg = new CImage
	(
		GOLD_BAG_PATH,
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
	// 画像
	mpTrashBagImg->Update();
	mpGoldTrashBagImg->Update();
}

// 描画
void CTrashGameResultUI::Render()
{
	CResultUIBase::Render();
	// 画像
	mpTrashBagImg->Render();
	mpGoldTrashBagImg->Render();
}
