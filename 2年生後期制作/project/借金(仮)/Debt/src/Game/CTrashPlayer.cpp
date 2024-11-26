//プレイヤークラスのインクルード
#include "CTrashPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"

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
	{ "Character\\TrashBox\\anim\\Critical_.x",				false,	11.0f	},	// クリティカル攻撃中	（開閉）
	{ "Character\\TrashBox\\anim\\Critical_End.x",			false,	69.0f	},	// クリティカル攻撃終了	（開閉）
	{ "Character\\TrashBox\\anim\\Open.x",					true,	10.0f	},	// 蓋を開く				（と）
	{ "Character\\TrashBox\\anim\\Close.x",					true,	10.0f	},	// 蓋を閉じる			（開）
};

#define CAPSULE_RADIUS 2.5f
#define PLAYER_HEIGHT 16.0f

#define MOVE_SPEED 0.375f * 2.0f
#define JUMP_SPEED 1.5f

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
	 , mTest(0)
 {
	 // モデルデータ取得
	 CModelX* model = CResourceManager::Get<CModelX>("TrashPlayer");

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

	 // 最初は待機アニメーションを再生
	 ChangeAnimation(EAnimType::eIdle_Close);
 }

CTrashPlayer::~CTrashPlayer()
{
}

// 更新
void CTrashPlayer::Update()
{
	switch (mState)
	{
	case EState::eIdle:
		UpdateIdle();
		break;
	case EState::eDamageStart:
		UpdateDamageStart();
		break;
	case EState::eDamage:
		UpdateDamage();
		break;
	case EState::eDamageEnd:
		UpdateDamageEnd();
		break;
	case EState::eJumpStart:
		UpdateJumpStart();
		break;
	case EState::eJump:
		UpdateJump();
		break;
	case EState::eJumpEnd:
		UpdateJumpEnd();
		break;
	case EState::eAttackStart:
		UpdateAttackStart();
		break;
	case EState::eAttack:
		UpdateAttack();
		break;
	case EState::eAttackEnd:
		UpdateAttackEnd();
		break;
	case EState::eCriticalStart:
		UpdateCriticalStart();
		break;
	case EState::eCritical:
		UpdateCritical();
		break;
	case EState::eCriticalEnd:
		UpdateCriticalEnd();
		break;
	case EState::eOpenClose:
		UpdateOpenClose();
		break;
	}

	// 待機中とジャンプ中は、移動処理を行う
	if (mState == EState::eIdle
		|| mState == EState::eJump)
	{
		UpdateMove();
	}

	// キャラクターの更新
	CPlayerBase::Update();

	CDebugPrint::Print("State:%d\n", mState);
}

void CTrashPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);
}

// 待機状態
void CTrashPlayer::UpdateIdle()
{
	if (mIsGrounded)
	{
		// 左クリックで攻撃
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed = CVector::zero;
			mState = EState::eAttackStart;

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
			mState = EState::eOpenClose;

			// 閉じていたら蓋を開く
			if (!mIsOpen)
				ChangeAnimation(EAnimType::eOpen);
			// 開いていたら蓋を閉じる
			else
				ChangeAnimation(EAnimType::eClose);
		}
		// スペースでジャンプ
		if (CInput::PushKey(VK_SPACE))
		{
			mMoveSpeed = CVector::zero;
			mState = EState::eJumpStart;

			// 閉じた状態のジャンプ
			if (!mIsOpen)
				ChangeAnimation(EAnimType::eJump_Close_Start);
			// 開いた状態のジャンプ
			else
				ChangeAnimation(EAnimType::eJump_Open_Start);
		}
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
		mMoveSpeed += move * MOVE_SPEED;

		// 待機状態であれば、移動アニメーションに切り替え
		if (mState == EState::eIdle)
		{
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
	if (!mIsOpen)
		ChangeAnimation(EAnimType::eDamage_Close_Start);
	else
		ChangeAnimation(EAnimType::eDamage_Open_Start);

	if (IsAnimationFinished())
	{
		mState = EState::eDamage;
	}
}

// 被弾ノックバック
void CTrashPlayer::UpdateDamage()
{
	if (!mIsOpen)
		ChangeAnimation(EAnimType::eDamage_Close);
	else
		ChangeAnimation(EAnimType::eDamage_Open);

	if (IsAnimationFinished())
	{
		mState = EState::eDamageEnd;
	}
}

// 被弾終了
void CTrashPlayer::UpdateDamageEnd()
{
	if (!mIsOpen)
		ChangeAnimation(EAnimType::eDamage_Close_End);
	else
		ChangeAnimation(EAnimType::eDamage_Open_End);

	if (IsAnimationFinished())
	{
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eIdle_Close);
		else
			ChangeAnimation(EAnimType::eIdle_Open);
		mState = EState::eIdle;
	}
}

// ジャンプ開始
void CTrashPlayer::UpdateJumpStart()
{
	if (!mIsOpen)
		ChangeAnimation(EAnimType::eJump_Close_Start);
	else
		ChangeAnimation(EAnimType::eJump_Open_Start);

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;

	if (IsAnimationFinished())
	{
		mState = EState::eJump;
	}
}

// ジャンプ中
void CTrashPlayer::UpdateJump()
{
	if (!mIsOpen)
		ChangeAnimation(EAnimType::eJump_Close);
	else
		ChangeAnimation(EAnimType::eJump_Open);

	if (mMoveSpeedY <= 0.0f)
	{
		mState = EState::eJumpEnd;
	}
}

// ジャンプ終了
void CTrashPlayer::UpdateJumpEnd()
{
	if (!mIsOpen)
		ChangeAnimation(EAnimType::eJump_Close_End);
	else
		ChangeAnimation(EAnimType::eJump_Open_End);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
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
			mIsOpen = !mIsOpen;
			ChangeAnimation(EAnimType::eAttack_Start);
		}
	}
	// 開いているなら攻撃の最初のアニメーションが終了したので攻撃中へ
	else
	{
		if (IsAnimationFinished())
		{
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eAttack);
		}
	}
}

// 攻撃中
void CTrashPlayer::UpdateAttack()
{
	if (IsAnimationFinished())
	{
		mState = EState::eAttackEnd;
		ChangeAnimation(EAnimType::eAttack_End);
	}
}

// 攻撃終了
void CTrashPlayer::UpdateAttackEnd()
{
	if (IsAnimationFinished())
	{
		// 攻撃は最後には蓋が閉じる
		mState = EState::eIdle;
		mIsOpen = false;
		ChangeAnimation(EAnimType::eIdle_Close);
	}
}

// クリティカル攻撃開始
void CTrashPlayer::UpdateCriticalStart()
{
	ChangeAnimation(EAnimType::eCritical_Start);
	if (IsAnimationFinished())
	{
		mState = EState::eCritical;
	}
}

// クリティカル攻撃中
void CTrashPlayer::UpdateCritical()
{
	ChangeAnimation(EAnimType::eCritical);

	if (IsAnimationFinished())
	{
		mState = EState::eCriticalEnd;
	}
}

// クリティカル攻撃終了
void CTrashPlayer::UpdateCriticalEnd()
{
	ChangeAnimation(EAnimType::eCritical_End);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// 蓋を開閉する
void CTrashPlayer::UpdateOpenClose()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
		// 開き状態を変更
		mIsOpen = !mIsOpen;
	}
}

// アニメーション切り替え
void CTrashPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}
