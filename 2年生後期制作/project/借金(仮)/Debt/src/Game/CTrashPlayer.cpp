//プレイヤークラスのインクルード
#include "CTrashPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CJobStatusManager.h"

/*
 プレイヤーのアニメーションデータのテーブル
 (と)＝蓋が閉じている状態からのアニメーション
 (開)＝蓋が開いている状態からのアニメーション
 (開閉)＝両方の状態兼用のアニメーション
*/
const CTrashPlayer::AnimData CTrashPlayer::ANIM_DATA[] =
{
	{ "",													true,	0.0f	},	// Tポーズ
	{ "Character\\TrashBox\\anim\\Idle_Close.x",			true,	30.0f	},	// 待機					（と）
	{ "Character\\TrashBox\\anim\\Idle_Open.x",				true,	30.0f	},	// 待機					（開）
	{ "Character\\TrashBox\\anim\\Move_Close.x",			true,	80.0f	},	// 移動					（と）
	{ "Character\\TrashBox\\anim\\Move_Open.x",				true,	80.0f	},	// 移動					（開）
	{ "Character\\TrashBox\\anim\\Damage_Close_Start.x",	false,	10.0f	},	// 被弾開始				（と）
	{ "Character\\TrashBox\\anim\\Damage_Close.x",			true,	11.0f	},	// 被弾ノックバック		（と）
	{ "Character\\TrashBox\\anim\\Damage_Close_End.x",		false,	51.0f	},	// 被弾終了				（と）
	{ "Character\\TrashBox\\anim\\Damage_Open_Start.x",		false,	10.0f	},	// 被弾開始				（開）
	{ "Character\\TrashBox\\anim\\Damage_Open.x",			true,	11.0f	},	// 被弾ノックバック		（開）
	{ "Character\\TrashBox\\anim\\Damage_Open_End.x",		false,	51.0f	},	// 被弾終了				（開）
	{ "Character\\TrashBox\\anim\\Jump_Close_Start.x",		false,	20.0f	},	// ジャンプ開始			（と）
	{ "Character\\TrashBox\\anim\\Jump_Close.x",			true,	11.0f	},	// ジャンプ中			（と）
	{ "Character\\TrashBox\\anim\\Jump_Close_End.x",		false,	21.0f	},	// ジャンプ終了			（と）
	{ "Character\\TrashBox\\anim\\Jump_Open_Start.x",		false,	20.0f	},	// ジャンプ開始			（開）
	{ "Character\\TrashBox\\anim\\Jump_Open.x",				true,	11.0f	},	// ジャンプ中			（開）
	{ "Character\\TrashBox\\anim\\Jump_Open_End.x",			false,	21.0f	},	// ジャンプ終了			（開）
	{ "Character\\TrashBox\\anim\\Attack_Start.x",			false,	35.0f	},	// 攻撃開始				（開閉）
	{ "Character\\TrashBox\\anim\\Attack.x",				false,	11.0f	},	// 攻撃中				（開閉）
	{ "Character\\TrashBox\\anim\\Attack_End.x",			false,	69.0f	},	// 攻撃終了				（開閉）
	{ "Character\\TrashBox\\anim\\Critical_Start.x",		false,	35.0f	},	// クリティカル攻撃開始	（開閉）
	{ "Character\\TrashBox\\anim\\Critical.x",				false,	11.0f	},	// クリティカル攻撃中	（開閉）
	{ "Character\\TrashBox\\anim\\Critical_End.x",			false,	69.0f	},	// クリティカル攻撃終了	（開閉）
	{ "Character\\TrashBox\\anim\\Open.x",					true,	10.0f	},	// 蓋を開く				（と）
	{ "Character\\TrashBox\\anim\\Close.x",					true,	10.0f	},	// 蓋を閉じる			（開）
};

#define CAPSULE_RADIUS 2.5f
#define PLAYER_HEIGHT 25.0f
#define PLAYER_WIDTH 50.0f

// モーションブラーを掛ける時間
#define MOTION_BLUR_TIME 3.0f
// モーションブラーの幅
#define MOTION_BLUR_WIDTH 1.0f
// モーションブラーの反復回数
#define MOTION_BLUR_COUNT 5

