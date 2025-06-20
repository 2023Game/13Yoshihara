#pragma once

class CCharaStatusBase
{
public:
	CCharaStatusBase(int maxHp, int maxMp,
		float baseMoveSpeed, float jumpSpeed);
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

	// 最大MPを取得
	int GetMaxMp() const;
	// 最大MPを設定
	void SetMaxMp(int maxMp);
	// 現在MPを取得
	int GetMp() const;
	// 現在MPを加減算する
	void SetMp(int num);
	// 現在MPをリセット
	void ResetMp();

	// 基礎移動速度を取得
	virtual float GetBaseMoveSpeed() const;
	// 基礎移動速度を設定
	void SetBaseMoveSpeed(float baseMoveSpeed);

	// ジャンプ速度を取得
	float GetJumpSpeed() const;
	// ジャンプ速度を設定
	void SetJumpSpeed(float jumpSpeed);

private:
	int mMaxHp;				// 最大HP
	int mHp;				// 現在HP
	int mMaxMp;				// 最大MP
	int mMp;				// 現在MP
	float mBaseMoveSpeed;	// 基礎移動速度
	float mJumpSpeed;		// ジャンプ速度
};