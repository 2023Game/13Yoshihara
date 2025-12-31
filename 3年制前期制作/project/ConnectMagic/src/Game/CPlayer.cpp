#include "CPlayer.h"
#include "CInput.h"
#include "CWand.h"
#include "CConnectPoint.h"
#include "CConnectPointManager.h"
#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "CImage.h"
#include "CFade.h"
#include "Maths.h"
#include "CSaveManager.h"
#include "CTaskManager.h"
#include "CPhysicsManager.h"
#include "PhysicsMaterial.h"
#include "CollisionData.h"
#include "SaveData.h"
#include "CollisionLayer.h"


// 体の半径と高さ
constexpr float BODY_RADIUS =				2.5f;
constexpr float BODY_HEIGHT =				12.5f;
// 物理設定
constexpr float MASS =						1.0f;
constexpr float MAX_SPEED =					50.0f;	// 最高速度
constexpr float ACCELERATION_FORCE =		500.0f;	// 加速度の強さ
constexpr float TURN_RATE =					0.1f;	// 回転速度
constexpr float FRICTION =					0.8f;	// 摩擦（値が高いと停止まで早くなる）
constexpr float LIN_DAMPING =				0.8f;	// 線形減衰(値が高いと滑りが小さくなる)
constexpr float ANG_DAMPING =				0.8f;	// 角減衰(値が高いと微細な回転振動を吸収する）

// 接地とみなすための、接触点の最低限のY軸法線成分
constexpr float GROUND_NORMAL_THRESHOLD =	0.7f;

constexpr float MOVE_THRESHOLD =			0.01f;

// 前方の地面確認用レイの前方への距離
constexpr float RAY_FRONT_DIST =			5.0f;

// 探知コライダの半径
constexpr float SENSOR_RADIUS =				75.0f;

// 杖のオフセット座標と回転とスケール
const CVector WAND_OFFSET_POS =				CVector(-90.0f,8.0f,4.0f);
const CVector WAND_OFFSET_ROT =				CVector(0.0f,0.0f,90.0f);
constexpr float WAND_SCALE =				3.0f;

// 減速する速度
constexpr float DECREASE_SPEED =			0.1f;
// 加速する速度
constexpr float INCREASE_SPEED =			0.5f;

// ターゲットするときの視点とオブジェクトの画面距離の最大
constexpr float TARGET_MAX_DISTANCE =		(WINDOW_HEIGHT / 2);

// ターゲット場所の画像のスケール
constexpr float TARGET_POINT_IMG_SCALE =	0.1f;

// レイのベクトル
const CVector RAY_VEC =						CVector(0, 2.0f, 0);
// レイの開始のoffset
const CVector RAY_START_OFFSET =			CVector(0.0f, 0.5f, 0.0f);
// 接地している距離
constexpr float GROUNDED_DIST =				1.0f;

// アニメーションのパス
const std::string ANIM_PATH =				"Character\\Adventurer\\AdventurerAnim\\";

// アニメーションデータのテーブル
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",								true,	0.0f,	1.0f},	// Tポーズ
	{ANIM_PATH + "Idle.x",				true,	118.0f,	1.0f},	// 待機
	{ANIM_PATH + "Idle_Wand.x",			true,	221.0f,	1.0f},	// 杖持ち待機
	{ANIM_PATH + "Run.x",				true,	48.0f,	1.0f},	// 走り
	{ANIM_PATH + "Run_Wand.x",			true,	43.0f,	1.0f},	// 杖持ち走り
	{ANIM_PATH + "Jump_Start.x",		false,	40.0f,	2.0f},	// ジャンプ開始
	{ANIM_PATH + "Jump.x",				true,	40.0f,	1.0f},	// ジャンプ中
	{ANIM_PATH + "Jump_End.x",			false,	11.0f,	2.0f},	// ジャンプ終了
	{ANIM_PATH + "Attack_Wand.x",		false,	61.0f,	1.5f},	// 杖攻撃
	{ANIM_PATH + "Swing.x",				true,	70.0f,	1.0f},	// スイング
	{ANIM_PATH + "Swing_End_Start.x",	false,	60.0f,	2.0f},	// スイング終了の開始
	{ANIM_PATH + "Swing_End.x",			true,	40.0f,	1.0f},	// スイング終了中
	{ANIM_PATH + "Swing_End_End.x",		false,	69.0f,	2.0f},	// スイング終了の終了
};

