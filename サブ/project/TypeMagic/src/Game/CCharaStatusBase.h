#pragma once

class CCharaStatusBase
{
public:
	CCharaStatusBase(int maxHp, float baseMoveSpeed, 
		float jumpSpeed, int attackPower);
	~CCharaStatusBase();


	// ダメージを受ける
	virtual void TakeDamage(int damage, CObjectBase* attacker);

	// 死亡
	virtual void Death();
	// 死んでいるかどうか
	virtual bool IsDeath() const;

	// 最大HPを取得
	int GetMaxHp() const;
	// 最大HPを設定
	void SetMaxHp(int maxHp);
	// 現在HPを取得
	int GetHp() const;
	// 現在HPを加減算する
	void SetHp(int num);
	// 現在HPをリセット
	void ResetHp();


	// 基礎移動速度を取得
	virtual float GetBaseMoveSpeed() const;
	// 基礎移動速度を設定
	void SetBaseMoveSpeed(float baseMoveSpeed);

	// ジャンプ速度を取得
	float GetJumpSpeed() const;
	// ジャンプ速度を設定
	void SetJumpSpeed(float jumpSpeed);

	// 攻撃力を取得
	int GetAttackPower() const;
	// 攻撃力を設定
	void SetAttackPower(int attackPower);

private:
	int mMaxHp;				// 最大HP
	int mHp;				// 現在HP
	float mBaseMoveSpeed;	// 基礎移動速度
	float mJumpSpeed;		// ジャンプ速度
	int mAttackPower;		// 攻撃力
};