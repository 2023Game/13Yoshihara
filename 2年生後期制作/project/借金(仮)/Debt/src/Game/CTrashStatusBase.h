#pragma once

class CTrashStatusBase
{
public:
	// コンストラクタ
	CTrashStatusBase();
	~CTrashStatusBase();

	// ダメージを1受ける
	void TakeDamage();

	// 現在HPを取得
	int GetHp() const;
	// HPを設定
	void SetHp(int hp);

	// 移動速度を取得
	float GetBaseMoveSpeed() const;
	// 移動速度を設定
	void SetBaseMoveSpeed(float baseMoveSpeed);

	// ジャンプ速度を取得
	float GetJumpSpeed() const;
	// ジャンプ速度を設定
	void SetJumpSpeed(float jumpSpeed);

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
	int mHp;				// 現在HP
	float mBaseMoveSpeed;	// 基礎移動速度
	float mJumpSpeed;		// ジャンプ速度
	float mKnockback;		// ノックバック速度
	int mPower;				// ゴミを落とす力
	float mTakeKnockback;	// 受けるノックバック速度
};