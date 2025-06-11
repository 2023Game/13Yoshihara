#include "CSpellBase.h"
#include "CModel.h"
#include "CSpellCaster.h"

// コンストラクタ
CSpellBase::CSpellBase(ESpellElementalType elemental, ESpellShapeType shape,
	CObjectBase* owner, CObjectBase* target, float deleteTime)
	: CObjectBase(ETag::eSpell, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, CSpellStatus(elemental, shape)
	, mpModel(nullptr)
	, mpOwner(owner)
	, mpTarget(target)
	, mMoveDir(CVector::zero)
	, mDeleteTime(deleteTime)
	, mpSpellCol(nullptr)
{
	// 親についていく
	SetParent(owner);
}

// デストラクタ
CSpellBase::~CSpellBase()
{
	SAFE_DELETE(mpSpellCol);
}

// 更新
void CSpellBase::Update()
{
	switch (mState)
	{
	case CSpellBase::EState::eIdle:			UpdateIdle();		break;
	case CSpellBase::EState::eShooting:		UpdateShooting();	break;
	}

	// 移動
	Position(Position() + mMoveSpeed);
}

// 描画
void CSpellBase::Render()
{
	if (mpModel != nullptr)
	{
		mpModel->Render(Matrix());
	}
}

// 衝突処理
void CSpellBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpSpellCol)
	{
		// 相手が地形なら
		if (other->Layer() == ELayer::eGround ||
			other->Layer() == ELayer::eWall ||
			other->Layer() == ELayer::eObject)
		{
			// 削除
			Kill();
		}
		// 相手がプレイヤーなら
		else if (other->Layer() == ELayer::ePlayer)
		{
			// 削除
			Kill();

#if _DEBUG
			other->Owner()->AddHitCount();
#endif
		}
		// 相手が敵なら
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 削除
			Kill();
			
#if _DEBUG
			other->Owner()->AddHitCount();
#endif
		}
		// 相手が攻撃判定なら
		else if (other->Layer() == ELayer::eAttackCol)
		{
			CSpellBase* spell = dynamic_cast<CSpellBase*>(other->Owner());
			// 呪文の持ち主が違うなら
			if (spell->mpOwner != mpOwner)
			{
				// 削除
				TakeDamage(spell->GetSpellStatus().power, spell);
			}
		}
		// 相手が防御判定なら
		else if (other->Layer() == ELayer::eDefenseCol)
		{
			CSpellBase* spell = dynamic_cast<CSpellBase*>(other->Owner());
			// 呪文の持ち主が違うなら
			if (spell->mpOwner != mpOwner)
			{
				// 削除
				TakeDamage(spell->GetSpellStatus().power, spell);
			}
		}
	}
}

// 呪文のターゲットを設定
void CSpellBase::SetTarget(CObjectBase* target)
{
	mpTarget = target;
}

// 呪文の持ち主を設定
void CSpellBase::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// 発射をリスタート
void CSpellBase::Restart()
{
	mState = EState::eShooting;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// 待機中の更新
void CSpellBase::UpdateIdle()
{
}

// 発射中の更新
void CSpellBase::UpdateShooting()
{
	mElapsedTime += Times::DeltaTime();
	// 消滅するまでの時間が経過したら削除
	if (mElapsedTime > mDeleteTime)
	{
		Kill();
	}
}

// 目標への方向を求める
CVector CSpellBase::TargetDir()
{	
	// 目標への方向
	return mpTarget->Position() - Position();
}

// 耐久を減らす(最初から0の呪文はダメージで破棄されない)
void CSpellBase::TakeDamage(int num, CSpellBase* attacker)
{
	// 耐久が0なら処理しない
	if (mSpellStatus.hp == 0) return;

	// 相手のステータス
	SpellStatus status = attacker->GetSpellStatus();

	// 相手の形
	switch (status.shape)
	{
		// リフレクターの場合反射
	case ESpellShapeType::eReflector:
	{
		// 詠唱クラスを取得
		CSpellCaster* caster = dynamic_cast<CSpellCaster*>(mpOwner);
		// 目標を対戦相手に設定
		attacker->SetTarget(caster->GetOpponent());
		// 持ち主を設定
		attacker->SetOwner(mpOwner);
		// 攻撃力を強化
		attacker->AddPower(REFLECTOR_POWER_UP);
		// 耐久を強化
		attacker->AddHp(REFLECTOR_HP_UP);
		// 発射をリスタート
		Restart();
		break;
	}
		// それ以外
	default:
		// 属性倍率を適用
		int damage = num * PowerRatio(mSpellStatus.elemental, status.elemental);
		// ダメージを減算
		mSpellStatus.hp -= damage;
		// 耐久が0以下なら
		if (mSpellStatus.hp <= 0)
		{
			mSpellStatus.hp = 0;
			// 削除
			Kill();
		}
		break;
	}
}

// 状態を切り替え
void CSpellBase::ChangeState(EState state)
{
	if (mState == state) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;

	// コライダーがあるなら
	if (mpSpellCol != nullptr)
	{
		switch (mState)
		{
			// 待機時はコライダーは無効で
			// 親についていく
		case CSpellBase::EState::eIdle:
			mpSpellCol->SetEnable(false);
			SetParent(mpOwner);
			break;
			// 発射時は有効で
			// 親についていかない
		case CSpellBase::EState::eShooting:
			mpSpellCol->SetEnable(true);
			SetParent(nullptr);
			break;
		}
	}
}
