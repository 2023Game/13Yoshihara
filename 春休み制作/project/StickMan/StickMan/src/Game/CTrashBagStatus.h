#pragma once

// ゴミ袋のステータス
class CTrashBagStatus
{
public:
	// コンストラクタ
	CTrashBagStatus(bool gold);
	// デストラクタ
	~CTrashBagStatus();

	// ゴールドかどうかを取得する
	bool GetIsGold() const;
	// ゴールドかどうかを設定する
	void SetIsGold(bool isGold);

	// 消滅までの時間を取得する
	float GetDeleteTime() const;
	// 消滅までの時間を初期値に設定する
	void SetDeleteTime();
	// 消滅までの時間が経過したかどうか
	bool IsElapsedDeleteTime() const;
	// 消滅までの時間をカウントダウン
	void CountDeleteTime();

	// 得点を取得する
	int GetPoint(bool gold) const;

private:
	// ゴールド袋かどうか
	bool mIsGold;
	// 消滅までの時間
	float mDeleteTime;
};