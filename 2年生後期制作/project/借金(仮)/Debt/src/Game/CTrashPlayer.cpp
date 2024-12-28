#include "CTrashPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"

// 衝突相手の車両基底クラスを取得するための
// 車両の基底クラスのインクルード
#include "CVehicleBase.h"

// コライダのインクルード
#include "CColliderCapsule.h"

// アニメーションのパス
#define ANIM_PATH "Character\\TrashBox\\anim\\"
/*
 プレイヤーのアニメーションデータのテーブル
 (と)＝蓋が閉じている状態からのアニメーション
 (開)＝蓋が開いている状態からのアニメーション
 (開閉)＝両方の状態兼用のアニメーション
*/
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",								true,	0.0f,	1.0f},	// Tポーズ
	{ ANIM_PATH"Idle_Close.x",			true,	30.0f,	1.0f},	// 待機					（と）
	{ ANIM_PATH"Idle_Open.x",			true,	30.0f,	1.0f},	// 待機					（開）
	{ ANIM_PATH"Move_Close.x",			true,	80.0f,	1.0f},	// 移動					（と）
	{ ANIM_PATH"Move_Open.x",			true,	80.0f,	1.0f},	// 移動					（開）
	{ ANIM_PATH"Damage_Close_Start.x",	false,	10.0f,	1.0f},	// 被弾開始				（と）
	{ ANIM_PATH"Damage_Close.x",		true,	11.0f,	1.0f},	// 被弾ノックバック		（と）
	{ ANIM_PATH"Damage_Close_End.x",	false,	51.0f,	1.0f},	// 被弾終了				（と）
	{ ANIM_PATH"Damage_Open_Start.x",	false,	10.0f,	1.0f},	// 被弾開始				（開）
	{ ANIM_PATH"Damage_Open.x",			true,	11.0f,	1.0f},	// 被弾ノックバック		（開）
	{ ANIM_PATH"Damage_Open_End.x",		false,	51.0f,	1.0f},	// 被弾終了				（開）
	{ ANIM_PATH"Jump_Close_Start.x",	false,	20.0f,	1.0f},	// ジャンプ開始			（と）
	{ ANIM_PATH"Jump_Close.x",			true,	11.0f,	1.0f},	// ジャンプ中			（と）
	{ ANIM_PATH"Jump_Close_End.x",		false,	21.0f,	1.0f},	// ジャンプ終了			（と）
	{ ANIM_PATH"Jump_Open_Start.x",		false,	20.0f,	1.0f},	// ジャンプ開始			（開）
	{ ANIM_PATH"Jump_Open.x",			true,	11.0f,	1.0f},	// ジャンプ中			（開）
	{ ANIM_PATH"Jump_Open_End.x",		false,	21.0f,	1.0f},	// ジャンプ終了			（開）
	{ ANIM_PATH"Attack_Start.x",		false,	35.0f,	1.0f},	// 攻撃開始				（開閉）
	{ ANIM_PATH"Attack.x",				false,	11.0f,	1.0f},	// 攻撃中				（開閉）
	{ ANIM_PATH"Attack_End.x",			false,	69.0f,	1.0f},	// 攻撃終了				（開閉）
	{ ANIM_PATH"Critical_Start.x",		false,	35.0f,	1.0f},	// クリティカル攻撃開始	（開閉）
	{ ANIM_PATH"Critical.x",			false,	11.0f,	1.0f},	// クリティカル攻撃中	（開閉）
	{ ANIM_PATH"Critical_End.x",		false,	69.0f,	1.0f},	// クリティカル攻撃終了	（開閉）
	{ ANIM_PATH"Open.x",				true,	10.0f,	1.0f},	// 蓋を開く				（と）
	{ ANIM_PATH"Close.x",				true,	10.0f,	1.0f},	// 蓋を閉じる			（開）
};

#define BODY_RADIUS 2.5f	// 本体のコライダ―の半径
#define BODY_HEIGHT 25.0f	// 本体のコライダ―の高さ
#define BODY_WIDTH 50.0f	// 本体のコライダ―の幅

// モーションブラーを掛ける時間
#define MOTION_BLUR_TIME 3.0f
// モーションブラーの幅
#define MOTION_BLUR_WIDTH 1.0f
// モーションブラーの反復回数
#define MOTION_BLUR_COUNT 5

// コンストラクタ
CTrashPlayer::CTrashPlayer()
	: CPlayerBase()
	, mState(EState::eIdle)
	, mIsOpen(false)
	, mIsJump(false)
{
	// アニメーションとモデルの初期化
	InitAnimationModel("TrashPlayer", &ANIM_DATA);

	// 地形、敵、攻撃、車両
	// と衝突判定をする本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(BODY_WIDTH - BODY_RADIUS * 10, BODY_HEIGHT, 0.0f),
		CVector(-BODY_WIDTH + BODY_RADIUS * 10, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eEnemy, ETag::eVehicle});
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::eObject,
		ELayer::eEnemy, ELayer::eAttackCol, ELayer::eVehicle});

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle_Close);
}

// デストラクタ
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

#if _DEBUG
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("PlayerIsOpen:%s\n", mIsOpen ? "true" : "false");
	CDebugPrint::Print("PlayerIsJump:%s\n", mIsJump ? "true" : "false");
#endif
}

