#include "CPlayer.h"
#include "CColliderCapsule.h"
#include "CInput.h"

#define BODY_RADIUS 2.5f
#define BODY_HEIGHT 12.5f

// アニメーションのパス
#define ANIM_PATH "Character\\Player\\anim\\"

// アニメーションデータのテーブル
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f},	// Tポーズ
	{ ANIM_PATH"idle.x",		true,	153.0f,	1.0f	},	// 待機
	{ ANIM_PATH"walk.x",		true,	66.0f,	1.0f	},	// 歩行
	{ ANIM_PATH"attack.x",		false,	92.0f,	1.0f	},	// 斬り攻撃
	{ ANIM_PATH"kick.x",		false,	74.0f,	1.0f	},	// 蹴り攻撃
	{ ANIM_PATH"jump_start.x",	false,	25.0f,	1.0f	},	// ジャンプ開始
	{ ANIM_PATH"jump.x",		true,	1.0f,	1.0f	},	// ジャンプ中
	{ ANIM_PATH"jump_end.x",	false,	26.0f,	1.0f	},	// ジャンプ終了
	{ ANIM_PATH"hit.x",			false,	44.0f,	1.0f	},	// 仰け反り
	//{ ANIM_PATH"Jump.x",		true,	0.0f,	1.0f},	// ジャンプ
	//// 攻撃
	//{ ANIM_PATH"AttackW_1.x",	false,	0.0f,	1.0f},	// W
	//{ ANIM_PATH"AttackA_1.x",	false,	0.0f,	1.0f},	// A
	//{ ANIM_PATH"AttackS_1.x",	false,	0.0f,	1.0f},	// S
	//{ ANIM_PATH"AttackD_1.x",	false,	0.0f,	1.0f},	// D
	//// 被弾
	//{ ANIM_PATH"DamageW_1.x",	false,	0.0f,	1.0f},	// W
	//{ ANIM_PATH"DamageA_1.x",	false,	0.0f,	1.0f},	// A
	//{ ANIM_PATH"DamageS_1.x",	false,	0.0f,	1.0f},	// S
	//{ ANIM_PATH"DamageD_1.x",	false,	0.0f,	1.0f},	// D
	//// 回避
	//{ ANIM_PATH"DodgeW_1.x",	false,	0.0f,	1.0f},	// W
	//{ ANIM_PATH"DodgeA_1.x",	false,	0.0f,	1.0f},	// A
	//{ ANIM_PATH"DodgeS_1.x",	false,	0.0f,	1.0f},	// S
	//{ ANIM_PATH"DodgeD_1.x",	false,	0.0f,	1.0f},	// D
	//// パリィ
	//{ ANIM_PATH"ParryW_1.x",	false,	0.0f,	1.0f},	// W
	//{ ANIM_PATH"ParryA_1.x",	false,	0.0f,	1.0f},	// A
	//{ ANIM_PATH"ParryS_1.x",	false,	0.0f,	1.0f},	// S
	//{ ANIM_PATH"ParryD_1.x",	false,	0.0f,	1.0f},	// D
};

// コンストラクタ
CPlayer::CPlayer()
	: CPlayerBase()
	, CPlayerStatus()
	, mState(EState::eIdle)
{
	// アニメーションとモデルの初期化
	InitAnimationModel("Player", &ANIM_DATA);
	
	// コライダ―を生成
	CreateCol();

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);
}

// デストラクタ
CPlayer::~CPlayer()
{
}

// 攻撃中か
bool CPlayer::IsAttacking() const
{
	return false;
}

// 攻撃開始
void CPlayer::AttackStart()
{
}

// 攻撃終了
void CPlayer::AttackEnd()
{
}

// ダメージを受ける
void CPlayer::TakeDamage(int damage, CObjectBase* causer)
{
}

// 更新
void CPlayer::Update()
{
	switch (mState)
	{
	case EState::eIdle:			UpdateIdle();			break;
	case EState::eDamageStart:	UpdateDamageStart();	break;
	case EState::eDamage:		UpdateDamage();			break;
	case EState::eDamageEnd:	UpdateDamageEnd();		break;
	case EState::eAttackStart:	UpdateAttackStart();	break;
	case EState::eAttack:		UpdateAttack();			break;
	case EState::eAttackEnd:	UpdateAttackEnd();		break;
	case EState::eDodgeStart:	UpdateDodgeStart();		break;
	case EState::eDodge:		UpdateDodge();			break;
	case EState::eDodgeEnd:		UpdateDamageEnd();		break;
	case EState::eParryStart:	UpdateParryStart();		break;
	case EState::eParry:		UpdateParry();			break;
	case EState::eParryEnd:		UpdateParryEnd();		break;
	case EState::eDeath:		UpdateDeath();			break;
	}

	// 待機中は、移動処理を行う
	if (mState == EState::eIdle)
	{
		UpdateMove();
	}

	CPlayerBase::Update();

#if _DEBUG
	CDebugPrint::Print("PlayerState:%d\n", GetStateStr(mState));
#endif
}