PlayerData CPlayer::SaveState()
{
	PlayerData data(Position(),
		Rotation(),
		CConnectPointManager::Instance()->GetConnectWandTarget(),
		AnimationIndex(),
		GetAnimationFrame());

	return data;
}

void CPlayer::LoadState(const PlayerData& data)
{
	Position(data.pos);
	Rotation(data.vec);
	CConnectPointManager::Instance()->EnableConnect(data.target);
	ChangeAnimation(data.animationType);
	SetAnimationFrame(data.animationFrame);
}

// コンストラクタ
CPlayer::CPlayer(const CVector& pos)
	: CPlayerBase()
	, CPlayerStatus()
	, mState(EState::eIdle)
	, mIsWand(true)
	, mIsAttacking(false)
	, mpWandPoint(nullptr)
	, mpCenterTarget(nullptr)
	, mWasGrounded(false)
	, mIsJump(false)
	, mTarzanMoveSpeed(CVector::zero)
{
	Position(pos);

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

	// ターゲットしている場所の画像
	mpTargetPointImg = new CImage
	(
		"UI\\TargetPoint.png"
	);
	mpTargetPointImg->SetSize(mpTargetPointImg->GetSize() * TARGET_POINT_IMG_SCALE);
	// 最初は非表示
	mpTargetPointImg->SetEnable(false);
	mpTargetPointImg->SetShow(false);
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
	// 接地しているかを判定
	CheckGrounded();

	// 待機中、ターザン開始は、移動処理を行う
	if (mState == EState::eIdle ||
		mState == EState::eTarzanStart ||
		mState == EState::eEdgeJumpStart ||
		mState == EState::eEdgeJump ||
		mState == EState::eEdgeJumpEnd)
	{
		UpdateMove();
	}

	// 死亡状態以外なら
	if (mState != EState::eDeath)
	{
		// キー入力
		ActionInput();
	}

	CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();

	if (!mIsGrounded &&
		mState != EState::eTarzanStart &&
		mState != EState::eTarzan &&
		mState != EState::eTarzanEnd &&
		connectPointMgr->IsWandConnectAirObject())
	{
		ChangeState(EState::eTarzan);
	}

	// 落ちるときならジャンプする
	if (mWasGrounded && !mIsGrounded) {
		ChangeState(EState::eEdgeJumpStart);
	}
	mWasGrounded = mIsGrounded;


	switch (mState)
	{
	case EState::eIdle:			UpdateIdle();			break;	// 待機
	case EState::eDamageStart:	UpdateDamageStart();	break;	// 被弾開始
	case EState::eDamage:		UpdateDamage();			break;	// 被弾中
	case EState::eDamageEnd:	UpdateDamageEnd();		break;	// 被弾終了
	case EState::eAttackStart:	UpdateAttackStart();	break;	// 攻撃開始
	case EState::eAttack:		UpdateAttack();			break;	// 攻撃中
	case EState::eAttackEnd:	UpdateAttackEnd();		break;	// 攻撃終了
	case EState::eTarzanStart:	UpdateTarzanStart();	break;	// ターザン開始
	case EState::eTarzan:		UpdateTarzan();			break;	// ターザン中
	case EState::eTarzanEnd:	UpdateTarzanEnd();		break;	// ターザン終了
	case EState::eEdgeJumpStart:UpdateEdgeJumpStart();	break;	// エッジジャンプ開始
	case EState::eEdgeJump:		UpdateEdgeJump();		break;	// エッジジャンプ中
	case EState::eEdgeJumpEnd:	UpdateEdgeJumpEnd();	break;	// エッジジャンプ終了
	case EState::eDeath:		UpdateDeath();			break;	// 死亡
	}
	
	// 基底プレイヤークラスの更新
	CPlayerBase::Update();

	// 中心に一番近いオブジェクトを求める
	CenterTarget();
	// ターゲット中の場所の画像の位置や有効無効を更新
	UpdatePointImg();

	// 杖の行列を更新
	mpWand->UpdateMtx();

#if _DEBUG
	CDebugPrint::Print("WasGrounded:%s\n", mWasGrounded ? "true" : "false");
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("IsWand:%s\n", mIsWand ? "持っている" : "持っていない");
	CDebugPrint::Print("ConnectObj:%d\n", mConnectObjs.size());
#endif

	// コネクトオブジェクトのリストをクリア
	mConnectObjs.clear();
}

