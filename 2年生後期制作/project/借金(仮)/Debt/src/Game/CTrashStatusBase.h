#pragma once
#include "CCharaStatusBase.h"

class CTrashStatusBase : CCharaStatusBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="mMaxHp">最大HP</param>
	/// <param name="moveSpeed">移動速度</param>
	/// <param name="jumpSpeed">ジャンプ速度</param>
	/// <param name="knockback">ノックバック速度</param>
	/// <param name="power">ゴミを落とす力</param>
	CTrashStatusBase(int mMaxHp, float moveSpeed, float jumpSpeed,
		float knockback, int power);
	~CTrashStatusBase();

	// 与えるノックバック速度を取得
	float GetKnockback() const;
	// 与えるノックバック速度を設定
	void SetKnockback(float knockback);

	// ゴミを落とす力を取得
	int GetPower() const;
	// ゴミを落とす力を設定
	void SetPower(int power);

	// 受けるノックバック速度を取得
	float GetTakeKnockback() const;
	// 受けるノックバック速度を設定
	void SetTakeKnockback(float knockback);
private:
	float mKnockback;		// ノックバック速度
	int mPower;				// ゴミを落とす力
	float mTakeKnockback;	// 受けるノックバック速度
};