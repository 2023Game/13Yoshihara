#include "CCollector.h"
#include "CDebugFieldOfView.h"
#include "CTrashPlayer.h"
#include "CPlayerBase.h"
#include "CFieldBase.h"
#include "Primitive.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "CVehicleManager.h"
#include "Maths.h"
#include "CTrashPlayer.h"

// TODO：後で消すテスト用
#include "CInput.h"

// 衝突相手の車両クラスを取得するための
// 車両のクラスのインクルード
#include "CCar.h"
#include "CGarbageTruck.h"

// コライダのインクルード
#include "CColliderCapsule.h"

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
	{ "",								 true,	  0.0f,	1.0f},	// Tポーズ
	{ ANIM_PATH"Idle.x",				 true,	 80.0f,	1.0f},	// 待機					（無）
	{ ANIM_PATH"Idle_Bag.x",			 true,	 80.0f,	1.0f},	// 待機					（有）
	{ ANIM_PATH"Move.x",				 true,	 80.0f,	1.0f},	// 移動					（無）
	{ ANIM_PATH"Move_Bag.x",			 true,	 80.0f,	1.0f},	// 移動					（有）
	{ ANIM_PATH"GetBag.x",				false,	240.0f,	1.0f},	// ゴミ袋獲得			（無）
	{ ANIM_PATH"GetBag_End.x",			false,	 60.0f,	1.0f},	// ゴミ袋獲得終了		（有）
	{ ANIM_PATH"Death.x",				false,	  5.0f,	1.0f},	// 死亡					（有無）
	{ ANIM_PATH"Attack_Start.x",		false,	 50.0f,	1.0f},	// 攻撃開始				（無）
	{ ANIM_PATH"Attack_True.x",			false,	 20.0f,	1.0f},	// 攻撃成功				（無）
	{ ANIM_PATH"Attack_False.x",		false,	 20.0f,	1.0f},	// 攻撃失敗				（無）
	{ ANIM_PATH"Attack_False_Fall.x",	 true,	 20.0f,	1.0f},	// 攻撃失敗落下中		（無）
	{ ANIM_PATH"Attack_False_StandUp.x",false,	 70.0f,	1.0f},	// 立ち上がる			（無）
};

#define BODY_RADIUS 2.5f	// 本体のコライダ―の半径
#define BODY_HEIGHT 50.0f	// 本体のコライダ―の高さ

#define FOV_ANGLE 45.0f		// 視野範囲の角度
#define FOV_LENGTH 100.0f	// 視野範囲の距離
#define EYE_HEIGHT 5.0f		// 視点の高さ

#define ROTATE_SPEED 9.0f	// 回転速度
#define ATTACK_RANGE 18.0f	// 攻撃する距離

#define PATROL_INTERVAL	0.5f	// 次の巡回ポイントに移動開始するまでの時間
#define PATROL_NEAR_DIST 10.0f	// 巡回開始時に選択される巡回ポイントの最短距離
#define IDLE_TIME 0.5f			// 待機状態の時間

#define ROAD_X_AREA 90.0f	// 車道のXの範囲

// 攻撃成功時のオフセット座標
#define ATTACK_SUCCESS_OFFSET_POSY 3.0f
// Z座標はプレイヤーの前方向のベクトルを掛けて求めるのでfloat型
#define ATTACK_SUCCESS_OFFSET_POSZ 4.0f

#define SCALE 0.2f	// スケール

// 死んだときの消えるまでの時間
#define DEATH_WAIT_TIME 2.0f
// 死んだとき退避させる場所
#define DEATH_POS CVector(100.0f,100.0f,100.0f)

//	コンストラクタ
CCollector::CCollector(bool punisher, CObjectBase* owner,
	std::vector<CNavNode*> patrolPoints)
	: CEnemyBase
	(
		FOV_ANGLE,
		FOV_LENGTH,
		{},
		EYE_HEIGHT
	)
	, CCollectorStatus(punisher)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsBag(false)
	, mIsAttackSuccess(false)
	, mIsAttacking(false)
	, mpOwner(owner)
{
	// 巡回ポイントを設定
	mpPatrolPoints = patrolPoints;
	// 基本重力の5分の1の重力
	mGravity = mGravity * 0.2f;
	// 大きさの調整
	Scale(SCALE, SCALE, SCALE);
	// アニメーションとモデルの初期化
	InitAnimationModel("Collector", &ANIM_DATA);

	// 本体のコライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eCollector,
		CVector(0.0f, BODY_RADIUS / SCALE, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS / SCALE, 0.0f),
		BODY_RADIUS
	);
	// 地形、プレイヤー、敵、回収員、攻撃、車両、キャラの探知用
	// と衝突判定をする
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy, ETag::eVehicle });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,ELayer::eCharaSearch,
		ELayer::ePlayer, ELayer::eEnemy, ELayer::eCollector, ELayer::eAttackCol, ELayer::eVehicle });

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);
}

