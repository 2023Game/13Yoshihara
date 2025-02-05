#include "CTrashStatusBase.h"
#include "Maths.h"
#include "CTrashBag.h"

// コンストラクタ
CTrashStatusBase::CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
	float knockbackDealt, int attackPower, int power)
	: CCharaStatusBase(maxHp, baseMoveSpeed, jumpSpeed, attackPower)
	, mKnockbackDealt(knockbackDealt)
	, mPower(power)
	, mKnockbackReceived(CVector::zero)
	, mTrashBagNum(0)
	, mGoldTrashBagNum(0)
{
}

// デストラクタ
CTrashStatusBase::~CTrashStatusBase()
{
}

// 全てのステータスを設定
void CTrashStatusBase::SetAllStatus(int maxHp, float baseMoveSpeed, float jumpSpeed, float knockbackDealt, int attackPower, int power)
{
	SetMaxHp(maxHp);
	SetBaseMoveSpeed(baseMoveSpeed);
	SetJumpSpeed(jumpSpeed);
	SetKnockbackDealt(knockbackDealt);
	SetAttackPower(attackPower);
	SetPower(power);
}

// 与えるノックバック速度を取得
float CTrashStatusBase::GetKnockbackDealt() const
{
	return mKnockbackDealt;
}

// 与えるノックバック速度を設定
void CTrashStatusBase::SetKnockbackDealt(float knockbackDealt)
{
	mKnockbackDealt = knockbackDealt;
}

// 受けるノックバックを取得
CVector CTrashStatusBase::GetKnockbackReceived() const
{
	return mKnockbackReceived;
}

// 受けるノックバックを設定
void CTrashStatusBase::SetKnockbackReceived(CVector knockbackReceived)
{
	mKnockbackReceived = knockbackReceived;
}

// ゴミを落とす力を取得
int CTrashStatusBase::GetPower() const
{
	return mPower;
}

// ゴミを落とす力を設定
void CTrashStatusBase::SetPower(int power)
{
	mPower = power;
}

// ゴミ袋の数を加算する
void CTrashStatusBase::SetTrashBag(int num)
{
	mTrashBagNum += num;
	// 0より小さくなったら0にする
	if (mTrashBagNum < 0) mTrashBagNum = 0;
}

// ゴミ袋の数を取得する
int CTrashStatusBase::GetTrashBag() const
{
	return mTrashBagNum;
}

// ゴールドゴミ袋の数を加算する
void CTrashStatusBase::SetGoldTrashBag(int num)
{
	mGoldTrashBagNum += num;
	// 0より小さくなったら0にする
	if (mGoldTrashBagNum < 0) mGoldTrashBagNum = 0;
}

// ゴールドゴミ袋の数を取得する
int CTrashStatusBase::GetGoldTrashBag() const
{
	return mGoldTrashBagNum;
}

// ゴミ袋を落とす処理
void CTrashStatusBase::DropTrashBag(int power,
	CVector pos, CVector front, CVector side,
	CVector offsetPos)
{
	// ゴミ袋を落とす力が0以下になるまで繰り返す
	// ゴミ袋を落とすことが出来たらdropPowerを減らす
	for (int dropPower = power; dropPower > 0; dropPower)
	{
		// 両方のゴミ袋の所持数が0なら処理しない
		if (GetTrashBag() + GetGoldTrashBag() <= 0) return;
		// 落とす方を決定
		int random = Math::Rand(0, 1);
		// 片方が0なら、落とすのはもう一方確定
		if (GetTrashBag() <= 0) random = 1;
		if (GetGoldTrashBag() <= 0) random = 0;

		switch (random)
		{
		case 0:
			// ゴミ袋を一つでも所持していたら落とす
			if (GetTrashBag() > 0)
			{
				// Xのオフセット座標は範囲内でランダムに決める
				float randomX = Math::Rand(-offsetPos.X(), offsetPos.X());
				Math::Clamp(randomX, -offsetPos.X(), offsetPos.X());
				// 持っているゴミ袋の数を1減らす
				SetTrashBag(-1);
				// ゴミ袋を生成
				CTrashBag* trashBag = new CTrashBag(false);
				// （オフセット座標×落とす力）分ずらした位置に生成
				CVector offset = CVector(0.0f, offsetPos.Y(), 0.0f);
				offset += side * randomX;
				offset += front * offsetPos.Z();
				trashBag->Position(pos + offset * dropPower);
				// 飛んでいく速度を設定
				trashBag->SetThrowSpeed(front * GetKnockbackDealt(), GetKnockbackDealt());
				// 落とす力を1減らす
				dropPower--;
			}
			break;
		case 1:
			// ゴールドゴミ袋を一つでも所持していたら落とす
			if (GetGoldTrashBag() > 0)
			{
				// Xのオフセット座標は範囲内でランダムに決める
				float randomX = Math::Rand(-offsetPos.X(), offsetPos.X());
				// 持っているゴールドゴミ袋の数を1減らす
				SetGoldTrashBag(-1);
				// ゴミ袋を生成
				CTrashBag* trashBag = new CTrashBag(true);
				// （オフセット座標×落とす力）分ずらした位置に生成
				CVector offset = CVector(0.0f, offsetPos.Y(), 0.0f);
				offset += side * randomX;
				offset += front * offsetPos.Z();
				trashBag->Position(pos + offset * dropPower);
				// 飛んでいく速度を設定
				trashBag->SetThrowSpeed(front * GetKnockbackDealt(), GetKnockbackDealt());
				// 落とす力を1減らす
				dropPower--;
			}
			break;
		}
	}
}
