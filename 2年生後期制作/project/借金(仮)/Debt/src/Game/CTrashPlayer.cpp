//プレイヤークラスのインクルード
#include "CTrashPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"

// プレイヤーのインスタンス
CTrashPlayer* CTrashPlayer::spInstance = nullptr;

/*
 プレイヤーのアニメーションデータのテーブル
 (と)＝蓋が閉じている状態からのアニメーション
 (開)＝蓋が開いている状態からのアニメーション
 (開閉)＝両方の状態兼用のアニメーション
*/
 const CTrashPlayer::AnimData CTrashPlayer::ANIM_DATA[] =
{
	{ "",												true,	0.0f	},	// Tポーズ
	{ "Character\\TrashBox\\anim\\IdleClose.x",			true,	10.0f	},	// 待機					（と）
	{ "Character\\TrashBox\\anim\\IdleOpen.x",			true,	10.0f	},	// 待機					（開）
	{ "Character\\TrashBox\\anim\\MoveClose.x",			true,	32.0f	},	// 移動					（と）
	{ "Character\\TrashBox\\anim\\MoveOpen.x",			true,	32.0f	},	// 移動					（開）
	{ "Character\\TrashBox\\anim\\DamageStartClose.x",	false,	4.0f	},	// 被弾開始				（と）
	{ "Character\\TrashBox\\anim\\DamageStartOpen.x",	false,	4.0f	},	// 被弾開始				（開）
	{ "Character\\TrashBox\\anim\\DamageClose.x",		false,	4.0f	},	// 被弾ノックバック		（と）
	{ "Character\\TrashBox\\anim\\DamageOpen.x",		false,	4.0f	},	// 被弾ノックバック		（開）
	{ "Character\\TrashBox\\anim\\DamageEndClose.x",	false,	28.0f	},	// 被弾終了				（と）
	{ "Character\\TrashBox\\anim\\DamageEndOpen.x",		false,	28.0f	},	// 被弾終了				（開）
	{ "Character\\TrashBox\\anim\\JumpStartClose.x",	false,	12.0f	},	// ジャンプ開始			（と）
	{ "Character\\TrashBox\\anim\\JumpStartOpen.x",		false,	12.0f	},	// ジャンプ開始			（開）
	{ "Character\\TrashBox\\anim\\JumpClose.x",			true,	12.0f	},	// ジャンプ中			（と）
	{ "Character\\TrashBox\\anim\\JumpOpen.x",			true,	12.0f	},	// ジャンプ中			（開）
	{ "Character\\TrashBox\\anim\\JumpEndClose.x",		false,	10.0f	},	// ジャンプ終了			（と）
	{ "Character\\TrashBox\\anim\\JumpEndOpen.x",		false,	10.0f	},	// ジャンプ終了			（開）
	{ "Character\\TrashBox\\anim\\AttackStartClose.x",	false,	11.0f	},	// 攻撃開始				（と）
	{ "Character\\TrashBox\\anim\\AttackStartOpen.x",	false,	10.0f	},	// 攻撃開始				（開）
	{ "Character\\TrashBox\\anim\\Attack.x",			false,	5.0f	},	// 攻撃中				（開閉）
	{ "Character\\TrashBox\\anim\\AttackEnd.x",			false,	24.0f	},	// 攻撃終了				（開閉）
	{ "Character\\TrashBox\\anim\\CriticalStartClose.x",false,	11.0f	},	// クリティカル攻撃開始	（と）
	{ "Character\\TrashBox\\anim\\CriticalStartOpen.x",	false,	10.0f	},	// クリティカル攻撃開始	（開）
	{ "Character\\TrashBox\\anim\\Critical.x",			false,	5.0f	},	// クリティカル攻撃中	（開閉）
	{ "Character\\TrashBox\\anim\\CriticalEnd.x",		false,	24.0f	},	// クリティカル攻撃終了	（開閉）
};

#define PLAYER_HEIGHT 16.0f
#define MOVE_SPEED 0.375f * 2.0f
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f
#define CAPSULE_RADIUS_HOME 2.5f
#define CAPSULE_RADIUS_TRASH 2.5f

