#include "CCollector.h"
#include "CDebugFieldOfView.h"
#include "CPlayerBase.h"
#include "CTrashPlayer.h"
#include "CFieldBase.h"
#include "Primitive.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "CVehicleManager.h"
#include "Maths.h"

// TODO：後で消すテスト用
#include "CInput.h"

// 衝突相手の車両クラスを取得するための
// 車両のクラスのインクルード
#include "CCar.h"
#include "CGarbageTruck.h"

// コライダのインクルード
#include "CColliderCapsule.h"
#include "CColliderSphere.h"

// アニメーションのパス
#define ANIM_PATH "Character\\Collector\\anim\\"
/*
 回収員のアニメーションデータのテーブル
 (有)＝袋を持っている状態のアニメーション
 (無)＝袋を持っていない状態のアニメーション
 (有無)＝両方の状態兼用のアニメーション
*/
const std::vector<CEnemyBase::AnimData> ANIM_DATA =
{
	{ "",								true,	 0.0f,	1.0f},	// Tポーズ
	{ ANIM_PATH"Idle.x",				true,	30.0f,	1.0f},	// 待機					（無）
	{ ANIM_PATH"Idle_Bag.x",			true,	30.0f,	1.0f},	// 待機					（有）
	{ ANIM_PATH"Move.x",				true,	80.0f,	1.0f},	// 移動					（無）
	{ ANIM_PATH"Move_Bag.x",			true,	80.0f,	1.0f},	// 移動					（有）
	{ ANIM_PATH"GetBag.x",				false,	10.0f,	1.0f},	// ゴミ袋獲得			（無）
	{ ANIM_PATH"GetBag_End.x",			false,	10.0f,	1.0f},	// ゴミ袋獲得終了		（有）
	{ ANIM_PATH"Death.x",				true,	11.0f,	1.0f},	// 死亡					（有無）
	{ ANIM_PATH"Attack_Start.x",		false,	51.0f,	1.0f},	// 攻撃開始				（無）
	{ ANIM_PATH"Attack_True.x",			false,	20.0f,	1.0f},	// 攻撃成功				（無）
	{ ANIM_PATH"Attack_False.x",		true,	11.0f,	1.0f},	// 攻撃失敗				（無）
	{ ANIM_PATH"Attack_False_Fall.x",	false,	21.0f,	1.0f},	// 攻撃失敗落下中		（無）
	{ ANIM_PATH"Attack_False_StandUp.x",false,	20.0f,	1.0f},	// 立ち上がる			（無）
};

#define BODY_RADIUS 2.5f	// 本体のコライダ―の半径
#define BODY_HEIGHT 25.0f	// 本体のコライダ―の高さ
#define BODY_WIDTH 50.0f	// 本体のコライダ―の幅

#define FOV_ANGLE 45.0f		// 視野範囲の角度
#define FOV_LENGTH 100.0f	// 視野範囲の距離
#define EYE_HEIGHT 5.0f		// 視点の高さ

#define ROTATE_SPEED 9.0f	// 回転速度
#define ATTACK_RANGE 18.0f	// 攻撃する距離

#define PATROL_INTERVAL	3.0f	// 次の巡回ポイントに移動開始するまでの時間
#define PATROL_NEAR_DIST 10.0f	// 巡回開始時に選択される巡回ポイントの最短距離
#define IDLE_TIME 5.0f			// 待機状態の時間

#define PATROL_POS0 CVector( 40.0f,0.0f,  0.0f)
#define PATROL_POS1 CVector( 40.0f,0.0f,100.0f)
#define PATROL_POS2 CVector(  0.0f,0.0f,100.0f)
#define PATROL_POS3 CVector(-40.0f,0.0f,100.0f)
#define PATROL_POS4 CVector(-40.0f,0.0f,  0.0f)
#define PATROL_POS5 CVector(-40.0f,0.0f,-100.0f)
#define PATROL_POS6 CVector(  0.0f,0.0f,-100.0f)
#define PATROL_POS7 CVector( 40.0f,0.0f,-100.0f)

#define ROAD_X_AREA 90.0f	// 車道のXの範囲

