#include "CPlayer.h"
#include "CharaStatusDefine.h"
#include "CColliderSphere.h"
#include "CInput.h"
#include "CImage.h"
#include "CFade.h"
#include "CBall.h"
#include "CGameCamera2.h"
#include "CGaugeUI2D.h"
#include "CTextUI2D.h"
#include "CEnemyManager.h"

// 体の半径
#define BODY_RADIUS 4.0f

// 詠唱文字のオフセット座標
#define SPELL_TEXT_UI_OFFSET_POS CVector(0.0f,WINDOW_HEIGHT * 0.8f, 0.0f)

#define SPELLS {"fire","thunder","wind","ball","bolt","breath","teleport","shield","reflector"}

// ゲージのパス
#define GAUGE_PATH "UI\\gauge.png"
// ゲージの座標
#define GAUGE_POS CVector(0.0f,0.0f,0.0f)
// ゲージの大きさ
#define GAUGE_SIZE 2.0f
// ゲージ同士の間隔
#define GAUGE_DIST 10.0f

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
	, CCastSpellStr(this, ECastType::eQuick, SPELLS, SPELL_TEXT_UI_OFFSET_POS)
	, mState(EState::eIdle)
	, mIsAttacking(false)
{
	// 重力無効
	mIsGravity = false;
	// 移動方向を向かない
	mIsMoveDir = false;
	// アニメーションとモデルの初期化
	//InitAnimationModel("Player", &ANIM_DATA);

	// HPゲージを設定
	mpHpGauge = new CGaugeUI2D(this, GAUGE_PATH, true, CGaugeUI2D::EGaugeType::eHpGauge);
	mpHpGauge->Size(GAUGE_SIZE);
	mpHpGauge->Position(GAUGE_POS);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());

	// MPゲージを設定
	mpMpGauge = new CGaugeUI2D(this, GAUGE_PATH, true, CGaugeUI2D::EGaugeType::eMpGauge);
	mpMpGauge->Size(GAUGE_SIZE);
	mpMpGauge->Position(GAUGE_POS + CVector(0.0f, mpMpGauge->Size().Y() + GAUGE_DIST, 0.0f));
	mpMpGauge->SetMaxPoint(GetMaxMp());
	mpMpGauge->SetCurrPoint(GetMp());

	// 呪文の文字列テキストの持ち主を設定
	mpSpellText->SetOwner(this);

	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CPlayer::~CPlayer()
{
	// MPゲージが存在したら、一緒に削除する
	if (mpMpGauge != nullptr)
	{
		mpMpGauge->SetOwner(nullptr);
		mpMpGauge->Kill();
	}
}

// 更新
void CPlayer::Update()
{
	// 相手がいれば
	if (mpOpponent != nullptr)
	{
		// 相手の方を向き続ける
		CVector lookPos = mpOpponent->Position();
		lookPos.Y(Position().Y());
		LookAt(lookPos);
	}

	// 徐々に減速
	mMoveSpeed -= mMoveSpeed * DECREASE_MOVE_SPEED;

	// 待機中は、移動処理を行う
	if (mState == EState::eIdle)
	{
		UpdateMove();
	}

	// 待機か詠唱中なら
	if (mState == EState::eIdle ||
		mState == EState::eCast)
	{
		// 詠唱のキー入力
		CastInput();
	}

	switch (mState)
	{
	case EState::eIdle:			UpdateIdle();			break;	// 待機
	case EState::eDamageStart:	UpdateDamageStart();	break;	// 被弾開始
	case EState::eDamage:		UpdateDamage();			break;	// 被弾中
	case EState::eDamageEnd:	UpdateDamageEnd();		break;	// 被弾終了
	case EState::eDeath:		UpdateDeath();			break;	// 死亡
	}
	// Mpの再生
	RegeneMp();

	// 基底プレイヤークラスの更新
	CPlayerBase::Update();
	// 詠唱呪文指定クラスの更新
	CCastSpellStr::Update();

	// HPゲージを更新
	mpHpGauge->SetCurrPoint(GetHp());
	// MPゲージ
	mpMpGauge->SetCurrPoint(GetMp());


#if _DEBUG
	CDebugPrint::Print("PlayerHp:%d\n", GetHp());
	CDebugPrint::Print("PlayerMp:%f\n", GetMp());
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
#endif
}