// 杖のポインタを取得
CWand* CPlayer::GetWand()
{
	return mpWand;
}

// コライダ―を生成
void CPlayer::CreateCol()
{
	// 物理設定
	PhysicsMaterial material;
	material.mass = MASS;
	material.friction = FRICTION;
	material.linDamping = LIN_DAMPING;
	material.angDamping = ANG_DAMPING;

	CPhysicsManager* physicsMgr = CPhysicsManager::Instance();
	// 本体コライダー
	physicsMgr->CreateCapsuleRigidBody(
		this,
		material,
		BODY_RADIUS,
		BODY_HEIGHT,
		Position(),
		Rotation(),
		ELayer::ePlayer,
		{ ELayer::eField,ELayer::ePortal,
		ELayer::eConnectObj,ELayer::eObject,ELayer::eCrushed,
		ELayer::eSwitch,ELayer::eShield}
	);

	// 探知用コライダー
	// 接続オブジェクトだけ衝突判定
	physicsMgr->CreateSphereSensor(
		this,
		SENSOR_RADIUS,
		ELayer::eConnectSearch,
		{ ELayer::eConnectObj }
	);
}

void CPlayer::OnCollision(const CollisionData& data)
{
	// 接地判定
	if (data.otherObj->Tag() == ETag::eField)
	{
		if (data.contactNormal.Y() > GROUND_NORMAL_THRESHOLD)
		{
			mGroundNormal = data.contactNormal;
		}
	}
	// 水
	else if (data.otherObj->Tag() == ETag::eWater)
	{
		// 保存管理クラスをロード状態へ
		CSaveManager::Instance()->ChangeState(CSaveManager::EState::eLoad);
		// 死亡状態にする
		ChangeState(EState::eDeath);
		return;
	}
}

void CPlayer::OnSensorEnter(const CollisionData& data)
{
	// コネクトオブジェクト
	if (data.otherObj->Tag() == ETag::eConnectObject)
	{
		// コネクトオブジェクトクラスを取得
		CConnectObject* obj = dynamic_cast<CConnectObject*>(data.otherObj);
		// 射程内にあるコネクトオブジェクトに追加
		mConnectObjs.push_back(obj);
	}
}

void CPlayer::CheckGrounded()
{
	// 足元からレイを飛ばす
	CVector pos = Position();
	CVector rayStart = pos + RAY_START_OFFSET;
	CVector rayEnd = pos - RAY_VEC;

	CVector hitPos;

	if (CPhysicsManager::Instance()->Raycast(rayStart, rayEnd, &hitPos,
		{ELayer::eField})) {
		// 地面が見つかった
		float distance = pos.Y() - hitPos.Y();

		// 設置している距離より低いなら
		if (distance < GROUNDED_DIST) 
		{
			// 接地中とみなす
			SetGrounded(true);
		}
	}
	else {
		// 空中にいる
		SetGrounded(false);
	}
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
			// 視点の中心に近いターゲットがあるなら
			if (mpCenterTarget != nullptr)
			{
				// 接続部の管理クラス
				CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
				// 接続オブジェクトのタグが空中なら
				if (mpCenterTarget->GetConnectObj()->GetConnectObjTag() == EConnectObjTag::eAir)
				{
					// 接続部を有効
					connectPointMgr->EnableConnect(mpCenterTarget);
					// ターザン中状態へ
					ChangeState(EState::eTarzanStart);
				}
				// 空中でないなら
				else
				{
					// 攻撃開始へ
					ChangeState(EState::eAttackStart);
				}
			}
		}
		if (CInput::PullKey(VK_LBUTTON))
		{
			// 接続部管理クラス
			CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();

			// ターザン状態なら
			if (mState == EState::eTarzanStart ||
				mState == EState::eTarzan)
			{
				// 重力オン
				mIsGravity = true;
				// ターザン終了状態へ
				ChangeState(EState::eTarzanEnd);
				// 杖の接続を解除
				connectPointMgr->DisableConnect(connectPointMgr->GetConnectWandTarget());
			}
			else
			{				
				// 視点の中心に近いターゲットがあるかつ
				// 今つながっているものと違うなら
				if (mpCenterTarget != nullptr &&
					mpCenterTarget != connectPointMgr->GetConnectWandTarget())
				{
					// 攻撃開始へ
					ChangeState(EState::eAttackStart);
				}
				else
				{
					// 杖の接続を解除
					connectPointMgr->DisableConnect(connectPointMgr->GetConnectWandTarget());
					// 待機状態へ
					ChangeState(EState::eIdle);
				}
			}
		}
		// 左クリックが押されていないなら
		else if (!CInput::Key(VK_LBUTTON))
		{
			// 重力オン
			mIsGravity = true;
			// 攻撃中かターザン状態かエッジジャンプ中でないなら
			if (mState != EState::eAttackStart &&
				mState != EState::eAttack &&
				mState != EState::eTarzanStart &&
				mState != EState::eTarzan &&
				mState != EState::eTarzanEnd &&
				mState != EState::eEdgeJumpStart &&
				mState != EState::eEdgeJump &&
				mState != EState::eEdgeJumpEnd)
			{
				// 接続部管理クラス
				CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
				// 待機状態へ
				ChangeState(EState::eIdle);
				// 杖の接続を解除
				connectPointMgr->DisableConnect(connectPointMgr->GetConnectWandTarget());
			}
		}
	}
}