// 攻撃成功時のオフセット座標
#define ATTACK_SUCCESS_OFFSET_POS CVector(0.0f,0.0f,5.0f)

//	コンストラクタ
CCollector::CCollector(bool punisher)
	: CEnemyBase
	(
		FOV_ANGLE,
		FOV_LENGTH,
		{
			PATROL_POS0,
			PATROL_POS1,
			PATROL_POS2,
			PATROL_POS3,
			PATROL_POS4,
			PATROL_POS5,
			PATROL_POS6,
			PATROL_POS7
		},
		EYE_HEIGHT
	)
	, CCollectorStatus(punisher)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsBag(false)
	, mIsAttackSuccess(false)
	, mpAttackChara(nullptr)
{
	// アニメーションとモデルの初期化
	InitAnimationModel("Collector", &ANIM_DATA);

	// 地形、プレイヤー、敵、攻撃、車両、キャラの探知用
	// と衝突判定をする本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(BODY_WIDTH - BODY_RADIUS * 10, BODY_HEIGHT, 0.0f),
		CVector(-BODY_WIDTH + BODY_RADIUS * 10, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy, ETag::eVehicle });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,ELayer::eCharaSearch,
		ELayer::ePlayer, ELayer::eEnemy, ELayer::eAttackCol, ELayer::eVehicle });

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);
}

// デストラクタ
CCollector::~CCollector()
{
}

// 更新
void CCollector::Update()
{
	// 現在の状態に合わせて更新処理を切り替え
	switch (mState)
	{
	case EState::eIdle:				UpdateIdle();			break;
	case EState::ePatrol:			UpdatePatrol();			break;
	case EState::eChase:			UpdateChase();			break;
	case EState::eLost:				UpdateLost();			break;
	case EState::eAttackStart:		UpdateAttackStart();	break;
	case EState::eAttack:			UpdateAttack();			break;
	case EState::eAttackEnd:		UpdateAttackEnd();		break;
	}

	// 攻撃が成功している間はプレイヤーの前に固定
	if (mIsAttackSuccess)
	{
		CPlayerBase* player = CPlayerBase::Instance();
		Position(player->Position() + ATTACK_SUCCESS_OFFSET_POS);
	}

	// キャラクターの更新
	CEnemyBase::Update();

#if _DEBUG
	// 現在の状態に合わせて視野範囲の色を変更
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("CollectorState:%s\n", GetStateStr(mState).c_str());
#endif
}

// 衝突処理
void CCollector::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 基底クラスの衝突処理
	CEnemyBase::Collision(self, other, hit);

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
		}
		// 衝突した相手がプレイヤーなら
		else if (other->Layer() == ELayer::ePlayer)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);

			// 自分が攻撃開始状態かつ攻撃がすでに成功していない場合
			if (mState==EState::eAttackStart &&
				!mIsAttackSuccess)
			{
				// プレイヤークラスを取得
				CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(other->Owner());

				if (player != nullptr &&
					!IsAttackHitObj(player))
				{
					AddAttackHitObj(player);
					mIsAttackSuccess = true; // 攻撃が成功
				}
			}
		}
	}
}