// 衝突処理
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);
}

// コライダ―を生成
void CPlayer::CreateCol()
{
	// 本体コライダ
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	// フィールド,壁、オブジェクトとだけ衝突
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject });
}

// アクションのキー入力
void CPlayer::ActionInput()
{
	// 接地していれば
	if (mIsGrounded)
	{
		if (CInput::PushKey(VK_LBUTTON))
		{

		}
	}
}

// 待機状態
void CPlayer::UpdateIdle()
{
	// キー入力
	ActionInput();
}

// 移動処理
void CPlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;
	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * GetBaseMoveSpeed() * Times::DeltaTime();

		// 待機状態であれば、移動アニメーションに切り替え
		if (mState == EState::eIdle)
		{	
			// ブレンドが終了していたら
			if (ChangeAnimationBlend((int)EAnimType::eMove, 5.0f))
			{
				// 移動アニメーションに切り替え
				ChangeAnimation((int)EAnimType::eMove);
			}
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			// ブレンドが終了していたら
			if (ChangeAnimationBlend((int)EAnimType::eIdle, 5.0f))
			{
				// 待機アニメーションに切り替え
				ChangeAnimation((int)EAnimType::eIdle);
			}
		}
	}
}

// 被弾開始
void CPlayer::UpdateDamageStart()
{
}

// 被弾ノックバック
void CPlayer::UpdateDamage()
{
}

// 被弾終了
void CPlayer::UpdateDamageEnd()
{
}

// ジャンプ開始
void CPlayer::UpdateJumpStart()
{
}

// ジャンプ中
void CPlayer::UpdateJump()
{
}

// ジャンプ終了
void CPlayer::UpdateJumpEnd()
{
}

// 攻撃開始
void CPlayer::UpdateAttackStart()
{
}

// 攻撃中
void CPlayer::UpdateAttack()
{
}

// 攻撃終了
void CPlayer::UpdateAttackEnd()
{
}

// 回避開始
void CPlayer::UpdateDodgeStart()
{
}

// 回避中
void CPlayer::UpdateDodge()
{
}

// 回避終了
void CPlayer::UpdateDodgeEnd()
{
}

// パリィ開始
void CPlayer::UpdateParryStart()
{
}

// パリィ中
void CPlayer::UpdateParry()
{
}

// パリィ終了
void CPlayer::UpdateParryEnd()
{
}

// 死亡の更新処理
void CPlayer::UpdateDeath()
{
}

// 死亡
void CPlayer::Death()
{
}

// 状態切り替え
void CPlayer::ChangeState(EState state)
{
	if (mState == state) return;

	mState = state;
}

#if _DEBUG

// 状態の文字列を取得
std::string CPlayer::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:			return "待機";			break;
	case EState::eMove:			return "移動";			break;
	case EState::eDamageStart:	return "被弾開始";		break;
	case EState::eDamage:		return "被弾中";		break;
	case EState::eDamageEnd:	return "被弾終了";		break;
	case EState::eJumpStart:	return "ジャンプ開始";	break;
	case EState::eJump:			return "ジャンプ中";	break;
	case EState::eJumpEnd:		return "ジャンプ終了";	break;
	case EState::eAttackStart:	return "攻撃開始";		break;
	case EState::eAttack:		return "攻撃中";		break;
	case EState::eAttackEnd:	return "攻撃終了";		break;
	case EState::eDodgeStart:	return "回避開始";		break;
	case EState::eDodge:		return "回避中";		break;
	case EState::eDodgeEnd:		return "回避終了";		break;
	case EState::eParryStart:	return "パリィ開始";	break;
	case EState::eParry:		return "パリィ中";		break;
	case EState::eParryEnd:		return "パリィ終了";	break;
	case EState::eDeath:		return "死亡";			break;
	}
}

#endif