// モーションブラーを掛ける時間
#define MOTION_BLUR_TIME 3.0f
// モーションブラーの幅
#define MOTION_BLUR_WIDTH 1.0f
// モーションブラーの反復回数
#define MOTION_BLUR_COUNT 5

// コンストラクタ
 CTrashPlayer::CTrashPlayer()
	 : CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	 , mState(EState::eIdle)
	 , IsOpen(false)
	 , mTest(0)
{
	spInstance = this;

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
	ChangeAnimation(EAnimType::eIdleClose);
}

CTrashPlayer::~CTrashPlayer()
{
}

CTrashPlayer* CTrashPlayer::Instance()
{
	return spInstance;
}

// 更新
void CTrashPlayer::Update()
{
	switch (mState)
	{
	case EState::eIdle:
		UpdateIdle();
		break;
	case EState::eMove:
		UpdateMove();
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
	}

	//// 待機中とジャンプ中は、移動処理を行う
	//if (mState == EState::eIdle
	//	|| mState == EState::eJump)
	//{
	//	UpdateMove();
	//}
	// キャラクターの更新
	CXCharacter::Update();

	CDebugPrint::Print("TState:%d\n", mState);
}

// 描画
void CTrashPlayer::Render()
{
	CXCharacter::Render();
}

// 待機状態
void CTrashPlayer::UpdateIdle()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eIdleClose);
	else
		ChangeAnimation(EAnimType::eIdleOpen);
	mTest++;
	if (mTest > 5)
	{
		mTest = 1;
		IsOpen = !IsOpen;
	}

	if (mTest == 1)
		mState = EState::eDamageStart;
	else if (mTest == 2)
		mState = EState::eJumpStart;
	else if (mTest == 3)
		mState = EState::eAttackStart;
	else if (mTest == 4)
		mState = EState::eCriticalStart;
	else if (mTest == 5)
		mState = EState::eMove;
}

// 移動
void CTrashPlayer::UpdateMove()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eMoveClose);
	else
		ChangeAnimation(EAnimType::eMoveOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// 被弾開始
void CTrashPlayer::UpdateDamageStart()
{

	if (!IsOpen)
		ChangeAnimation(EAnimType::eDamageStartClose);
	else
		ChangeAnimation(EAnimType::eDamageStartOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eDamage;
	}
}

// 被弾ノックバック
void CTrashPlayer::UpdateDamage()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eDamageClose);
	else
		ChangeAnimation(EAnimType::eDamageOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eDamageEnd;
	}
}

// 被弾終了
void CTrashPlayer::UpdateDamageEnd()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eDamageEndClose);
	else
		ChangeAnimation(EAnimType::eDamageEndOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// ジャンプ開始
void CTrashPlayer::UpdateJumpStart()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eJumpStartClose);
	else
		ChangeAnimation(EAnimType::eJumpStartOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eJump;
	}
}

// ジャンプ中
void CTrashPlayer::UpdateJump()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eJumpClose);
	else
		ChangeAnimation(EAnimType::eJumpOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eJumpEnd;
	}
}

// ジャンプ終了
void CTrashPlayer::UpdateJumpEnd()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eJumpEndClose);
	else
		ChangeAnimation(EAnimType::eJumpEndOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// 攻撃開始
void CTrashPlayer::UpdateAttackStart()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eAttackStartClose);
	else
		ChangeAnimation(EAnimType::eAttackStartOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eAttack;
	}
}

// 攻撃中
void CTrashPlayer::UpdateAttack()
{
	ChangeAnimation(EAnimType::eAttack);

	if (IsAnimationFinished())
	{
		mState = EState::eAttackEnd;
	}
}

// 攻撃終了
void CTrashPlayer::UpdateAttackEnd()
{
	ChangeAnimation(EAnimType::eAttackEnd);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// クリティカル攻撃開始
void CTrashPlayer::UpdateCriticalStart()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eCriticalStartClose);
	else
		ChangeAnimation(EAnimType::eCriticalStartOpen);

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
	ChangeAnimation(EAnimType::eCriticalEnd);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// アニメーション切り替え
void CTrashPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}
