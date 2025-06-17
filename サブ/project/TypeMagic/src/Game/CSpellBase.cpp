#include "CSpellBase.h"
#include "CModel.h"
#include "CSpellCaster.h"
#include "CDamageUI3D.h"
#include "CPlayer.h"

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
			// 持ち主なら処理しない
			if (mpOwner == other->Owner()) return;
			// 既にヒットしていたら処理しない
			if (IsAttackHitObj(other->Owner())) return;

			// ダメージ表記を生成
			DamageText(mSpellStatus.power, mSpellStatus.elemental, this);

			// 攻撃がヒットしたオブジェクトに追加
			AddAttackHitObj(other->Owner());

#if _DEBUG
			other->Owner()->AddHitCount();
#endif

			// テレポート呪文なら処理しない
			if (mSpellStatus.shape == ESpellShapeType::eTeleport) return;
			// 削除
			Kill();
		}
		// 相手が敵なら
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 持ち主なら処理しない
			if (mpOwner == other->Owner()) return;
			// 既にヒットしていたら処理しない
			if (IsAttackHitObj(other->Owner())) return;

			// ダメージ表記を生成
			DamageText(mSpellStatus.power, mSpellStatus.elemental, this);

			// 攻撃がヒットしたオブジェクトに追加
			AddAttackHitObj(other->Owner());
			
#if _DEBUG
			other->Owner()->AddHitCount();