// コンストラクタ
CTrashPlayer::CTrashPlayer()
	: CPlayerBase(CAPSULE_RADIUS, PLAYER_HEIGHT)
	, mState(EState::eIdle)
	, mIsOpen(false)
	, mIsJump(false)
{
	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("TrashEnemy");

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	//フィールド、壁、オブジェクトとだけ衝突判定をする
	mpColliderCapsule = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(PLAYER_WIDTH - CAPSULE_RADIUS * 10, PLAYER_HEIGHT, 0.0f),
		CVector(-PLAYER_WIDTH + CAPSULE_RADIUS * 10, PLAYER_HEIGHT, 0.0f),
		CAPSULE_RADIUS
	);
	mpColliderCapsule->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::eObject });

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle_Close);
}

CTrashPlayer::~CTrashPlayer()
{
}

// 更新
void CTrashPlayer::Update()
{
	// 現在の状態に合わせて更新処理を切り替え
	switch (mState)
	{
	case EState::eIdle:				UpdateIdle();			break;
	case EState::eDamageStart:		UpdateDamageStart();	break;
	case EState::eDamage:			UpdateDamage();			break;
	case EState::eDamageEnd:		UpdateDamageEnd();		break;
	case EState::eJumpStart:		UpdateJumpStart();		break;
	case EState::eJump:				UpdateJump();			break;
	case EState::eJumpEnd:			UpdateJumpEnd();		break;
	case EState::eAttackStart:		UpdateAttackStart();	break;
	case EState::eAttack:			UpdateAttack();			break;
	case EState::eAttackEnd:		UpdateAttackEnd();		break;
	case EState::eCriticalStart:	UpdateCriticalStart();	break;
	case EState::eCritical:			UpdateCritical();		break;
	case EState::eCriticalEnd:		UpdateCriticalEnd();	break;
	case EState::eOpenClose:		UpdateOpenClose();		break;
	}

	// 待機中とジャンプ中は、移動処理を行う
	if (mState == EState::eIdle
		|| mState == EState::eJump)
	{
		UpdateMove();
	}

	// 地面に接しているならジャンプしていない
	if (mIsGrounded)
		mIsJump = false;

	// キャラクターの更新
	CPlayerBase::Update();

	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("IsOpen:%s\n", mIsOpen ? "true" : "false");
	CDebugPrint::Print("IsJump:%s\n", mIsJump ? "true" : "false");
}

void CTrashPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);
}

// アクションのキー入力
void CTrashPlayer::ActionInput()
{
	// スペースでジャンプ
	if (CInput::PushKey(VK_SPACE))
	{
		ChangeState(EState::eJumpStart);

		// 閉じた状態のジャンプ
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eJump_Close_Start);
		// 開いた状態のジャンプ
		else
			ChangeAnimation(EAnimType::eJump_Open_Start);
	}
	// 左クリックで攻撃
	if (CInput::PushKey(VK_LBUTTON))
	{
		mMoveSpeed = CVector::zero;
		ChangeState(EState::eAttackStart);

		// 閉じていたら開く
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eOpen);
		else
			ChangeAnimation(EAnimType::eAttack_Start);
	}
	// 右クリックで蓋の開閉
	if (CInput::PushKey(VK_RBUTTON))
	{
		mMoveSpeed = CVector::zero;
		ChangeState(EState::eOpenClose);

		// 閉じていたら蓋を開く
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eOpen);
		// 開いていたら蓋を閉じる
		else
			ChangeAnimation(EAnimType::eClose);
	}
	if (CInput::PushKey('1'))
	{
		mMoveSpeed = CVector::zero;
		ChangeState(EState::eCriticalStart);
		// 閉じていたら開く
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eOpen);
		else
			ChangeAnimation(EAnimType::eCritical_Start);
	}
	if (CInput::PushKey('2'))
	{
		mIsDamage = true;
		mMoveSpeed = CVector::zero;
		SetTakeKnockback(GetStatusBase().mKnockback);
		ChangeState(EState::eDamageStart);
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eDamage_Close_Start);
		else
			ChangeAnimation(EAnimType::eDamage_Open_Start);
	}
}

// 待機状態
void CTrashPlayer::UpdateIdle()
{
	if (mIsGrounded)
	{
		ActionInput();
	}
}