// プレイヤーが詠唱状態か
bool CPlayer::IsCastState() const
{
	return mState == EState::eCast ? true : false;
}

// オブジェクト削除処理
void CPlayer::DeleteObject(CObjectBase* obj)
{
	CPlayerBase::DeleteObject(obj);

	// 削除されたのが呪文のテキストなら
	// ポインタを空にする
	if (obj == mpSpellText)
	{
		mpSpellText = nullptr;
	}
}

// コライダ―を生成
void CPlayer::CreateCol()
{
	// 本体コライダ
	mpBodyCol = new CColliderSphere
	(
		this, ELayer::ePlayer,
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

// 詠唱のキー入力
void CPlayer::CastInput()
{
	switch (mCastType)
	{
		// 基本詠唱のキー入力
	case CCastSpellStr::ECastType::eBasic:
		// 詠唱中なら
		if (mState == EState::eCast)
		{
			// キー入力
			BasicCastInput();
			// エンターで呪文を発動
			if (CInput::PushKey(VK_RETURN))
			{
				// 呪文を詠唱
				CastSpell();
				//　待機へ
				ChangeState(EState::eIdle);
			}
			// バックスペースで文字列の削除
			if (CInput::PushKey(VK_BACK))
			{
				DeleteStr();
			}
		}
		// そうでないなら
		else
		{
			// エンターで詠唱中へ
			if (CInput::PushKey(VK_RETURN))
			{
				// 文字列をクリア
				CInput::ClearStr();
				ChangeState(EState::eCast);
			}
		}
		break;
		// 短縮詠唱のキー入力
	case CCastSpellStr::ECastType::eQuick:
		// キー入力
		QuickCastInput();
		// エンターで呪文を発動
		if (CInput::PushKey(VK_RETURN))
		{
			CastSpell();
		}
		// バックスペースで文字列の削除
		if (CInput::PushKey(VK_BACK))
		{
			DeleteStr();
		}
		break;
	}
}

// 基本詠唱のキー入力
void CPlayer::BasicCastInput()
{
	// 押された文字列を取得して設定
	BasicCastSpell(CInput::GetInputStr());
}

// 短縮詠唱のキー入力
void CPlayer::QuickCastInput()
{
	if (CInput::PushKey('U'))
	{
		QuickCastSpell(0);
	}
	if (CInput::PushKey('I'))
	{
		QuickCastSpell(1);
	}
	if (CInput::PushKey('O'))
	{
		QuickCastSpell(2);
	}
	if (CInput::PushKey('J'))
	{
		QuickCastSpell(3);
	}
	if (CInput::PushKey('K'))
	{
		QuickCastSpell(4);
	}
	if (CInput::PushKey('L'))
	{
		QuickCastSpell(5);
	}
	if (CInput::PushKey('M'))
	{
		QuickCastSpell(6);
	}
	if (CInput::PushKey(VK_OEM_COMMA))
	{
		QuickCastSpell(7);
	}
	if (CInput::PushKey(VK_OEM_PERIOD))
	{
		QuickCastSpell(8);
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
	case EState::eDamageStart:	return "被弾開始";		break;
	case EState::eDamage:		return "被弾中";		break;
	case EState::eDamageEnd:	return "被弾終了";		break;
	case EState::eCast:			return "詠唱中";		break;
	case EState::eDeath:		return "死亡";			break;
	}

	return "エラー";
}
#endif

// 指定方向で一番角度が近い敵をロックオンターゲットに設定
void CPlayer::ChangeLockOnTarget(EDirection dir)
{
	// ターゲット候補を取得
	std::vector<CEnemy*> targets = CEnemyManager::Instance()->GetTargets();

	switch (dir)
	{
		// 上方向
	case CPlayer::EDirection::eUp:

		break;
		// 下方向
	case CPlayer::EDirection::eDown:
		break;
		// 左方向
	case CPlayer::EDirection::eLeft:
		break;
		// 右方向
	case CPlayer::EDirection::eRight:
		break;
	}
}