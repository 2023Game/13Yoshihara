#include "CPlayer.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CInput.h"
#include "CWand.h"
#include "CConnectPoint.h"
#include "CConnectPointManager.h"
#include "CConnectObject.h"

// 体の半径と高さ
#define BODY_RADIUS 2.5f
#define BODY_HEIGHT 15.0f

// 探知コライダの半径
#define SEARCH_RADIUS 100.0f

// 杖のオフセット座標と回転とスケール
#define WAND_OFFSET_POS CVector(-90.0f,8.0f,4.0f)
#define WAND_OFFSET_ROT CVector(0.0f,0.0f,90.0f)
#define WAND_SCALE	3.0f

// アニメーションのパス
#define ANIM_PATH "Character\\Adventurer\\AdventurerAnim\\"

// アニメーションデータのテーブル
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",								true,	0.0f,	1.0f},	// Tポーズ
	{ANIM_PATH"Idle.x",					true,	118.0f,	1.0f},	// 待機
	{ANIM_PATH"Idle_Wand.x",			true,	221.0f,	1.0f},	// 杖持ち待機
	{ANIM_PATH"Run.x",					true,	48.0f,	1.0f},	// 走り
	{ANIM_PATH"Run_Wand.x",				true,	43.0f,	1.0f},	// 杖持ち走り
	{ANIM_PATH"Jump.x",					false,	51.0f,	1.0f},	// ジャンプ
	{ANIM_PATH"Attack_Wand.x",			false,	61.0f,	1.5f},	// 杖攻撃
};

// コンストラクタ
CPlayer::CPlayer()
	: CPlayerBase()
	, CPlayerStatus()
	, mState(EState::eIdle)
	, mIsWand(false)
	, mIsAttacking(false)
	, mpPoint(nullptr)
{
	// アニメーションとモデルの初期化
	InitAnimationModel("Player", &ANIM_DATA);

	// コライダ―を生成
	CreateCol();

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

	// 杖を作成
	mpWand = new CWand
	(
		this,
		ETag::eWand
	);

	// 右手のフレームを取得し、
	// 杖にプレイヤーの右手の行列をアタッチ
	CModelXFrame* frame = mpModel->FinedFrame("Armature_mixamorig_RightHand");
	mpWand->SetAttachMtx(&frame->CombinedMatrix());
	mpWand->Position(WAND_OFFSET_POS);
	mpWand->Rotation(WAND_OFFSET_ROT);
	mpWand->Scale(mpWand->Scale() * WAND_SCALE);

	// 最初は非表示
	mpWand->SetEnable(false);
	mpWand->SetShow(false);
}

// デストラクタ
CPlayer::~CPlayer()
{
	// 杖が存在したら
	if (mpWand != nullptr)
	{
		// 持ち主を解除してから、削除
		mpWand->SetOwner(nullptr);
		mpWand->Kill();
	}
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
	switch (mState)
	{
	case EState::eIdle:			UpdateIdle();			break;	// 待機
	case EState::eDamageStart:	UpdateDamageStart();	break;	// 被弾開始
	case EState::eDamage:		UpdateDamage();			break;	// 被弾中
	case EState::eDamageEnd:	UpdateDamageEnd();		break;	// 被弾終了
	case EState::eJumpStart:	UpdateJumpStart();		break;	// ジャンプ開始
	case EState::eJump:			UpdateJump();			break;	// ジャンプ中
	case EState::eJumpEnd:		UpdateJumpEnd();		break;	// ジャンプ終了
	case EState::eAttackStart:	UpdateAttackStart();	break;	// 攻撃開始
	case EState::eAttack:		UpdateAttack();			break;	// 攻撃中
	case EState::eAttackEnd:	UpdateAttackEnd();		break;	// 攻撃終了
	case EState::eDeath:		UpdateDeath();			break;
	}

	// 待機中とジャンプ中は、移動処理を行う
	if (mState == EState::eIdle ||
		mState == EState::eJumpStart ||
		mState == EState::eJump ||
		mState == EState::eJumpEnd)
	{
		UpdateMove();
	}

	// 基底プレイヤークラスの更新
	CPlayerBase::Update();

	// コネクトオブジェクトのリストをクリア
	mConnectObjs.clear();

	// 杖の行列を更新
	mpWand->UpdateMtx();

#if _DEBUG
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("IsWand:%s\n", mIsWand ? "持っている" : "持っていない");
#endif
}

// 衝突処理
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);

	// コネクトオブジェクト探知用コライダーなら
	if (self == mpSearchConnectObjCol)
	{
		// コネクトオブジェクトの場合
		if (other->Owner()->Tag() == ETag::eConnectObject)
		{
			// コネクトオブジェクトクラスを取得
			CConnectObject* obj = dynamic_cast<CConnectObject*>(other->Owner());
			// 射程内にあるコネクトオブジェクトに追加
			mConnectObjs.push_back(obj);
		}
	}
}

// 杖のポインタを取得
CWand* CPlayer::GetWand()
{
	return mpWand;
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
	// フィールド,壁、オブジェクト、スイッチとだけ衝突
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject,ELayer::eSwitch });

	// コネクトオブジェクトの探知用コライダ
	mpSearchConnectObjCol = new CColliderSphere
	(
		this, ELayer::eConnectSearch,
		SEARCH_RADIUS
	);
	// コネクトオブジェクトだけ衝突
	mpSearchConnectObjCol->SetCollisionLayers({ ELayer::eObject });
}

