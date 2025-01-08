#pragma once

class CCharaStatusBase
{
public:
	CCharaStatusBase(int maxHp, float baseMoveSpeed, 
		float jumpSpeed, int attackPower);
	~CCharaStatusBase();


	// ダメージを受ける
	virtual void TakeDamage(int damage, CObjectBase* causer);

	// 死亡
	virtual void Death();
	// 死んでいるかどうか
	bool IsDeath() const;

	// 最大HPを取得
	int GetMaxHp() const;
	// 現在HPを取得
	int GetHp() const;

	// 基礎移動速度を取得
	float GetBaseMoveSpeed() const;

	// ジャンプ速度を取得
	float GetJumpSpeed() const;

	// 攻撃力を取得
	int GetAttackPower() const;

private:
	int mMaxHp;				// 最大HP
	int mHp;				// 現在HP
	float mBaseMoveSpeed;	// 基礎移動速度
	float mJumpSpeed;		// ジャンプ速度
	int mAttackPower;		// 攻撃力
};