// 衝突処理
void CTrashPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 基底クラスの衝突処理
	CPlayerBase::Collision(self, other, hit);

	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// 衝突した相手が車両なら
		if (other->Layer() == ELayer::eVehicle)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);

			// 車両クラスを取得
			CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());
			// 移動中なら吹き飛ばされる
			if (vehicle->IsMove())
			{
				// 移動速度をゼロにする
				mMoveSpeed = CVector::zero;

				// 車両クラスを取得
				CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());
				// 相手から自分の方向
				CVector direction = Position() - other->Owner()->Position();
				direction.Y(0.0f);
				direction = direction.Normalized();
				// 自分が受けるノックバック速度に、
				// 相手が与えるノックバック速度を相手から自分の方向に設定
				SetKnockbackReceived(direction * vehicle->GetKnockbackDealt());

				// 攻撃を受けていなければ被弾開始アニメーションに変更
				if (!mIsDamage)
				{
					// 蓋がしまっていたら蓋がしまっている状態からのアニメーション
					if (!mIsOpen)
						ChangeAnimation((int)EAnimType::eDamage_Close_Start);
					// 蓋が開いていたら蓋が開いている状態からのアニメーション
					else
						ChangeAnimation((int)EAnimType::eDamage_Open_Start);
				}

				// 攻撃を受けている
				mIsDamage = true;
				// 状態を被弾開始に変更
				ChangeState(EState::eDamageStart);
			}
		}
		// 衝突した相手が敵なら
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
		}
	}
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
			ChangeAnimation((int)EAnimType::eJump_Close_Start);
		// 開いた状態のジャンプ
		else
			ChangeAnimation((int)EAnimType::eJump_Open_Start);
	}
	// 左クリックで攻撃
	if (CInput::PushKey(VK_LBUTTON))
	{
		mMoveSpeed = CVector::zero;
		ChangeState(EState::eAttackStart);

		// 閉じていたら開く
		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eOpen);
		else
			ChangeAnimation((int)EAnimType::eAttack_Start);
	}
	// 右クリックで蓋の開閉
	if (CInput::PushKey(VK_RBUTTON))
	{
		mMoveSpeed = CVector::zero;
		ChangeState(EState::eOpenClose);

		// 閉じていたら蓋を開く
		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eOpen);
		// 開いていたら蓋を閉じる
		else
			ChangeAnimation((int)EAnimType::eClose);
	}
	if (CInput::PushKey('1'))
	{
		mMoveSpeed = CVector::zero;
		ChangeState(EState::eCriticalStart);
		// 閉じていたら開く
		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eOpen);
		else
			ChangeAnimation((int)EAnimType::eCritical_Start);
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
		mMoveSpeed += move * GetBaseMoveSpeed();

		// ジャンプ状態でなければ、移動アニメーションに切り替え
		if (mState != EState::eJump)
		{
			ChangeState(EState::eIdle);
			if (!mIsOpen)
				ChangeAnimation((int)EAnimType::eMove_Close);
			else
				ChangeAnimation((int)EAnimType::eMove_Open);
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			if (!mIsOpen)
				ChangeAnimation((int)EAnimType::eIdle_Close);
			else
				ChangeAnimation((int)EAnimType::eIdle_Open);
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
		TakeDamage(1,nullptr);		
		// ノックバック時の飛び上がりの速度を
		// 受けるノックバック速度の半分に設定
		mMoveSpeedY = GetKnockbackReceived().Length() * 0.5f;
		// 受けるノックバック速度を移動に設定
		mMoveSpeed = GetKnockbackReceived();
		mIsGrounded = false;

		ChangeState(EState::eDamage);

		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eDamage_Close);
		else
			ChangeAnimation((int)EAnimType::eDamage_Open);
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
			ChangeAnimation((int)EAnimType::eDamage_Close_End);
		else
			ChangeAnimation((int)EAnimType::eDamage_Open_End);
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
		SetKnockbackReceived(CVector::zero);
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
		mMoveSpeedY = GetJumpSpeed();
		mIsGrounded = false;

		ChangeState(EState::eJump);
		// 閉じた状態のジャンプ中へ
		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eJump_Close);
		// 開いた状態のジャンプ中へ
		else
			ChangeAnimation((int)EAnimType::eJump_Open);
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
			ChangeAnimation((int)EAnimType::eOpen);
		}
		// 開いていたら蓋を閉じる
		else
		{
			ChangeAnimation((int)EAnimType::eClose);
		}
	}
	// 地面に付いたら
	if (mIsGrounded)
	{
		ChangeState(EState::eJumpEnd);
		// 閉じた状態のジャンプ終了へ
		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eJump_Close_End);
		// 開いた状態のジャンプ終了へ
		else
			ChangeAnimation((int)EAnimType::eJump_Open_End);
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
			ChangeAnimation((int)EAnimType::eAttack_Start);
		}
	}
	// 開いているなら攻撃の最初のアニメーションが終了したので攻撃中へ
	else
	{
		if (IsAnimationFinished())
		{
			ChangeState(EState::eAttack);
			ChangeAnimation((int)EAnimType::eAttack);
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
		ChangeAnimation((int)EAnimType::eAttack_End);
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
			ChangeAnimation((int)EAnimType::eCritical_Start);
		}
	}
	// 開いているならクリティカルの最初のアニメーションが終了したのでクリティカルへ
	else
	{
		if (IsAnimationFinished())
		{
			ChangeState(EState::eCritical);
			ChangeAnimation((int)EAnimType::eCritical);
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
		ChangeAnimation((int)EAnimType::eCritical_End);
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
				ChangeAnimation((int)EAnimType::eJump_Close);
			// 開いた状態のジャンプ
			else
				ChangeAnimation((int)EAnimType::eJump_Open);
		}

	}
}

// 状態切り替え
void CTrashPlayer::ChangeState(EState state)
{
	// 既に同じ状態であれば、処理しない
	if (state == mState) return;

	mState = state;
}

#if _DEBUG
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
#endif