// 移動
void CTrashPlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * GetStatusBase().mMoveSpeed;

		// ジャンプ状態でなければ、移動アニメーションに切り替え
		if (mState != EState::eJump)
		{
			ChangeState(EState::eIdle);
			if (!mIsOpen)
				ChangeAnimation(EAnimType::eMove_Close);
			else
				ChangeAnimation(EAnimType::eMove_Open);
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			if (!mIsOpen)
				ChangeAnimation(EAnimType::eIdle_Close);
			else
				ChangeAnimation(EAnimType::eIdle_Open);
		}
	}
}

// 被弾開始
void CTrashPlayer::UpdateDamageStart()
{
	// アニメーションが終了したら被弾ノックバックへ
	if (IsAnimationFinished())
	{
		// ダメージを1受ける
		TakeDamage();
		// ノックバック速度の設定
		mMoveSpeedY = GetStatusBase().mJumpSpeed;
		// 後ろ方向にノックバックさせる
		mMoveSpeed = -VectorZ() * GetTakeKnockback();
		mIsGrounded = false;

		ChangeState(EState::eDamage);

		if (!mIsOpen)
			ChangeAnimation(EAnimType::eDamage_Close);
		else
			ChangeAnimation(EAnimType::eDamage_Open);
	}
}

// 被弾ノックバック
void CTrashPlayer::UpdateDamage()
{
	mIsOpen = true;

	// 地面に付いたら被弾終了へ
	if (mIsGrounded)
	{
		ChangeState(EState::eDamageEnd);

		if (!mIsOpen)
			ChangeAnimation(EAnimType::eDamage_Close_End);
		else
			ChangeAnimation(EAnimType::eDamage_Open_End);
	}
}

// 被弾終了
void CTrashPlayer::UpdateDamageEnd()
{
	// 被弾終了時は移動をゼロ
	mMoveSpeed = CVector::zero;

	// アニメーションが60%進行したら
	if (GetAnimationFrameRatio() >= 0.60f)
	{
		// 入力可能
		ActionInput();
		// 移動可能
		UpdateMove();

		mIsDamage = false;
		SetTakeKnockback(0.0f);
	}

	// アニメーションが終了したら待機へ
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// ジャンプ開始
void CTrashPlayer::UpdateJumpStart()
{
	// ジャンプ開始時は移動をゼロ
	mMoveSpeed = CVector::zero;

	// アニメーションが終了したら
	if (IsAnimationFinished())
	{
		// ジャンプ速度の設定
		mMoveSpeedY = GetStatusBase().mJumpSpeed;
		mIsGrounded = false;

		ChangeState(EState::eJump);
		// 閉じた状態のジャンプ中へ
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eJump_Close);
		// 開いた状態のジャンプ中へ
		else
			ChangeAnimation(EAnimType::eJump_Open);
	}
}

// ジャンプ中
void CTrashPlayer::UpdateJump()
{
	mIsJump = true;
	// ジャンプ中は移動しながら開閉可能
	// 右クリックで蓋の開閉
	if (CInput::PushKey(VK_RBUTTON))
	{
		ChangeState(EState::eOpenClose);
		// 閉じていたら蓋を開く
		if (!mIsOpen)
		{
			ChangeAnimation(EAnimType::eOpen);
		}
		// 開いていたら蓋を閉じる
		else
		{
			ChangeAnimation(EAnimType::eClose);
		}
	}
	// 地面に付いたら
	if (mIsGrounded)
	{
		ChangeState(EState::eJumpEnd);
		// 閉じた状態のジャンプ終了へ
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eJump_Close_End);
		// 開いた状態のジャンプ終了へ
		else
			ChangeAnimation(EAnimType::eJump_Open_End);
	}
}

