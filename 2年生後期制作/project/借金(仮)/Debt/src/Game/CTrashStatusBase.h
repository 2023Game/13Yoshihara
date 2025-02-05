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

	// ゴミ袋の数を加算する
	void SetTrashBag(int num);
	// ゴミ袋の数を取得する
	int GetTrashBag() const;

	// ゴールドゴミ袋の数を加算する
	void SetGoldTrashBag(int num);
	// ゴールドゴミ袋の数を取得する
	int GetGoldTrashBag() const;

	/// <summary>
	/// ゴミ袋を落とす処理
	/// </summary>
	/// <param name="power">落とす力</param>
	/// <param name="pos">自分の座標</param>
	/// <param name="front">正面方向</param>
	/// <param name="side">横方向</param>
	/// <param name="offsetPos">ゴミ袋を落とすオフセット座標</param>
	void DropTrashBag(int power,
		CVector pos, CVector front, CVector side,
		CVector offsetPos);
private:
	float mKnockbackDealt;		// 与えるノックバック速度
	CVector mKnockbackReceived;	// 受けるノックバック
	int mPower;					// ゴミを落とす力

	// 持っているゴミ袋の数
	int mTrashBagNum;
	// 持っているゴールドゴミ袋の数
	int mGoldTrashBagNum;
};