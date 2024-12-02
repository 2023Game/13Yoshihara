#pragma once

class CTrashStatusBase
{
public:
	// コンストラクタ
	CTrashStatusBase();
	~CTrashStatusBase();

	// ダメージを1受ける
	void TakeDamage();
	// ノックバックを受ける
	void SetTakeKnockBack(float knockback);
protected:
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
};