// ジャンプ終了
void CTrashPlayer::UpdateJumpEnd()
{
	// 着地中は移動をゼロ
	mMoveSpeed = CVector::zero;
	// アニメーションが50%以上進行したら
	if (GetAnimationFrameRatio() >= 0.50f)
	{
		// 入力可能
		ActionInput();
		// 移動可能
		UpdateMove();
	}

	// アニメーションが終了したら
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// 攻撃開始
void CTrashPlayer::UpdateAttackStart()
{
	// 開いていなければ開くアニメーションの再生をしているので
	// 終わってから攻撃の最初へ
	if (!mIsOpen)
	{
		if (IsAnimationFinished())
		{
			mIsOpen = true;
			ChangeAnimation(EAnimType::eAttack_Start);
		}
	}
	// 開いているなら攻撃の最初のアニメーションが終了したので攻撃中へ
	else
	{
		if (IsAnimationFinished())
		{
			ChangeState(EState::eAttack);
			ChangeAnimation(EAnimType::eAttack);
		}
	}
}

// 攻撃中
void CTrashPlayer::UpdateAttack()
{
	// アニメーションが終了したら
	if (IsAnimationFinished())
	{
		// 攻撃終了へ
		ChangeState(EState::eAttackEnd);
		ChangeAnimation(EAnimType::eAttack_End);
	}
}

// 攻撃終了
void CTrashPlayer::UpdateAttackEnd()
{
	// アニメーションが52%以上進行したら
	if (GetAnimationFrameRatio() >= 0.52f)
	{
		// 入力可能
		ActionInput();
		// 移動可能
		UpdateMove();
	}

	// アニメーションが終了したら待機へ
	if (IsAnimationFinished())
	{
		// 最後は蓋が開いた状態
		ChangeState(EState::eIdle);
		mIsOpen = true;
	}
}

// クリティカル攻撃開始
void CTrashPlayer::UpdateCriticalStart()
{
	// 開いていなければ開くアニメーションの再生をしているので
	// 終わってからクリティカルの最初へ
	if (!mIsOpen)
	{
		if (IsAnimationFinished())
		{
			mIsOpen = true;
			ChangeAnimation(EAnimType::eCritical_Start);
		}
	}
	// 開いているならクリティカルの最初のアニメーションが終了したのでクリティカルへ
	else
	{
		if (IsAnimationFinished())
		{
			ChangeState(EState::eCritical);
			ChangeAnimation(EAnimType::eCritical);
		}
	}
}

// クリティカル攻撃中
void CTrashPlayer::UpdateCritical()
{
	// アニメーションが終了したらクリティカル終了へ
	if (IsAnimationFinished())
	{
		ChangeState(EState::eCriticalEnd);
		ChangeAnimation(EAnimType::eCritical_End);
	}
}

// クリティカル攻撃終了
void CTrashPlayer::UpdateCriticalEnd()
{
	// アニメーションが52%以上進行したら
	if (GetAnimationFrameRatio() >= 0.52f)
	{
		// 入力可能
		ActionInput();
		// 移動可能
		UpdateMove();
	}

	// アニメーションが終了したら待機へ
	if (IsAnimationFinished())
	{
		// 最後は蓋が開いた状態
		ChangeState(EState::eIdle);
		mIsOpen = true;
	}
}

// 蓋を開閉する
void CTrashPlayer::UpdateOpenClose()
{
	if (IsAnimationFinished())
	{
		// 開き状態を変更
		mIsOpen = !mIsOpen;
		// ジャンプしていないなら待機へ
		if (!mIsJump)
		{
			ChangeState(EState::eIdle);
		}
		// ジャンプしているならジャンプへ戻る
		else
		{
			ChangeState(EState::eJump);
			// 閉じた状態のジャンプ
			if (!mIsOpen)
				ChangeAnimation(EAnimType::eJump_Close);
			// 開いた状態のジャンプ
			else
				ChangeAnimation(EAnimType::eJump_Open);
		}

	}
}

// アニメーション切り替え
void CTrashPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ステータス切り替え
void CTrashPlayer::ChangeState(EState state)
{
	// 既に同じ状態であれば、処理しない
	if (state == mState) return;

	mState = state;
}

// 状態の文字列を取得
std::string CTrashPlayer::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return "待機";
	case EState::eDamageStart:		return "被弾開始";
	case EState::eDamage:			return "被弾中";
	case EState::eDamageEnd:		return "被弾終了";
	case EState::eJumpStart:		return "ジャンプ開始";
	case EState::eJump:				return "ジャンプ中";
	case EState::eJumpEnd:			return "ジャンプ終了";
	case EState::eAttackStart:		return "攻撃開始";
	case EState::eAttack:			return "攻撃中";
	case EState::eAttackEnd:		return "攻撃終了";
	case EState::eCriticalStart:	return "クリティカル攻撃開始";
	case EState::eCritical:			return "クリティカル攻撃中";
	case EState::eCriticalEnd:		return "クリティカル攻撃終了";
	case EState::eOpenClose:		return "開閉中";
	}
	return "";
}
