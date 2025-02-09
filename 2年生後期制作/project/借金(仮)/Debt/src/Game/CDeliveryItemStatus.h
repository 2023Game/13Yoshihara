#pragma once

// 配達物のステータスクラス
class CDeliveryItemStatus
{
public:
	// コンストラクタ
	CDeliveryItemStatus();
	// デストラクタ
	~CDeliveryItemStatus();

	// 一つ当たりの得点を取得
	int GetScore() const;
	// 与えるダメージを取得
	int GetDamage() const;

private:
	// 一つ当たりの得点
	int mScore;
	// 与えるダメージ
	int mDamage;
};