// 待機状態
void CPlayer::UpdateIdle()
{
}

// 移動処理
void CPlayer::UpdateMove()
{
	btRigidBody* playerBody = GetRigidBody();
	// 現在のXZ平面の速度
	btVector3 currentVelocity = playerBody->getLinearVelocity();
	currentVelocity.setY(0.0f);

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();

	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > MOVE_THRESHOLD)
	{
		// 現在の速度が最高速度に達していないか
		if (currentVelocity.length() < MAX_SPEED)
		{
			// 達していないなら、入力方向に力を加える
			CVector force = move * ACCELERATION_FORCE;
			AddForce(force);
		}
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
	// 入力していない
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

	// 攻撃を受けていない時かつ
	// 移動方向を向く設定がオンの時
	// 速度が小さすぎない
	if (!mIsDamage &&
		mIsMoveDir &&
		currentVelocity.length2() > MOVE_THRESHOLD)
	{
		// 移動方向ベクトルから回転角度を計算する
		float angle = atan2(currentVelocity.getX(), currentVelocity.getZ());

		// 目標回転を作成
		btQuaternion targetRotation(btVector3(0.0f, 1.0f, 0.0f), angle);

		// 現在の回転を取得
		btQuaternion currentRotation = playerBody->getWorldTransform().getRotation();
		// 目標回転まで補間
		btQuaternion newRotation = currentRotation.slerp(targetRotation, TURN_RATE);
		// 変換
		CQuaternion newRot = CQuaternion(newRotation.getX(), newRotation.getY(), newRotation.getZ(), newRotation.getW());
		// 回転設定
		Rotation(newRot);
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
	switch (mStateStep)
	{
		// アニメーション変更
	case 0:
		ResetForce();
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
		// アニメーションが20フレーム以降なら
	case 0:
		if (GetAnimationFrame() > 20.0f)
		{
			// 視点の中心に近いターゲットがあるなら
			if (mpCenterTarget != nullptr)
			{
				// 接続部の管理クラス
				CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
				// 接続部を有効
				connectPointMgr->EnableConnect(mpCenterTarget);
			}
			// 次へ
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

// ターザン開始
void CPlayer::UpdateTarzanStart()
{		
	// 接地したら
	if (mIsGrounded)
	{
		// 重力オン
		mIsGravity = true;
		// 待機状態へ
		ChangeState(EState::eIdle);
		return;
	}
	switch (mStateStep)
	{
		// 20フレーム進行したら
	case 0:
		mElapsedTime++;
		if (mElapsedTime > 20.0f)
		{
			// 接続部管理クラス
			CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
			// 杖の接続線の長さを設定
			connectPointMgr->SetConnectDistance();
			mStateStep++;
		}
		break;
		// ターザン中へ
	case 1:

		ChangeState(EState::eTarzan);
		break;
	}
}

// ターザン中
void CPlayer::UpdateTarzan()
{		
	// 接地したら
	if (mIsGrounded)
	{
		// 重力オン
		mIsGravity = true;
		// 待機状態へ
		ChangeState(EState::eIdle);
		return;
	}
	switch (mStateStep)
	{
		// スイングアニメーションに切り替える
	case 0:
		ResetForce();
		mTarzanMoveSpeed = CVector::zero;
		// スイングアニメーションに切り替え
		ChangeAnimation((int)EAnimType::eSwing);
		mStateStep++;
		break;

		// スイング処理
	case 1:
		// 接続部管理クラス
		CConnectPointManager * connectPointMgr = CConnectPointManager::Instance();

		// ターゲット
		CConnectTarget* target = connectPointMgr->GetConnectWandTarget();
		// ターゲットがnullなら重力オン
		if (target == nullptr)
		{
			// 重力オン
			mIsGravity = true;
			return;
		}
		// 重力オフ
		mIsGravity = false;
		// ターゲットの座標
		CVector targetPos = target->Position();
		// プレイヤーの座標
		CVector playerPos = Position();

		// ターゲットからプレイヤーの方向
		CVector dir = playerPos - targetPos;
		dir.Normalize();

		// 重力の方向
		CVector gravity = CVector(0.0f, -GRAVITY, 0.0f);
		// 線を引っ張る重力を除外
		gravity = gravity - dir * gravity.Dot(dir);

		// 移動入力
		CVector moveDir = CalcMoveVec();

		// 入力を線の垂直面に投影
		moveDir = dir.Cross(moveDir.Cross(dir));
		moveDir.Normalize();

		// 移動速度
		mTarzanMoveSpeed += moveDir * INCREASE_SPEED * Times::DeltaTime();
		// 重力を加える
		// 振り切る時の減速と戻る時の加速のため
		mTarzanMoveSpeed += gravity;

		// 少しずつ減速していく
		mTarzanMoveSpeed *= (1.0f - DECREASE_SPEED * Times::DeltaTime());

		// 線方向の速度を削除
		// ターゲットの真下での急な減速を防ぐため
		mTarzanMoveSpeed -= dir * mTarzanMoveSpeed.Dot(dir);
		// プレイヤー座標に追加
		playerPos += mTarzanMoveSpeed;

		// 新しいプレイヤー座標への方向
		dir = playerPos - targetPos;
		dir.Normalize();
		// プレイヤーの座標を線から一定の距離に保つ
		playerPos = targetPos +
			dir * connectPointMgr->GetConnectDistance();

		// 座標を設定
		Position(playerPos);
		break;
	}
}

// ターザン終了
void CPlayer::UpdateTarzanEnd()
{
	switch (mStateStep)
	{
		// 接続を解除し飛ぶ
	case 0:
	{
		// ターザンからのジャンプアニメーションに切り替え
		ChangeAnimation((int)EAnimType::eSwing_End_Start);
		// ジャンプ速度を設定
		AddImpulse(VectorY() * GetJumpSpeed());

		mStateStep++;
		break;
	}
	// ターザンからのジャンプ中アニメーションに切り替え
	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			ChangeAnimation((int)EAnimType::eSwing_End);
			mStateStep++;
		}
		// その前に地面に付いたら次へ
		else if (mIsGrounded)
		{
			mStateStep++;
		}
		break;
		// ターザン終了アニメーションに切り替え
	case 2:
		if (mIsGrounded)
		{
			mTarzanMoveSpeed = CVector::zero;
			ChangeAnimation((int)EAnimType::eSwing_End_End);
			mStateStep++;
		}
		break;
		// アニメーションが終了したら待機へ
	case 3:
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// エッジジャンプの開始
void CPlayer::UpdateEdgeJumpStart()
{
	switch (mStateStep)
	{
		// アニメーション変更
	case 0:
		mTarzanMoveSpeed = VectorZ() * GetBaseMoveSpeed() * Times::DeltaTime();
		AddForce(VectorZ() * GetBaseMoveSpeed());
		AddImpulse(VectorY() * GetJumpSpeed());
		// ジャンプ開始アニメーション
		ChangeAnimation((int)EAnimType::eJump_Start);
		mStateStep++;
		break;

		// アニメーションが終了したら次へ
	case 1:
	{
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;
	}

	// 次の状態へ
	case 2:
		// ジャンプ中状態へ
		ChangeState(EState::eEdgeJump);
		break;
	}
}

// エッジジャンプ中
void CPlayer::UpdateEdgeJump()
{
	switch (mStateStep)
	{
		// アニメーション変更
	case 0:
		ChangeAnimation((int)EAnimType::eJump);
		mStateStep++;
		break;
		// 接地したら次の状態へ
	case 1:
		if (mIsGrounded)
		{
			ChangeState(EState::eEdgeJumpEnd);
		}
		break;
	}
}

// エッジジャンプの終了
void CPlayer::UpdateEdgeJumpEnd()
{
	switch (mStateStep)
	{
		// アニメーション変更
	case 0:
		ChangeAnimation((int)EAnimType::eJump_End);
		mStateStep++;
		break;
		// アニメーションが終了したら待機へ戻る
	case 1:
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle);
		}
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
	case EState::eAttackStart:	return "攻撃開始";		break;
	case EState::eAttack:		return "攻撃中";		break;
	case EState::eAttackEnd:	return "攻撃終了";		break;
	case EState::eTarzanStart:	return "ターザン開始";	break;
	case EState::eTarzan:		return "ターザン中";	break;
	case EState::eTarzanEnd:	return "ターザン終了";	break;
	case EState::eEdgeJumpStart:return "ジャンプ開始";	break;
	case EState::eEdgeJump:		return "ジャンプ中";	break;
	case EState::eEdgeJumpEnd:	return "ジャンプ終了";	break;
	case EState::eDeath:		return "死亡";			break;
	}

	return "エラー";
}
#endif

// 視点の中心に一番近いオブジェクトを求める
void CPlayer::CenterTarget()
{
	// 最短距離
	float minDist = -1.0f;
	// 中心に一番近いターゲット
	CConnectTarget* nearTarget = nullptr;

	// 画面の中心を求める
	CVector2 screenCenter = CVector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	// リストの全てのオブジェクト
	for (const auto& obj : mConnectObjs)
	{
		// オブジェクトが視界内にないなら次へ
		CVector eyeVec = CCamera::CurrentCamera()->GetEyeVec();
		CVector objVec = obj->Position() - Position();
		float dot = eyeVec.Dot(objVec);
		if (dot < 0.0f) continue;

		// オブジェクトが持つターゲット
		CConnectTarget* target = obj->GetTarget();
		// 接続部の管理クラス
		CConnectPointManager* pointMgr = CConnectPointManager::Instance();
		// ターゲットが見える場所にないなら次へ
		if (pointMgr->RayTarget(target->Position())) continue;

		// スクリーン座標に変換
		CVector2 screenPos = CCamera::CurrentCamera()->WorldToScreenPos(target->Position());

		// 中心からの2乗の長さを求める
		float dist = (screenPos - screenCenter).LengthSqr();

		// 最短距離が不整値か、求めた距離の方が短い場合
		if (minDist == -1.0f || minDist > dist)
		{
			// 最短距離を更新
			minDist = dist;
			// ターゲットを設定
			nearTarget = target;
		}
	}

	// 最短距離がターゲットする時の画面距離の最大値の2乗より長ければ
	if (minDist > TARGET_MAX_DISTANCE * TARGET_MAX_DISTANCE)
	{
		// nullを設定
		mpCenterTarget = nullptr;
	}
	// そうでなければ
	else
	{
		// 一番近いターゲットを設定
		mpCenterTarget = nearTarget;
	}
}

// ターゲット中の画像の位置や有効無効の更新
void CPlayer::UpdatePointImg()
{
	// 中心に近いオブジェクトがある場合
	if (mpCenterTarget != nullptr)
	{
		// ターゲット中の場所を表示
		mpTargetPointImg->SetEnable(true);
		mpTargetPointImg->SetShow(true);
		// 座標を求める
		CVector2 screenPos = CCamera::CurrentCamera()->WorldToScreenPos(mpCenterTarget->Position());
		// 画像のサイズを取得
		float imgSizeX = mpTargetPointImg->GetSize().X();
		float imgSizeY = mpTargetPointImg->GetSize().Y();
		// サイズの半分を減算
		screenPos.X(screenPos.X() - imgSizeX / 2);
		screenPos.Y(screenPos.Y() - imgSizeY / 2);
		// 座標を設定
		mpTargetPointImg->SetPos(screenPos);
	}
	// ないなら
	else
	{
		// ターゲット中の場所を非表示
		mpTargetPointImg->SetEnable(false);
		mpTargetPointImg->SetShow(false);
	}
}