// 描画
void CCollector::Render()
{
	CEnemyBase::Render();

	// 巡回状態であれば、
	if (mState == EState::ePatrol)
	{
		// 巡回ポイントを全て描画
		int size = mPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawWireBox
			(
				mPatrolPoints[i]->GetPos() + CVector(0.0f, 1.0f, 0.0f),
				CVector::one,
				c
			);
		}
	}
	// 見失った状態であれば、
	else if (mState == EState::eLost)
	{
		// プレイヤーを見失った位置にデバッグ表示
		Primitive::DrawWireBox
		(
			mpLostPlayerNode->GetPos() + CVector(0.0f, 1.0f, 0.0f),
			CVector::one,
			CColor::blue
		);
	}

	CPlayerBase* player = CPlayerBase::Instance();
	CFieldBase* field = CFieldBase::Instance();
	CVehicleManager* vehicleMgr = CVehicleManager::Instance();
	if (player != nullptr && field != nullptr)
	{
		CVector offsetPos = CVector(0.0f, mEyeHeight, 0.0f);
		CVector playerPos = player->Position() + offsetPos;
		CVector selfPos = Position() + offsetPos;

		// プレイヤーとの間に遮蔽物が存在する場合
		CHitInfo hit;
		bool isHit = false;
		// フィールドとのレイ判定
		if (field->CollisionRay(selfPos, playerPos, &hit))
		{
			isHit = true;
		}
		// 車両とのレイ判定
		if (vehicleMgr->CollisionRay(selfPos, playerPos, &hit, isHit))
		{
			isHit = true;
		}
		if (isHit)
		{
			// 衝突した位置まで赤線を描画
			Primitive::DrawLine
			(
				selfPos, hit.cross,
				CColor::red,
				2.0f
			);
		}
		// 遮蔽物が存在しなかった場合
		else
		{
			// プレイヤーの位置まで緑線を描画
			Primitive::DrawLine
			(
				selfPos, playerPos,
				CColor::green,
				2.0f
			);
		}
	}
}

// 待機状態
void CCollector::UpdateIdle()
{
	mMoveSpeed = CVector::zero;
	// プレイヤーの座標を取得
	CPlayerBase* player = CPlayerBase::Instance();
	CVector playerPos = player->Position();
	// プレイヤーが視野範囲内かつ、道路内なら、
	// 追跡状態へ移行
	if (IsFoundPlayer() &&
		playerPos.X() <= ROAD_X_AREA && playerPos.X() >= -ROAD_X_AREA)
	{
		ChangeState(EState::eChase);
		return;
	}

	// 待機アニメーションを再生
	if (!mIsBag)
	{
		ChangeAnimation((int)EAnimType::eIdle);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eIdle_Bag);
	}

	if (mElapsedTime < IDLE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// 待機時間が経過したら、巡回状態へ移行
		ChangeState(EState::ePatrol);
		mElapsedTime = 0.0f;
		mStateStep = 0;
	}
}

// 巡回処理
void CCollector::UpdatePatrol()
{
	// プレイヤーの座標を取得
	CPlayerBase* player = CPlayerBase::Instance();
	CVector playerPos = player->Position();
	// プレイヤーが視野範囲内かつ、道路内なら、
	// 追跡状態へ移行
	if (IsFoundPlayer() &&
		playerPos.X() <= ROAD_X_AREA && playerPos.X() >= -ROAD_X_AREA)
	{
		ChangeState(EState::eChase);
		return;
	}

	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
		// ステップ0：巡回開始時の巡回ポイントを求める
	case 0:
		mNextPatrolIndex = -1;
		ChangePatrolPoint(PATROL_NEAR_DIST);
		mStateStep++;
		break;

		// ステップ1：巡回ポイントまで移動
	case 1:
	{
		// 移動アニメーションを再生
		if (!mIsBag)
		{
			ChangeAnimation((int)EAnimType::eMove);
		}
		else
		{
			ChangeAnimation((int)EAnimType::eMove_Bag);
		}
		if (mMoveRoute.size() == 1)
		{
			mNextMoveIndex = 0;
		}
		// 最短経路の次のノードまで移動
		CNavNode* moveNode = mMoveRoute[mNextMoveIndex];

		if (MoveTo(moveNode->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{
			// 移動が終われば、次のノードへ切り替え
			mNextMoveIndex++;
			// 最後のノード（目的地のノード）だった場合は、次のステップへ進める
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				mStateStep++;
			}
		}
		break;
	}
	// ステップ2：移動後の待機
	case 2:
		mMoveSpeed = CVector::zero;
		// 待機アニメーションを再生
		if (!mIsBag)
		{
			ChangeAnimation((int)EAnimType::eIdle);
		}
		else
		{
			ChangeAnimation((int)EAnimType::eIdle_Bag);
		}

		if (mElapsedTime < PATROL_INTERVAL)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			ChangePatrolPoint(PATROL_NEAR_DIST);
			mStateStep = 1;
			mElapsedTime = 0.0f;
		}
	}
}

