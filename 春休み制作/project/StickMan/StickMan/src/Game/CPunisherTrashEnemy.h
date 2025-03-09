#pragma once
#include "CTrashEnemy.h"

class CPunisherTrashEnemy : public CTrashEnemy
{
public:
	// コンストラクタ
	CPunisherTrashEnemy();
	// デストラクタ
	~CPunisherTrashEnemy();

private:
	// コライダーを作成する
	void CreateCol() override;
	// 巡回状態から他の状態へ移行する条件をチェック
	// お仕置き用はすぐ追いかける
	bool ChangePatrolToOther() override;
	// 追跡状態から他の状態へ移行する条件をチェック
	// お仕置き用は追いかけ続ける
	bool ChangeChaseToOther() override;
	// Hpゲージの更新
	void UpdateHpGauge() override;

};