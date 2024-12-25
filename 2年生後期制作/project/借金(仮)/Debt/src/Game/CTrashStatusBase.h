#pragma once
#include "CCharaStatusBase.h"

class CTrashStatusBase : public CCharaStatusBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="maxHp">			最大HP</param>
	/// <param name="baseMoveSpeed">	基礎移動速度</param>
	/// <param name="jumpSpeed">		ジャンプ速度</param>
	/// <param name="knockbackDealt">	与えるノックバック速度</param>
	/// <param name="power">			ゴミを落とす力</param>
	CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
		float knockbackDealt, int power);
	~CTrashStatusBase();

	// 与えるノックバック速度を取得
	float GetKnockbackDealt() const;

	// 受けるノックバック速度を取得
	float GetKnockbackReceived() const;
	// 受けるノックバック速度を設定
	void SetKnockbackReceived(float knockbackReceived);

	// ゴミを落とす力を取得
	int GetPower() const;
private:
	float mKnockbackDealt;		// 与えるノックバック速度
	float mKnockbackReceived;	// 受けるノックバック速度
	int mPower;					// ゴミを落とす力
};