// 追跡処理
void CCollector::UpdateChase()
{
	// プレイヤーの座標へ向けて移動する
	CPlayerBase* player = CPlayerBase::Instance();
	CVector targetPos = player->Position();
	// プレイヤーの座標が道路外なら追いかけるのをやめる
	if (targetPos.X() >= ROAD_X_AREA ||
		targetPos.X() <= -ROAD_X_AREA)
	{
		ChangeState(EState::eIdle);
		return;
	}
	// プレイヤーが見えなくなったら、見失った状態へ移行
	if (!IsLookPlayer())
	{
		// 見失った位置にノードを配置
		mpLostPlayerNode->SetPos(targetPos);
		mpLostPlayerNode->SetEnable(true);
		ChangeState(EState::eLost);
		mStateStep = 0;
		return;
	}
	// プレイヤーに攻撃できるならば、攻撃状態へ移行
	if (CanAttackPlayer(ATTACK_RANGE))
	{
		ChangeState(EState::eAttackStart);
		return;
	}

	// 移動アニメーションを再生
	if (!mIsBag)
	{
		ChangeAnimation((int)EAnimType::eMove);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eMove_Bag);
	}

	// 経路探索管理クラスが存在すれば
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// 経路探索用のノードの座標を更新
		mpNavNode->SetPos(Position());

		// 自身のノードからプレイヤーのノードまでの最短経路を求める
		CNavNode* playerNode = player->GetNavNode();
		if (navMgr->Navigate(mpNavNode, playerNode, mMoveRoute))
		{
			// 自身のノードからプレイヤーのノードまで繋がっていたら、
			// 移動する位置を次のノードの位置に設定
			targetPos = mMoveRoute[1]->GetPos();
		}
	}
	// 移動処理
	if (MoveTo(targetPos, GetBaseMoveSpeed(), ROTATE_SPEED))
	{

	}
}

// 見失う処理
void CCollector::UpdateLost()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr)
	{
		ChangeState(EState::eIdle);
		return;
	}
	// プレイヤーが見えたら、追跡状態へ移行
	if (IsLookPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	// 移動アニメーションを再生
	if (!mIsBag)
	{
		ChangeAnimation((int)EAnimType::eMove);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eMove_Bag);
	}

	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
		// ステップ0：見失った位置までの最短経路を求める
	case 0:
		// 経路探索用のノードの座標を更新
		mpNavNode->SetPos(Position());

		if (navMgr->Navigate(mpNavNode, mpLostPlayerNode, mMoveRoute))
		{
			// 見失った位置まで経路が繋がっていたら、次のステップへ
			mNextMoveIndex = 1;
			mStateStep++;
		}
		else
		{
			// 経路が繋がっていなければ、待機状態へ戻す
			ChangeState(EState::eIdle);
			mpLostPlayerNode->SetEnable(false);
		}
		break;
	case 1:
		// プレイヤーを見失った位置まで移動
		if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{
			mNextMoveIndex++;
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				// 移動が終われば、待機状態へ移行
				ChangeState(EState::eIdle);
				mpLostPlayerNode->SetEnable(false);
			}
		}
		break;
	}
}

// TODO：ゴミ収集車に戻る処理
void CCollector::UpdateReturn()
{
}

// 攻撃開始
void CCollector::UpdateAttackStart()
{
	// プレイヤーの座標
	CPlayerBase* player = CPlayerBase::Instance();
	CVector targetPos = player->Position();
	// プレイヤーの方向へ移動
	if (MoveTo(targetPos, GetBaseMoveSpeed(), ROTATE_SPEED))
	{

	}
	switch (mStateStep)
	{
	case 0:
		// 上方向の移動を設定
		mMoveSpeedY = GetJumpSpeed();
		// 攻撃開始
		AttackStart();
		// 攻撃開始アニメーションを再生
		ChangeAnimation((int)EAnimType::eAttack_Start);
		mStateStep++;
		break;

	case 1:
		// 攻撃開始アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 攻撃中へ
			ChangeState(EState::eAttack);
		}
		break;
	}
}