// デストラクタ
CCollector::~CCollector()
{
	if (mpOwner != nullptr)
	{
		mpOwner = nullptr;
	}
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
	case EState::eReturn:			UpdateReturn();			break;
	case EState::eAttackStart:		UpdateAttackStart();	break;
	case EState::eAttackTrue:		UpdateAttackTrue();		break;
	case EState::eAttackFalse:		UpdateAttackFalse();	break;
	case EState::eAttackEnd:		UpdateAttackEnd();		break;
	case EState::eDeath:			UpdateDeath();			break;
	}

	// キャラクターの更新
	CEnemyBase::Update();

#if _DEBUG
	// 現在の状態に合わせて視野範囲の色を変更
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("CollectorState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("CollectorAttackSuccess:%s\n", mIsAttackSuccess ? "true" : "false");
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
			// 死んでないなら押し戻す
			if (!IsDead())
			{
				// 押し戻しベクトル
				CVector adjust = hit.adjust;
				adjust.Y(0.0f);

				// 押し戻しベクトルの分、座標を移動
				Position(Position() + adjust * hit.weight);
			}

			// 相手の持ち主と自分の持ち主が一致しているかつ、
			// 自分が撤退状態なら
			if (other->Owner() == mpOwner&&
				mState == EState::eReturn)
			{
				// 無効にする
				SetOnOff(false);
			}
		}
		// 衝突した相手がプレイヤーなら
		else if (other->Layer() == ELayer::ePlayer)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);

			// 自分が攻撃中かつ攻撃がすでに成功していない場合
			if (IsAttacking() &&
				!mIsAttackSuccess)
			{
				// プレイヤークラスを取得
				CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(other->Owner());

				// プレイヤーが存在してまだ攻撃が当たっていないかつ
				// 回収員が既についていないなら
				if (player != nullptr &&
					!IsAttackHitObj(player)&&
					!player->GetStickCollector())
				{
					AddAttackHitObj(player);
					mIsAttackSuccess = true;		// 攻撃が成功
					player->SetStickCollector(true);// 回収員がついている
					// この回収員をついている回収員に設定
					player->SetStickCollectorPointer(this);
				}
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
		// 衝突した相手が回収員なら
		else if (other->Layer() == ELayer::eCollector)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
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
		int size = mpPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawWireBox
			(
				mpPatrolPoints[i]->GetPos() + CVector(0.0f, 1.0f, 0.0f),
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

// 回収員の有効無効を切り替える
void CCollector::SetOnOff(bool setOnOff)
{
	// 無効にするとき
	if (!setOnOff)
	{
		// 退避場所に移動する
		Position(DEATH_POS);
		// ゴミ収集車のクラスを取得
		CGarbageTruck* owner = dynamic_cast<CGarbageTruck*>(mpOwner);
		// 回収員の人数を減らす
		owner->DecreaseCollectors();
	}
	// 有効にするとき
	else
	{
		// Hpをリセットする
		SetHp();
		ChangeState(EState::eIdle);
	}
	// 有効無効を設定する
	SetEnable(setOnOff);
	SetShow(setOnOff);
}

// 撤収状態に変える
void CCollector::ChangeStateReturn()
{
	// 死亡状態じゃないなら
	if (mState != EState::eDeath)
	{
		ChangeState(EState::eReturn);
	}
}

// 回収員の持ち主を設定
void CCollector::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// 回収員の持ち主を取得
CObjectBase* CCollector::GetOwner() const
{
	return mpOwner;
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
		if (mpMoveRoute.size() == 1)
		{
			mNextMoveIndex = 0;
		}
		// 最短経路の次のノードまで移動
		CNavNode* moveNode = mpMoveRoute[mNextMoveIndex];

		if (MoveTo(moveNode->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{
			// 移動が終われば、次のノードへ切り替え
			mNextMoveIndex++;
			// 最後のノード（目的地のノード）だった場合は、次のステップへ進める
			if (mNextMoveIndex >= mpMoveRoute.size())
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
		CTrashPlayer* trashPlayer = dynamic_cast<CTrashPlayer*>(player);
		// 回収員が既についていなければ攻撃状態へ
		if (!trashPlayer->GetStickCollector())
		{
			ChangeState(EState::eAttackStart);
		}
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
		if (navMgr->Navigate(mpNavNode, playerNode, mpMoveRoute))
		{
			// 自身のノードからプレイヤーのノードまで繋がっていたら、
			// 移動する位置を次のノードの位置に設定
			targetPos = mpMoveRoute[1]->GetPos();
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

		if (navMgr->Navigate(mpNavNode, mpLostPlayerNode, mpMoveRoute))
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

		// ステップ1：プレイヤーを見失った位置まで移動
	case 1:
		if (MoveTo(mpMoveRoute[mNextMoveIndex]->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{
			mNextMoveIndex++;
			if (mNextMoveIndex >= mpMoveRoute.size())
			{
				// 移動が終われば、待機状態へ移行
				ChangeState(EState::eIdle);
				mpLostPlayerNode->SetEnable(false);
			}
		}
		break;
	}
}

// ゴミ収集車に戻る処理
void CCollector::UpdateReturn()
{

	switch (mStateStep)
	{
		// ステップ0：移動アニメーションを再生
	case 0:
		// ゴミ袋を持っていないなら
		if (!mIsBag)
		{
			ChangeAnimation((int)EAnimType::eMove);
		}
		// ゴミ袋を持っているなら
		else
		{
			ChangeAnimation((int)EAnimType::eMove_Bag);
		}
		mStateStep++;
		break;

		// ステップ1：自分の持ち主とぶつかるまで移動を続ける
	case 1:
		// 持ち主の座標へ移動
		if (MoveTo(mpOwner->Position(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{

		}
		break;
	}

}

// 攻撃開始
void CCollector::UpdateAttackStart()
{


	switch (mStateStep)
	{
		// ステップ0：攻撃の開始
	case 0:
		// 攻撃開始アニメーションを再生
		ChangeAnimation((int)EAnimType::eAttack_Start);
		mStateStep++;
		break;

		// ステップ1：上方向の移動を設定
	case 1:
		// アニメーションが60%以上進行したら
		if (GetAnimationFrameRatio() >= 0.6f)
		{
			// 上方向の移動を設定
			mMoveSpeedY = GetJumpSpeed();
			// 攻撃開始
			AttackStart();
			mStateStep++;
		}
		break;

		// ステップ2：アニメーションが終了したら攻撃中へ
	case 2:
		// プレイヤーの座標
		CPlayerBase * player = CPlayerBase::Instance();
		CVector targetPos = player->Position();
		// プレイヤーの方向へ移動
		if (MoveTo(targetPos, GetBaseMoveSpeed(), ROTATE_SPEED))
		{

		}
		// 攻撃開始アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 攻撃成功なら
			if (mIsAttackSuccess)
			{
				// 攻撃中（成功）へ
				ChangeState(EState::eAttackTrue);
			}
			// 失敗なら
			else
			{
				// 攻撃中（失敗）へ
				ChangeState(EState::eAttackFalse);
			}
		}
		break;
	}
}

// 攻撃中（成功）
void CCollector::UpdateAttackTrue()
{
	// 座標をプレイヤーの前に固定
	CPlayerBase* player = CPlayerBase::Instance();
	CVector playerForward = player->VectorZ();
	CVector playerUp = player->VectorY();
	// 固定する座標を求める
	CVector lockPos = player->Position() + playerForward * ATTACK_SUCCESS_OFFSET_POSZ +
		playerUp * ATTACK_SUCCESS_OFFSET_POSY;
	Position(lockPos);
	// 向く方向をプレイヤーの方向にする
	CVector lookVec = player->Position() - Position();
	lookVec.Y(0.0f);
	Rotation(CQuaternion::LookRotation(lookVec));

	switch (mStateStep)
	{
		// ステップ0：アニメーション再生
	case 0:
	{
		// 重力を掛けない
		mIsGravity = false;
		// 攻撃成功アニメーション再生
		ChangeAnimation((int)EAnimType::eAttack_True);
		mStateStep++;
		break;
	}

		// ステップ1：次のアニメーション再生
	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// ゴミ袋獲得アニメーション再生
			ChangeAnimation((int)EAnimType::eGetBag);
			mStateStep++;
		}
		break;

		// ステップ2：ゴミ袋を手に持ち攻撃終了状態へ
	case 2:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 重力を掛ける
			mIsGravity = true;
			// TODO：ゴミ袋を表示
			// ゴミ袋を取得
			mIsBag = true;
			// 攻撃終了状態へ
			ChangeState(EState::eAttackEnd);
		}
	}
}

// 攻撃中（失敗）
void CCollector::UpdateAttackFalse()
{
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// 攻撃が成功に変わって、回収員がプレイヤーについていなかったら
	if (mIsAttackSuccess &&
		!player->GetStickCollector())
	{
		// 攻撃成功状態へ
		ChangeState(EState::eAttackTrue);
		return;
	}

	// 自分の前方を目標地点に設定
	CVector targetPos = Position() + VectorZ() * 10.0f;
	// 目標地点の方向へ移動
	if (MoveTo(targetPos, GetBaseMoveSpeed(), ROTATE_SPEED))
	{

	}

	switch (mStateStep)
	{
		// ステップ0：アニメーション再生
	case 0:
		// 攻撃失敗アニメーション再生
		ChangeAnimation((int)EAnimType::eAttack_False);
		mStateStep++;
		break;

		// ステップ1：次のアニメーション再生
	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 攻撃失敗落下中アニメーション再生
			ChangeAnimation((int)EAnimType::eAttack_False_Fall);
			mStateStep++;
		}
		break;

		// ステップ2：ゴミ袋を手に持ち攻撃終了状態へ
	case 2:
		// 接地したら
		if (mIsGrounded)
		{
			// 攻撃終了状態へ
			ChangeState(EState::eAttackEnd);
		}
	}
}

// 攻撃終了
void CCollector::UpdateAttackEnd()
{
	switch (mStateStep)
	{
	case 0:
	{
		// プレイヤーのクラスを取得
		CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
		// ついている回収員のポインタを取得
		CCollector* stickCollector = player->GetStickCollectorPointer();
		// ついている回収員と一致したら
		if (this == stickCollector)
		{
			// ついているのを解除
			player->SetStickCollector(false);
		}
		// 攻撃の判定自体はここまで
		mIsAttacking = false;
		// 攻撃が成功している場合
		if (mIsAttackSuccess)
		{
			// ゴミ袋獲得終了アニメーション再生
			ChangeAnimation((int)EAnimType::eGetBag_End);
		}
		// 失敗なら
		else
		{
			// 立ち上がるアニメーション再生
			ChangeAnimation((int)EAnimType::eAttack_False_StandUp);
		}
		mStateStep++;
		break;
	}

	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 攻撃が成功している場合
			if (mIsAttackSuccess)
			{
				// ゴミ収集車に戻る状態へ
				ChangeState(EState::eReturn);
			}
			// 失敗なら
			else
			{
				// 追跡状態へ
				ChangeState(EState::eChase);
			}
			// 攻撃終了
			AttackEnd();
		}
		break;
	}
}

// 死亡の更新処理
void CCollector::UpdateDeath()
{
	switch (mStateStep)
	{
		// ステップ0：死亡アニメーションを再生
	case 0:
		ChangeAnimation((int)EAnimType::eDeath);
		mStateStep++;
		break;

		// ステップ1：アニメーションが終了したら次のステップへ
	case 1:
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;
		// ステップ1：消えるまでの時間になるまでカウント
	case 2:
		mElapsedTime += Times::DeltaTime();
		if (mElapsedTime >= DEATH_WAIT_TIME)
		{
			mStateStep++;
		}
		break;
		// ステップ2：アニメーションが終了したら無効にする
	case 3:
		SetOnOff(false);
		break;
	}
}

// 死亡処理
void CCollector::Death()
{
	// 死亡状態へ
	ChangeState(EState::eDeath);
}


// 攻撃中か
bool CCollector::IsAttacking() const
{
	return mIsAttacking;
}

// 攻撃開始
void CCollector::AttackStart()
{
	// ベースクラスの攻撃開始処理を呼び出し
	CXCharacter::AttackStart();
	// 攻撃中
	mIsAttacking = true;
}

// 攻撃終了
void CCollector::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CXCharacter::AttackEnd();

	// 攻撃が成功したかをリセット
	mIsAttackSuccess = false;
	// 攻撃中でない
	mIsAttacking = false;
	// 重力を掛ける
	mIsGravity = true;
	// プレイヤーのクラスを取得
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// ついている回収員のポインタを取得
	CCollector* stickCollector = player->GetStickCollectorPointer();
	// ついている回収員と一致すれば
	if (this == stickCollector)
	{
		// ついているのを解除
		player->SetStickCollector(false);
	}
}

// 攻撃が成功したか
bool CCollector::GetAttackSuccess() const
{
	return mIsAttackSuccess;
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

	// 攻撃中に攻撃関連以外の状態に変わるとき
	// 攻撃終了処理を呼ぶ
	if (IsAttacking() &&
		state != EState::eAttackStart &&
		state != EState::eAttackTrue &&
		state != EState::eAttackFalse &&
		state != EState::eAttackEnd)
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
	case EState::eAttackTrue:		return "攻撃中（成功）";
	case EState::eAttackFalse:		return "攻撃中（失敗）";
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
	case EState::eAttackTrue:		return CColor::magenta;
	case EState::eAttackFalse:		return CColor::magenta;
	case EState::eAttackEnd:		return CColor::magenta;
	}
	return CColor::white;
}
#endif
