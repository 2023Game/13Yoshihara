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
#define DELIVERY_TRUCK_PATH "UI/deliveryTruck.png"

// 画像のサイズの倍率
#define SIZE_RATE 0.25f
// 配達物の画像のオフセット座標
#define DELIVERY_ITEM_OFFEST_POS	CVector2(WINDOW_WIDTH * 0.225f, WINDOW_HEIGHT * 0.38f)
// トラックの画像のオフセット座標
#define DELIVERY_TRUCK_OFFSET_POS	CVector2(WINDOW_WIDTH * 0.225f, WINDOW_HEIGHT * 0.59f)

// コンストラクタ
CDeliveryGameResultUI::CDeliveryGameResultUI()
	: CResultUIBase(BACKGROUND_PATH)
{
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

	// スコアを設定
	SetScore(totalScore);

	// 得点のテキストを設定
	mpScorePointText->ChangeToStr("%d円\n", totalScore);
	
	// 数値のテキスト1を設定
	mpNumText1->ChangeToStr("%d\n　\n　\n%d\n　\n　\n%d\n",
		scoreData->mDeliveryNum, 
		scoreData->mDestroyEnemyNum, 
		deliveryItemTotalScore + destroyEnemyTotalScore);
	
	// 数値のテキスト2を設定
	mpNumText2->ChangeToStr("%d\n　\n　\n%d\n　\n　\n%.2f\n",
		deliveryItemPoint,
		destroyEnemyPoint,
		bonus);
	
	// 計算結果のテキストを設定
	mpTotalText->ChangeToStr("%d\n　\n　\n%d\n　\n　\n%d\n",
		deliveryItemTotalScore,
		destroyEnemyTotalScore,
		bonusScore);
	
	// テキストを設定
	mpBonusDescriptionText->SetStr("命中率");
	
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

	// トラックの画像を生成
	mpDeliveryTruckImg = new CImage
	(
		DELIVERY_TRUCK_PATH,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	// サイズの取得
	CVector2 deliveryTruckSize = mpDeliveryTruckImg->GetSize();
	// サイズの倍率を掛けた値を設定
	mpDeliveryTruckImg->SetSize(deliveryTruckSize* SIZE_RATE);
	// 座標を設定
	mpDeliveryTruckImg->SetPos(DELIVERY_TRUCK_OFFSET_POS);
}

// デストラクタ
CDeliveryGameResultUI::~CDeliveryGameResultUI()
{
	SAFE_DELETE(mpDeliveryItemImg);
	SAFE_DELETE(mpDeliveryTruckImg);
}

// 更新
void CDeliveryGameResultUI::Update()
{
	CResultUIBase::Update();
	// 画像
	mpDeliveryItemImg->Update();
	mpDeliveryTruckImg->Update();
}

// 描画
void CDeliveryGameResultUI::Render()
{
	CResultUIBase::Render();
	// 画像
	mpDeliveryItemImg->Render();
	mpDeliveryTruckImg->Render();
}