// 攻撃中
void CCollector::UpdateAttack()
{
	switch (mStateStep)
	{
	case 0:
		// 攻撃が成功している場合
		if (mIsAttackSuccess)
		{
			// 攻撃成功アニメーション再生
			ChangeAnimation((int)EAnimType::eAttack_True);
		}
		// 失敗なら
		else
		{
			// 攻撃失敗アニメーション再生
			ChangeAnimation((int)EAnimType::eAttack_False);
		}
		mStateStep++;
		break;

	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 攻撃が成功している場合
			if (mIsAttackSuccess)
			{
				// ゴミ袋獲得アニメーション再生
				ChangeAnimation((int)EAnimType::eGetBag);
			}
			// 失敗なら
			else
			{
				// 攻撃失敗落下中アニメーション再生
				ChangeAnimation((int)EAnimType::eAttack_False_Fall);
			}
			mStateStep++;
		}
		break;

	case 2:
		// 攻撃が成功している場合
		if (mIsAttackSuccess)
		{
			// アニメーションが終了したら
			if (IsAnimationFinished())
			{
				// TODO：ゴミ袋を表示
				// ゴミ袋を取得
				mIsBag = true;
				// 攻撃終了状態へ
				ChangeState(EState::eAttackEnd);
			}
		}
		// 失敗なら
		else
		{
			// 地面に付いたら
			if (mIsGrounded)
			{
				// 攻撃終了状態へ
				ChangeState(EState::eAttackEnd);
			}
		}
	}
}

// 攻撃終了
void CCollector::UpdateAttackEnd()
{
	switch (mStateStep)
	{
	case 0:
		// 攻撃終了
		AttackEnd();
		// 攻撃が成功している場合
		if (mIsAttackSuccess)
		{
			// ゴミ袋獲得終了アニメーション再生
			ChangeAnimation((int)EAnimType::eGetBag_End);
		}
		// 失敗なら
		else
		{
			// 攻撃終了アニメーション再生
			ChangeAnimation((int)EAnimType::eAttack_False_StandUp);
		}
		mStateStep++;
		break;

	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// ゴミ収集車に戻る状態へ
			ChangeState(EState::eReturn);
		}
		break;
	}
}

// TODO：死亡処理
void CCollector::Death()
{
}


// 攻撃中か
bool CCollector::IsAttacking() const
{
	// 攻撃中
	if (mState == EState::eAttack) return true;

	// 攻撃中でない
	return false;
}

// 攻撃開始
void CCollector::AttackStart()
{
	// ベースクラスの攻撃開始処理を呼び出し
	CXCharacter::AttackStart();
}

// 攻撃終了
void CCollector::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CXCharacter::AttackEnd();
}

// ダメージを受ける
void CCollector::TakeDamage(int damage, CObjectBase* causer)
{
	CCharaStatusBase::TakeDamage(damage, causer);
}

// 死んでいるかどうか
bool CCollector::IsDead()
{
	return IsDeath();
}


// 状態切り替え
void CCollector::ChangeState(EState state)
{
	// 既に同じ状態であれば、処理しない
	if (state == mState) return;

	// 攻撃中に他の状態に変わるとき
	// 攻撃終了処理を呼ぶ
	if (IsAttacking())
	{
		AttackEnd();
	}

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

#if _DEBUG
// 状態の文字列を取得
std::string CCollector::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return "待機";
	case EState::ePatrol:			return "巡回";
	case EState::eChase:			return "追跡";
	case EState::eLost:				return "見失う";
	case EState::eReturn:			return "ゴミ収集車に戻る";
	case EState::eAttackStart:		return "攻撃開始";
	case EState::eAttack:			return "攻撃中";
	case EState::eAttackEnd:		return "攻撃終了";
	}
	return "";
}

// 状態の色を取得
CColor CCollector::GetStateColor(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return CColor::white;
	case EState::ePatrol:			return CColor::green;
	case EState::eChase:			return CColor::red;
	case EState::eLost:				return CColor::yellow;
	case EState::eReturn:			return CColor::blue;
	case EState::eAttackStart:		return CColor::magenta;
	case EState::eAttack:			return CColor::magenta;
	case EState::eAttackEnd:		return CColor::magenta;
	}
	return CColor::white;
}
#endif
