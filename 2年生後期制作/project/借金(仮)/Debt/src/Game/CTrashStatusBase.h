#pragma once

class CTrashStatusBase
{
private:
	// ステータス
	struct StatusBase
	{
		int mHp;			// 現在HP
		float mMoveSpeed;	// 移動速度
		float mJumpSpeed;	// ジャンプ速度
		float mKnockback;	// ノックバック速度
		int mPower;			// ゴミを落とす力
	};
	// ステータス
	StatusBase mStatus;

	// 受けるノックバック速度
	float mTakeKnockback;

public:
	// コンストラクタ
	CTrashStatusBase();
	~CTrashStatusBase();

	// ダメージを1受ける
	void TakeDamage();

	// ノックバックの値を取得
	float GetTakeKnockback();
	// ノックバックを受ける
	void SetTakeKnockback(float knockback);

	// ベースステータスを取得する
	StatusBase GetStatusBase();
	// ベースステータスを設定する
	void SetStatusBase(int hp, float moveSpeed, float jumpSpeed, float knockback, int power);
};