#include "CPlayer.h"
#include "CColliderCapsule.h"
#include "CInput.h"
#include "CImage.h"
#include "CFade.h"
#include "CBall.h"
#include "CGameCamera2.h"

// 体の半径と高さ
#define BODY_RADIUS 2.5f
#define BODY_HEIGHT 15.0f

// アニメーションのパス
#define ANIM_PATH "Character\\Adventurer\\AdventurerAnim\\"

// アニメーションデータのテーブル
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",								true,	0.0f,	1.0f},	// Tポーズ
};

// コンストラクタ
CPlayer::CPlayer()
	: CPlayerBase()
	, CPlayerStatus()
	, CSpellCaster(this)
	, mState(EState::eIdle)
	, mIsAttacking(false)
{
	// 重力無効
	mIsGravity = false;
	// 移動方向を向かない
	mIsMoveDir = false;
	// アニメーションとモデルの初期化
	//InitAnimationModel("Player", &ANIM_DATA);

	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CPlayer::~CPlayer()
{
}

// 攻撃中か
bool CPlayer::IsAttacking() const
{
	return mIsAttacking;
}

// 攻撃開始
void CPlayer::AttackStart()
{
	mIsAttacking = true;
}

// 攻撃終了
void CPlayer::AttackEnd()
{
	mIsAttacking = false;
}

// ダメージを受ける
void CPlayer::TakeDamage(int damage, CObjectBase* causer)
{
}

// 更新
void CPlayer::Update()
{
	// 相手の方を向き続ける
	CVector lookPos = mpOpponent->Position();
	lookPos.Y(Position().Y());
	LookAt(lookPos);

	// 待機中は、移動処理を行う
	if (mState == EState::eIdle)
	{
		UpdateMove();
	}

	switch (mState)
	{
	case EState::eIdle:			UpdateIdle();			break;	// 待機
	case EState::eDamageStart:	UpdateDamageStart();	break;	// 被弾開始
	case EState::eDamage:		UpdateDamage();			break;	// 被弾中
	case EState::eDamageEnd:	UpdateDamageEnd();		break;	// 被弾終了
	case EState::eAttackStart:	UpdateAttackStart();	break;	// 攻撃開始
	case EState::eAttack:		UpdateAttack();			break;	// 攻撃中
	case EState::eAttackEnd:	UpdateAttackEnd();		break;	// 攻撃終了
	case EState::eDeath:		UpdateDeath();			break;	// 死亡
	}

	// 基底プレイヤークラスの更新
	CPlayerBase::Update();
	// 呪文詠唱クラスの更新
	CSpellCaster::Update();


#if _DEBUG
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("MoveSpeed:%f,%f,%f\n", mMoveSpeed.X(), mMoveSpeed.Y(), mMoveSpeed.Z());
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
		CVector(0.0f, BODY_RADIUS / Scale().Y(), 0.0f),
		CVector(0.0f, BODY_HEIGHT / Scale().Y(), 0.0f),
		BODY_RADIUS
	);
	// フィールド,壁、オブジェクトと敵と攻撃とだけ衝突
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject,ELayer::eEnemy,
		ELayer::eAttackCol});
}

// アクションのキー入力
void CPlayer::ActionInput()
{
	// 左クリックで攻撃へ
	if (CInput::PushKey(VK_LBUTTON))
	{
		// 攻撃開始へ
		ChangeState(EState::eAttackStart);
	}

	if (CInput::PushKey('U'))
	{
		// ボール呪文を詠唱
		CastStart(ESpellElementalType::eFire, ESpellShapeType::eBall);
	}
	if (CInput::PushKey('I'))
	{
		// ボルト呪文を詠唱
		CastStart(ESpellElementalType::eFire, ESpellShapeType::eBolt);
	}
	if (CInput::PushKey('O'))
	{
		// ブレス呪文を詠唱
		CastStart(ESpellElementalType::eFire, ESpellShapeType::eBreath);
	}

	// スペースで上移動
	if (CInput::Key(VK_SPACE))
	{
		mMoveSpeedY = GetJumpSpeed() * Times::DeltaTime();
	}
	// シフトで下移動
	else if (CInput::Key(VK_SHIFT))
	{
		mMoveSpeedY = -GetJumpSpeed() * Times::DeltaTime();
	}
	else
	{
		mMoveSpeedY = 0.0f;
	}

}

// 待機状態
void CPlayer::UpdateIdle()
{
	// キー入力可能
	ActionInput();
}

// 移動処理
void CPlayer::UpdateMove()
{
	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed = move * GetBaseMoveSpeed() * Times::DeltaTime();

		// 待機状態であれば、移動アニメーションに切り替え
		if (mState == EState::eIdle)
		{	

		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == EState::eIdle)
		{

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
	mStateStep = 0;
	mElapsedTime = 0.0f;
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
	case EState::eAttackStart:	return "攻撃開始";		break;
	case EState::eAttack:		return "攻撃中";		break;
	case EState::eAttackEnd:	return "攻撃終了";		break;
	case EState::eDeath:		return "死亡";			break;
	}

	return "エラー";
}
#endif