#endif

			// テレポート呪文なら処理しない
			if (mSpellStatus.shape == ESpellShapeType::eTeleport) return;
			// 削除
			Kill();
		}
		// 相手が攻撃判定なら
		else if (other->Layer() == ELayer::eAttackCol)
		{
			CSpellBase* spell = dynamic_cast<CSpellBase*>(other->Owner());
			// 呪文の持ち主が違うかつまだヒットしていないなら
			if (spell->mpOwner != mpOwner &&
				!IsAttackHitObj(other->Owner()))
			{
				// ダメージを与える
				spell->TakeDamage(mSpellStatus.power, this);
				// 攻撃がヒットしたオブジェクトに追加
				AddAttackHitObj(other->Owner());
			}
		}
		// 相手が防御判定なら
		else if (other->Layer() == ELayer::eDefenseCol)
		{
			CSpellBase* spell = dynamic_cast<CSpellBase*>(other->Owner());
			// 呪文の持ち主が違うかつまだヒットしていないなら
			if (spell->mpOwner != mpOwner &&
				!IsAttackHitObj(other->Owner()))
			{
				// ダメージを与える
				spell->TakeDamage(mSpellStatus.power, this);
				// 攻撃がヒットしたオブジェクトに追加
				AddAttackHitObj(other->Owner());
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

// 耐久を減らす(最初から耐久0の呪文はダメージで破棄されない)
void CSpellBase::TakeDamage(int num, CSpellBase* attacker)
{
	// 耐久が0なら処理しない
	if (mSpellStatus.hp == 0) return;
	// 攻撃力が0以下なら処理しない
	if (num <= 0) return;

	// 攻撃者のステータス
	SpellStatus attackerStatus = attacker->GetSpellStatus();
	// ダメージ
	int damage = num;

	// 自身の形
	switch (mSpellStatus.shape)
	{
		// リフレクターの場合反射
	case ESpellShapeType::eReflector:
	{
		// 弱点なら
		if (ELEMENTAL_POWER_RATIO <= ElementalPowerRatio(attackerStatus.elemental, mSpellStatus.elemental))
		{
			// 削除
			Kill();
			return;
		}
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
		attacker->Restart();
		break;
	}
		// シールドの場合消滅
	case ESpellShapeType::eShield:
		// 攻撃者を削除
		attacker->Kill();
		break;
		// それ以外の場合
	default:
		break;
	}

	// 属性倍率を適用
	damage *= ElementalPowerRatio(attackerStatus.elemental, mSpellStatus.elemental);
	// HPを減算
	mSpellStatus.hp -= damage;
	// 耐久が0以下なら
	if (mSpellStatus.hp <= 0)
	{
		mSpellStatus.hp = 0;
		// 削除
		Kill();
	}
	// ダメージ表記を生成
	DamageText(damage, attackerStatus.elemental, attacker);
}

// 属性による攻撃力倍率を計算する
float CSpellBase::ElementalPowerRatio(ESpellElementalType attacker, ESpellElementalType target)
{
	// 倍率
	float ratio = 1.0f;

	switch (attacker)
	{
		// 攻撃が炎
	case ESpellElementalType::eFire:
		// 弱点
		// 相手が風なら
		if (target == ESpellElementalType::eWind)
		{
			ratio *= ELEMENTAL_POWER_RATIO;
		}

		// いまひとつ
		// 相手が水なら
		if (target == ESpellElementalType::eWater)
		{
			ratio /= ELEMENTAL_POWER_RATIO;
		}
		break;

		// 攻撃が風
	case ESpellElementalType::eWind:
		// 弱点
		// 相手が地なら
		if (target == ESpellElementalType::eEarth)
		{
			ratio *= ELEMENTAL_POWER_RATIO;
		}

		// いまひとつ
		// 相手が炎なら
		if (target == ESpellElementalType::eFire)
		{
			ratio /= ELEMENTAL_POWER_RATIO;
		}
		break;

		// 攻撃が地
	case ESpellElementalType::eEarth:
		// 弱点
		// 相手が雷なら
		if (target == ESpellElementalType::eThunder)
		{
			ratio *= ELEMENTAL_POWER_RATIO;
		}

		// いまひとつ
		// 相手が風なら
		if (target == ESpellElementalType::eWind)
		{
			ratio /= ELEMENTAL_POWER_RATIO;
		}
		break;
		
		// 攻撃が雷
	case ESpellElementalType::eThunder:
		// 弱点
		// 相手が水なら
		if (target == ESpellElementalType::eWater)
		{
			ratio *= ELEMENTAL_POWER_RATIO;
		}

		// いまひとつ
		// 相手が地なら
		if (target == ESpellElementalType::eEarth)
		{
			ratio /= ELEMENTAL_POWER_RATIO;
		}
		break;

		// 攻撃が水
	case ESpellElementalType::eWater:
		// 弱点属性
		// 相手が炎なら
		if (target == ESpellElementalType::eFire)
		{
			ratio *= ELEMENTAL_POWER_RATIO;
		}

		// いまひとつ
		// 相手が雷なら
		if (target == ESpellElementalType::eThunder)
		{
			ratio /= ELEMENTAL_POWER_RATIO;
		}
		break;
	}

	return ratio;
}

// 属性によってカラーを返す
CColor CSpellBase::ElementalColor(ESpellElementalType elemental)
{
	CColor color;
	switch (elemental)
	{
	case ESpellElementalType::eFire:
		color = FIRE_COLOR;
		break;
	case ESpellElementalType::eWind:
		color = WIND_COLOR;
		break;
	case ESpellElementalType::eEarth:
		color = EARTH_COLOR;
		break;
	case ESpellElementalType::eThunder:
		color = THUNDER_COLOR;
		break;
	case ESpellElementalType::eWater:
		color = WATER_COLOR;
		break;
	case ESpellElementalType::eNeutral:
		color = NEUTRAL_COLOR;
		break;
	}
	return color;
}

// ダメージ表記を生成
void CSpellBase::DamageText(int damage, ESpellElementalType elemental,
	CSpellBase* attacker)
{	
	// 攻撃側のダメージ表記を生成
	CDamageUI3D* text = new CDamageUI3D(Position());
	// ダメージの数値
	text->ChangeToStr("%d\n", damage);;
	// 属性ごとの色を文字色に設定
	text->SetFontColor(ElementalColor(elemental));
	CColor color;
	// 持ち主がプレイヤーなら
	if (attacker->mpOwner == CPlayer::Instance())
	{
		color = PLAYER_COLOR;
	}
	// 敵なら
	else
	{
		color = ENEMY_COLOR;
	}
	// アウトラインの色を設定
	text->SetOutLineColor(color);
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