// アクションのキー入力
void CPlayer::ActionInput()
{
	// 杖を持っているなら
	if (mIsWand)
	{
		// 左クリックで攻撃へ
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 待機状態なら
			if (mState == EState::eIdle)
			{
				// 攻撃開始へ
				ChangeState(EState::eAttackStart);
			}
			// それ以外の場合はアニメーションを変更せずに繋げる
			else
			{
				CVector hitPos = CVector::zero;
				CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
				// レイが衝突するか判定
				// 衝突したら衝突位置と接続
				if (connectPointMgr->Ray(hitPos))
				{
					// 接続部を生成
					connectPointMgr->CreateConnectPoint(hitPos);
				}
			}
		}
		CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
		// Fで接続状態のキャンセル
		if (connectPointMgr->GetWandConnect() && CInput::PushKey('F'))
		{
			connectPointMgr->SetWandConnect(false);
			connectPointMgr->DeleteLastConnectPoint();
		}
	}

	// 接地していれば
	if (mIsGrounded)
	{
		// スペースでジャンプ
		if (CInput::PushKey(VK_SPACE))
		{
			// ジャンプ開始へ
			ChangeState(EState::eJumpStart);
		}
	}

	if (CInput::PushKey('L'))
	{
		mIsWand = !mIsWand;
		mpWand->SetEnable(mIsWand);
		mpWand->SetShow(mIsWand);
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
			// 杖を持っている場合
			if (mIsWand)
			{
				// 杖持ち移動アニメーションに切り替え
				ChangeAnimation((int)EAnimType::eMove_Wand);
			}
			// 杖を持っていない場合
			else
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
			// 杖を持っている場合
			if (mIsWand)
			{
				// 杖持ち待機アニメーションに切り替え
				ChangeAnimation((int)EAnimType::eIdle_Wand);
			}
			// 杖を持っていない場合
			else
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
	switch (mStateStep)
	{
		// ジャンプ開始
	case 0:
		// ジャンプアニメーションに切り替え
		ChangeAnimation((int)EAnimType::eJump);
		// ジャンプの速度を設定
		mMoveSpeedY = GetJumpSpeed() * Times::DeltaTime();
		mStateStep++;
		break;

		// 次の状態へ
	case 1:
		// ジャンプ中状態へ
		ChangeState(EState::eJump);
		break;
	}
}

// ジャンプ中
void CPlayer::UpdateJump()
{
	switch (mStateStep)
	{
		// 地面に付いたら次の状態へ
	case 0:
		// 地面に付いたら
		if (mIsGrounded)
		{
			// ジャンプ終了状態へ
			ChangeState(EState::eJumpEnd);
		}
		break;
	}
}

// ジャンプ終了
void CPlayer::UpdateJumpEnd()
{
	switch (mStateStep)
	{
		// 待機状態へ
	case 0:
		ChangeState(EState::eIdle);
		break;
	}
}

// 攻撃開始
void CPlayer::UpdateAttackStart()
{
	switch (mStateStep)
	{
		// アニメーション変更
	case 0:
		mMoveSpeed = CVector::zero;
		// 攻撃開始アニメーション
		ChangeAnimation((int)EAnimType::eAttack);
		mStateStep++;
		break;

		// キャラを視点の方向へ向ける
	case 1:
	{
		CCamera* camera = CCamera::CurrentCamera();
		// 向く方向を求める
		CVector vec = camera->GetEyeVec();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));

		mStateStep++;
		break;
	}

		// 次の状態へ
	case 2:
		// 攻撃中状態へ
		ChangeState(EState::eAttack);
		break;
	}
}

// 攻撃中
void CPlayer::UpdateAttack()
{
	switch (mStateStep)
	{
		// アニメーションが20フレーム以降なら次へ
	case 0:
		if (GetAnimationFrame() > 20.0f)
		{
			CVector hitPos = CVector::zero;
			CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
			// レイが衝突するか判定
			// 衝突したら衝突位置と接続
			if (connectPointMgr->Ray(hitPos))
			{
				// 接続部を生成
				connectPointMgr->CreateConnectPoint(hitPos);
			}
			mStateStep++;
		}
		break;

		// 次の状態へ
	case 1:
		// 攻撃終了へ
		ChangeState(EState::eAttackEnd);
		break;
	}
}

// 攻撃終了
void CPlayer::UpdateAttackEnd()
{
	switch (mStateStep)
	{
		// アニメーションが40フレーム以降なら次へ
	case 0:
		if (GetAnimationFrame() > 40.0f)
		{
			mStateStep++;
		}
		break;

		// アニメーションが終了したら次へ
	case 1:
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;

		// 次の状態へ
	case 2:
		// 待機状態へ
		ChangeState(EState::eIdle);
		break;
	}
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
	case EState::eJumpStart:	return "ジャンプ開始";	break;
	case EState::eJump:			return "ジャンプ中";	break;
	case EState::eJumpEnd:		return "ジャンプ終了";	break;
	case EState::eAttackStart:	return "攻撃開始";		break;
	case EState::eAttack:		return "攻撃中";		break;
	case EState::eAttackEnd:	return "攻撃終了";		break;
	case EState::eDeath:		return "死亡";			break;
	}
}
#endif

// 視点の中心に一番近いオブジェクトを求める
CConnectObject* CPlayer::CenterObject()
{
	// 画面の中心を求める
	CVector2 screenCenter = CVector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	// リストの全てのオブジェクト
	for (const auto& obj : mConnectObjs)
	{
		// スクリーン座標に変換
		CVector2 screenPos = obj->Position();
	}
	return nullptr;
}
