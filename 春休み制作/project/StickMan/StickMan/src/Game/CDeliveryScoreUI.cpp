#include "CDeliveryScoreUI.h"
#include "CDeliveryItemStatus.h"
#include "CDeliveryEnemyStatus.h"
#include "CDeliveryPlayer.h"

// コンストラクタ
CDeliveryScoreUI::CDeliveryScoreUI()
	: CScoreUIBase()
{
	// 配達物の得点取得用
	CDeliveryItemStatus* itemStatus = new CDeliveryItemStatus();
	// 配達物の得点を取得
	mDeliveryItemScore = itemStatus->GetScore();
	// 不要なので削除
	delete itemStatus;

	// 敵を壊した時の得点取得用
	CDeliveryEnemyStatus* enemyStatus = new CDeliveryEnemyStatus();
	// 敵を壊した時の得点を取得
	mDestroyEnemyScore = enemyStatus->GetScore();
	// 不要なので削除
	delete enemyStatus;
}

// デストラクタ
CDeliveryScoreUI::~CDeliveryScoreUI()
{
}

// 更新
void CDeliveryScoreUI::Update()
{
	CScoreUIBase::Update();
}

// スコアを計算して取得する
int CDeliveryScoreUI::GetScore() const
{
	// プレイヤーを取得
	CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(CDeliveryPlayer::Instance());
	// 配達した数を取得
	int deliveryNum = player->GetDeliveryNum();
	// 壊した敵の数を取得
	int destroyNum = player->GetDestroyEnemyNum();
	// 数と得点を掛けてスコアを計算
	int score = deliveryNum * mDeliveryItemScore;
	score += destroyNum * mDestroyEnemyScore;
	return score;
}

// 配達1個当たりの得点を取得
int CDeliveryScoreUI::GetDeliveryItemScore() const
{
	return mDeliveryItemScore;
}

// 敵を1体壊すと貰える得点を取得
int CDeliveryScoreUI::GetDestroyEnemyScore() const
{
	return mDestroyEnemyScore;
}
