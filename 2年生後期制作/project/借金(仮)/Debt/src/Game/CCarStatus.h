#pragma once
#include "CTrashStatusBase.h"

class CCarStatus : public CTrashStatusBase
{
public:
	// コンストラクタ
	CCarStatus();
	/// <summary>
	/// コンストラクタ
	/// このクラスを継承したキャラのステータスを設定する用
	/// </summary>
	/// <param name="maxHp">最大HP</param>
	/// <param name="baseMoveSpeed">基礎移動速度</param>
	/// <param name="jumpSpeed">ジャンプ速度</param>
	/// <param name="knockback">ノックバック速度</param>
	/// <param name="power">ゴミを落とす力</param>
	CCarStatus(int maxHp, float baseMoveSpeed,
		float jumpSpeed, float knockback, int power);
	~CCarStatus();

	// 消滅までの時間を取得する
	float GetDeleteTime() const;
	// 消滅までの時間を設定する
	void SetDeleteTime(float deleteTime);
	// 消滅までの時間が経過したかどうか
	bool IsElapsedDeleteTime() const;
	// 消滅までの時間をカウント
	void CountDeleteTime();
private:
	float mDeleteTime;	// 消滅までの時間
};