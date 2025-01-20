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
	/// <param name="attackPower">		攻撃力</param>
	/// <param name="power">			ゴミを落とす力</param>
	CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
		float knockbackDealt, int attackPower, int power);
	~CTrashStatusBase();

	/// <summary>
	/// 全てのステータスを設定
	/// </summary>
	/// <param name="maxHp">			最大HP</param>
	/// <param name="baseMoveSpeed">	基礎移動速度</param>
	/// <param name="jumpSpeed">		ジャンプ速度</param>
	/// <param name="knockbackDealt">	与えるノックバック速度</param>
	/// <param name="attackPower">		攻撃力</param>
	/// <param name="power">			ゴミを落とす力</param>
	void SetAllStatus(int maxHp, float baseMoveSpeed, float jumpSpeed,
		float knockbackDealt, int attackPower, int power);

	// 与えるノックバック速度を取得
	float GetKnockbackDealt() const;
	// 与えるノックバック速度を設定
	void SetKnockbackDealt(float knockbackDealt);

	// 受けるノックバックを取得
	CVector GetKnockbackReceived() const;
	// 受けるノックバックを設定
	void SetKnockbackReceived(CVector knockbackReceived);

	// ゴミを落とす力を取得
	int GetPower() const;
	// ゴミを落とす力を設定
	void SetPower(int power);
private:
	float mKnockbackDealt;		// 与えるノックバック速度
	CVector mKnockbackReceived;	// 受けるノックバック
	int mPower;					// ゴミを落とす力
};