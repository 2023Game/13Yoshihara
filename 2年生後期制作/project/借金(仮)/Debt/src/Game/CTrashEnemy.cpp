#include "CTrashEnemy.h"
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
#define ANIM_PATH "Character\\TrashBox\\anim\\"
/*
 敵のアニメーションデータのテーブル
 (と)＝蓋が閉じている状態からのアニメーション
 (開)＝蓋が開いている状態からのアニメーション
 (開閉)＝両方の状態兼用のアニメーション
*/
const std::vector<CEnemyBase::AnimData> ANIM_DATA =
{
	{ "",								true,	 0.0f,	1.0f},	// Tポーズ
	{ ANIM_PATH"Idle_Close.x",			true,	30.0f,	1.0f},	// 待機					（と）
	{ ANIM_PATH"Idle_Open.x",			true,	30.0f,	1.0f},	// 待機					（開）
	{ ANIM_PATH"Move_Close.x",			true,	80.0f,	1.0f},	// 移動					（と）
	{ ANIM_PATH"Move_Open.x",			true,	80.0f,	1.0f},	// 移動					（開）
	{ ANIM_PATH"Damage_Close_Start.x",	false,	10.0f,	1.0f},	// 被弾開始				（と）
	{ ANIM_PATH"Damage_Open_Start.x",	false,	10.0f,	1.0f},	// 被弾開始				（開）
	{ ANIM_PATH"Damage.x",				true,	11.0f,	1.0f},	// 被弾ノックバック		（開閉）
	{ ANIM_PATH"Damage_End.x",			false,	51.0f,	1.0f},	// 被弾終了				（開閉）
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
	{ ANIM_PATH"Open.x",				false,	10.0f,	1.0f},	// 蓋を開く				（と）
	{ ANIM_PATH"Close.x",				false,	10.0f,	1.0f},	// 蓋を閉じる			（開）
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

// 攻撃コライダ―
#define ATTACK_COL_RADIUS	2.0f	// 半径
#define ATTACK_COL_HEIGHT	25.0f	// 高さ
#define ATTACK_COL_WIDTH	70.0f	// 幅
// オフセット座標
#define ATTACK_COL_OFFSET_POS CVector(0.0f,0.0f,100.0f)

// クリティカル攻撃コライダー
#define CRITICAL_COL_RADIUS		3.0f		// 半径
#define CRITICAL_COL_HEIGHT		25.0f		// 高さ
#define CRITICAL_COL_WIDTH		140.0f		// 幅
// オフセット座標
#define CRITICAL_COL_OFFSET_POS CVector(0.0f,0.0f,160.0f)

// コンストラクタ
CTrashEnemy::CTrashEnemy(bool punisher, float scale)
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
	, CTrashEnemyStatus(punisher)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsOpen(false)
	, mIsJump(false)
{
	Scale(scale, scale, scale);
	// アニメーションとモデルの初期化
	InitAnimationModel("TrashEnemy", &ANIM_DATA);

	// 地形、プレイヤー、敵、回収員、攻撃、車両、キャラの探知用
	// と衝突判定をする本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(BODY_WIDTH - BODY_RADIUS / scale, BODY_HEIGHT, 0.0f),
		CVector(-BODY_WIDTH + BODY_RADIUS / scale, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy, ETag::eVehicle });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,ELayer::eCharaSearch,
		ELayer::ePlayer, ELayer::eEnemy,ELayer::eCollector, ELayer::eAttackCol, ELayer::eVehicle});

	// 攻撃コライダー
	mpAttackCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, ATTACK_COL_HEIGHT, ATTACK_COL_WIDTH - ATTACK_COL_RADIUS * 10),
		CVector(0.0f, ATTACK_COL_HEIGHT, -ATTACK_COL_WIDTH + ATTACK_COL_RADIUS * 10),
		ATTACK_COL_RADIUS
	);
	// クリティカル攻撃コライダー
	mpCriticalCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, CRITICAL_COL_HEIGHT, CRITICAL_COL_WIDTH - CRITICAL_COL_RADIUS * 10),
		CVector(0.0f, CRITICAL_COL_HEIGHT, -CRITICAL_COL_WIDTH + CRITICAL_COL_RADIUS * 10),
		CRITICAL_COL_RADIUS
	);

	// プレイヤーと衝突判定するように設定
	mpAttackCol->SetCollisionTags({ ETag::ePlayer});
	mpAttackCol->SetCollisionLayers({ ELayer::ePlayer});
	mpCriticalCol->SetCollisionTags({ ETag::ePlayer});
	mpCriticalCol->SetCollisionLayers({ ELayer::ePlayer});

	// 自分の前に位置調整
	mpAttackCol->Position(ATTACK_COL_OFFSET_POS);
	mpCriticalCol->Position(CRITICAL_COL_OFFSET_POS);

	// 攻撃コライダーは最初はオフにしておく
	mpAttackCol->SetEnable(false);
	mpCriticalCol->SetEnable(false);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle_Close);
}

// デストラクタ
CTrashEnemy::~CTrashEnemy()
{
	SAFE_DELETE(mpCriticalCol);
}

// 更新
void CTrashEnemy::Update()
{
	// 現在の状態に合わせて更新処理を切り替え
	switch (mState)
	{
	case EState::eIdle:				UpdateIdle();			break;
	case EState::ePatrol:			UpdatePatrol();			break;
	case EState::eChase:			UpdateChase();			break;
	case EState::eLost:				UpdateLost();			break;
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

	// キャラクターの更新
	CEnemyBase::Update();

#if _DEBUG
	// 現在の状態に合わせて視野範囲の色を変更
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("EnemyIsOpen:%s\n", mIsOpen ? "true" : "false");
	CDebugPrint::Print("EnemyIsJump:%s\n", mIsJump ? "true" : "false");
	CDebugPrint::Print("EnemyHp:%d\n", GetHp());
#endif
}

// 衝突処理
void CTrashEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
	// 攻撃コライダー
	else if (self == mpAttackCol)
	{
		// 衝突した相手がプレイヤーなら
		if (other->Layer() == ELayer::ePlayer)
		{
			// プレイヤークラスを取得
			CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(other->Owner());

			if (player != nullptr &&
				!IsAttackHitObj(player))
			{
				AddAttackHitObj(player);
				// 自分から相手の方向
				CVector direction = player->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// 相手が受けるノックバック速度に、
				// 自分が与えるノックバック速度を自分から相手の方向に設定
				player->SetKnockbackReceived(direction * GetKnockbackDealt());
				// 攻撃力分のダメージを与える
				player->TakeDamage(GetAttackPower(), this);
			}
		}
	}
	// クリティカル攻撃コライダ―
	else if (self == mpCriticalCol)
	{
		// 衝突した相手がプレイヤーなら
		if (other->Layer() == ELayer::ePlayer)
		{
			// プレイヤークラスを取得
			CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(other->Owner());

			if (player != nullptr &&
				!IsAttackHitObj(player))
			{
				AddAttackHitObj(player);
				// 自分から相手の方向
				CVector direction = player->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// 相手が受けるノックバック速度に、
				// 自分が与えるノックバック速度の2倍を自分から相手の方向に設定
				player->SetKnockbackReceived(direction * GetKnockbackDealt() * 2.0f);
				// 攻撃力分のダメージを与える
				player->TakeCritical(GetAttackPower(), this);
			}
		}
	}
}

// 描画
void CTrashEnemy::Render()
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
				selfPos,playerPos,
				CColor::green,
				2.0f
			);
		}
	}
}

// 待機状態
void CTrashEnemy::UpdateIdle()
{
	mMoveSpeed = CVector::zero;
	// プレイヤーの座標を取得
	CPlayerBase* player = CPlayerBase::Instance();
	CVector playerPos = player->Position();
	// プレイヤーが視野範囲内かつ、道路内なら、
	// 追跡状態へ移行
	if (IsFoundPlayer()&&
		playerPos.X() <= ROAD_X_AREA && playerPos.X() >= -ROAD_X_AREA)
	{
		ChangeState(EState::eChase);
		return;
	}

	// 待機アニメーションを再生
	if (!mIsOpen)
	{
		ChangeAnimation((int)EAnimType::eIdle_Close);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eIdle_Open);
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
void CTrashEnemy::UpdatePatrol()
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
		if (!mIsOpen)
		{
			ChangeAnimation((int)EAnimType::eMove_Close);
		}
		else
		{
			ChangeAnimation((int)EAnimType::eMove_Open);
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
		if (!mIsOpen)
		{
			ChangeAnimation((int)EAnimType::eIdle_Close);
		}
		else
		{
			ChangeAnimation((int)EAnimType::eIdle_Open);
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
void CTrashEnemy::UpdateChase()
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
		// 1から100までの100個の数から乱数を取得
		int random = Math::Rand(1, 100);
		// クリティカル確率以下の値ならクリティカル攻撃
		if (random <= GetCriticalChance())
		{
			ChangeState(EState::eCriticalStart);
		}
		// それ以外の時は通常攻撃
		else
		{
			ChangeState(EState::eAttackStart);
		}
		return;
	}

	// 移動アニメーションを再生
	if (!mIsOpen)
	{
		ChangeAnimation((int)EAnimType::eMove_Close);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eMove_Open);
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
void CTrashEnemy::UpdateLost()
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
	if (!mIsOpen)
	{
		ChangeAnimation((int)EAnimType::eMove_Close);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eMove_Open);
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

// 被弾開始
void CTrashEnemy::UpdateDamageStart()
{
	switch (mStateStep)
	{
	case 0:
		// ジャンプ中に攻撃されたときのためfalse
		mIsJump = false;
		mIsDamage = true;
		mMoveSpeed = CVector::zero;
		// 閉じていたら
		if (!mIsOpen)
		{
			// 閉じた被弾開始アニメーション再生
			ChangeAnimation((int)EAnimType::eDamage_Close_Start);
		}
		// 開いていたら
		else
		{
			// 開いた被弾開始アニメーション再生
			ChangeAnimation((int)EAnimType::eDamage_Open_Start);
		}
		mStateStep++;
		break;

	case 1:
		// ノックバック時の飛び上がりの速度を
		// 受けるノックバック速度の半分に設定
		mMoveSpeedY = GetKnockbackReceived().Length() * 0.5f;
		// 受けるノックバック速度を移動に設定
		mMoveSpeed = GetKnockbackReceived();
		mIsGrounded = false;

		// 被弾状態へ
		ChangeState(EState::eDamage);
		break;
	}
}

// 被弾ノックバック
void CTrashEnemy::UpdateDamage()
{
	switch (mStateStep)
	{
	case 0:
		// 開いている
		mIsOpen = true;
		// 被弾アニメーション再生
		ChangeAnimation((int)EAnimType::eDamage);
		mStateStep++;
		break;

	case 1:
		// 地面についたら
		if (mIsGrounded)
		{
			// 被弾終了状態へ
			ChangeState(EState::eDamageEnd);
		}
		break;
	}
}

// 被弾終了
void CTrashEnemy::UpdateDamageEnd()
{
	switch (mStateStep)
	{
	case 0:
		// 受けるノックバックをリセット
		SetKnockbackReceived(CVector::zero);
		// 移動をゼロ
		mMoveSpeed = CVector::zero;
		// 被弾終了アニメーション再生
		ChangeAnimation((int)EAnimType::eDamage_End);
		mStateStep++;
		break;

	case 1:
		// アニメーションが60%進行したら
		if (GetAnimationFrameRatio() >= 0.60f)
		{
			mIsDamage = false;
			mStateStep++;
		}
		break;

	case 2:
		// アニメーションが終了したら待機へ
		if (IsAnimationFinished())
		{
			// 攻撃してきた相手がプレイヤーなら追跡状態へ
			if (mDamageCauser = CPlayerBase::Instance())
			{
				ChangeState(EState::eChase);
			}
			// それ以外なら待機状態へ
			else
			{
				ChangeState(EState::eIdle);
			}
			// 攻撃してきた相手の記憶をリセットする
			mDamageCauser = nullptr;
		}
		break;
	}
}

// ジャンプ開始
void CTrashEnemy::UpdateJumpStart()
{
	switch (mStateStep)
	{
	case 0:
		// ジャンプ開始時は移動をゼロ
		mMoveSpeed = CVector::zero;
		// 閉じていたら
		if (!mIsOpen)
		{
			// 閉じているジャンプアニメーション再生
			ChangeAnimation((int)EAnimType::eJump_Close_Start);
		}
		// 開いていたら
		else
		{
			// 開いているジャンプアニメーション再生
			ChangeAnimation((int)EAnimType::eJump_Open_Start);
		}
		mStateStep++;
		break;

	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// ジャンプ速度の設定
			mMoveSpeedY = GetJumpSpeed();
			mIsGrounded = false;
			mIsJump = true;
			// ジャンプ中状態へ
			ChangeState(EState::eJump);
		}
		break;
	}
}

// ジャンプ中
void CTrashEnemy::UpdateJump()
{
	switch (mStateStep)
	{
	case 0:
		// 閉じていたら
		if (!mIsOpen)
		{
			// 閉じているジャンプ中アニメーション再生
			ChangeAnimation((int)EAnimType::eJump_Close);
		}
		// 開いていたら
		else
		{
			// 開いているジャンプ中アニメーション再生
			ChangeAnimation((int)EAnimType::eJump_Open);
		}
		mStateStep++;
		break;

	case 1:
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
void CTrashEnemy::UpdateJumpEnd()
{
	switch (mStateStep)
	{
	case 0:
		mIsJump = false;
		// 移動をゼロ
		mMoveSpeed = CVector::zero;
		// 閉じていたら
		if (!mIsOpen)
		{
			// 閉じているジャンプ終了アニメーション再生
			ChangeAnimation((int)EAnimType::eJump_Close_End);
		}
		// 開いていたら
		else
		{
			// 開いているジャンプ終了アニメーション再生
			ChangeAnimation((int)EAnimType::eJump_Open_End);
		}
		mStateStep++;
		break;

	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 待機状態へ
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// 攻撃開始
void CTrashEnemy::UpdateAttackStart()
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
		// 閉じているとき
		if (!mIsOpen)
		{
			// 開くアニメーション再生
			ChangeAnimation((int)EAnimType::eOpen);
			// 次のステップへ
			mStateStep++;
		}
		// 開いているとき
		else
		{
			// 攻撃開始アニメーション再生
			ChangeAnimation((int)EAnimType::eAttack_Start);
			// ステップ2へ
			mStateStep = 2;
		}
		break;

	case 1:
		// 開くアニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 攻撃開始アニメーションを再生
			ChangeAnimation((int)EAnimType::eAttack_Start);
			mStateStep++;
		}
		break;

	case 2:
		// 攻撃開始アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 攻撃中へ
			ChangeState(EState::eAttack);
			mMoveSpeed = CVector::zero;
		}
		break;
	}
}

// 攻撃中
void CTrashEnemy::UpdateAttack()
{
	switch (mStateStep)
	{
	case 0:
		// 攻撃アニメーション再生
		ChangeAnimation((int)EAnimType::eAttack);
		AttackStart();
		mStateStep++;
		break;

	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 攻撃終了へ
			ChangeState(EState::eAttackEnd);
			AttackEnd();
		}
		break;
	}
}

// 攻撃終了
void CTrashEnemy::UpdateAttackEnd()
{
	switch (mStateStep)
	{
	case 0:
		mIsOpen = true;
		// 攻撃終了アニメーション再生
		ChangeAnimation((int)EAnimType::eAttack_End);
		mStateStep++;
		break;

	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 追跡状態へ
			ChangeState(EState::eChase);
		}
		break;
	}
}

// クリティカル攻撃開始
void CTrashEnemy::UpdateCriticalStart()
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
		// 閉じているとき
		if (!mIsOpen)
		{
			// 開くアニメーション再生
			ChangeAnimation((int)EAnimType::eOpen);
			// 次のステップへ
			mStateStep++;
		}
		// 開いているとき
		else
		{
			// クリティカル開始アニメーション再生
			ChangeAnimation((int)EAnimType::eCritical_Start);
			// ステップ2へ
			mStateStep = 2;
		}
		break;

	case 1:
		// 開くアニメーションが終了したら
		if (IsAnimationFinished())
		{
			// クリティカル開始アニメーションを再生
			ChangeAnimation((int)EAnimType::eCritical_Start);
			mStateStep++;
		}
		break;

	case 2:
		// クリティカル開始アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// クリティカル中へ
			ChangeState(EState::eCritical);
			mMoveSpeed = CVector::zero;
		}
		break;
	}
}

// クリティカル攻撃中
void CTrashEnemy::UpdateCritical()
{
	switch (mStateStep)
	{
	case 0:
		// クリティカルアニメーション再生
		ChangeAnimation((int)EAnimType::eCritical);
		AttackStart();
		mStateStep++;
		break;

	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// クリティカル終了へ
			ChangeState(EState::eCriticalEnd);
		}
		break;
	}
}

// クリティカル攻撃終了
void CTrashEnemy::UpdateCriticalEnd()
{
	switch (mStateStep)
	{
	case 0:
		// クリティカル終了アニメーション再生
		ChangeAnimation((int)EAnimType::eCritical_End);
		AttackEnd();
		mStateStep++;
		break;

	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 追跡状態へ
			ChangeState(EState::eChase);
		}
		break;
	}
}

// 蓋を開閉する
void CTrashEnemy::UpdateOpenClose()
{
	switch (mStateStep)
	{
	case 0:
		// ジャンプしていなければ
		if (!mIsJump)
		{
			// 移動をゼロ
			mMoveSpeed = CVector::zero;
		}

		// 閉じていたら
		if (!mIsOpen)
		{
			// 開くアニメーション再生
			ChangeAnimation((int)EAnimType::eOpen);
		}
		// 開いていたら
		else
		{
			// 閉じるアニメーション再生
			ChangeAnimation((int)EAnimType::eClose);
		}
		mStateStep++;
		break;

	case 1:
		// アニメーションが50%以上進行したら
		if (GetAnimationFrameRatio() >= 0.50f)
		{
			// 開き状態を変更
			mIsOpen = !mIsOpen;
			mStateStep++;
		}
		break;

	case 2:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// ジャンプしていない
			if (!mIsJump)
			{
				// プレイヤーに攻撃されていたら
				if (mDamageCauser == CPlayerBase::Instance())
				{
					// 追跡状態へ
					ChangeState(EState::eChase);
					// 攻撃してきた相手の記憶をリセット
					mDamageCauser = nullptr;
				}
				// それ以外なら
				else
				{
					// 待機状態へ
					ChangeState(EState::eIdle);
				}
			}
			// ジャンプしているならジャンプへ戻る
			else
			{
				// ジャンプ状態へ
				ChangeState(EState::eJump);
			}
		}
		break;
	}
}

// TODO：死亡処理
void CTrashEnemy::Death()
{
}


// 攻撃中か
bool CTrashEnemy::IsAttacking() const
{
	// 攻撃中
	if (mState == EState::eAttack) return true;
	// クリティカル攻撃中
	if (mState == EState::eCritical) return true;

	// 攻撃中でない
	return false;
}

// 攻撃開始
void CTrashEnemy::AttackStart()
{
	// ベースクラスの攻撃開始処理を呼び出し
	CXCharacter::AttackStart();

	// 攻撃中なら、攻撃コライダーをオン
	if (mState == EState::eAttack)
	{
		mpAttackCol->SetEnable(true);
	}
	// クリティカル攻撃中なら、クリティカルコライダ―をオン
	else if (mState == EState::eCritical)
	{
		mpCriticalCol->SetEnable(true);
	}
}

// 攻撃終了
void CTrashEnemy::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CXCharacter::AttackEnd();

	// 攻撃コライダーをオフ
	mpAttackCol->SetEnable(false);
	mpCriticalCol->SetEnable(false);
}

// ダメージを受ける
void CTrashEnemy::TakeDamage(int damage, CObjectBase* causer)
{
	// 開いていればダメージを受ける
	if (mIsOpen)
	{
		CCharaStatusBase::TakeDamage(damage, causer);

		// 死亡していなければ
		if (!IsDeath())
		{
			// 攻撃を加えた相手の方向へ向く
			CVector targetPos = causer->Position();
			CVector vec = targetPos - Position();
			vec.Y(0.0f);
			Rotation(CQuaternion::LookRotation(vec.Normalized()));

			// 被弾開始状態へ移行
			ChangeState(EState::eDamageStart);
		}
	}
	// 閉じていればノーダメージで蓋が開く
	else
	{
		// 開閉状態へ移行
		ChangeState(EState::eOpenClose);
	}
	mDamageCauser = causer;
}

// クリティカルダメージを受ける
void CTrashEnemy::TakeCritical(int damage, CObjectBase* causer)
{
	// 開いていれば2倍のダメージを受ける
	if (mIsOpen)
	{
		// 攻撃力の2倍のダメージ
		int CriticalDamage = damage * 2;
		CCharaStatusBase::TakeDamage(CriticalDamage, causer);
	}
	// 閉じていても通常のダメージを受ける
	else
	{
		// TODO：Death()の処理を追加
		CCharaStatusBase::TakeDamage(damage, causer);
	}

	// 死亡していなければ
	if (!IsDeath())
	{
		// 攻撃を加えた相手の方向へ向く
		CVector targetPos = causer->Position();
		CVector vec = targetPos - Position();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));

		// 被弾開始状態へ移行
		ChangeState(EState::eDamageStart);
	}
}

// 死んでいるかどうか
bool CTrashEnemy::IsDead()
{
	return IsDeath();
}


// 状態切り替え
void CTrashEnemy::ChangeState(EState state)
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
std::string CTrashEnemy::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return "待機";
	case EState::ePatrol:			return "巡回";
	case EState::eChase:			return "追跡";
	case EState::eLost:				return "見失う";
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

// 状態の色を取得
CColor CTrashEnemy::GetStateColor(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return CColor::white;
	case EState::ePatrol:			return CColor::green;
	case EState::eChase:			return CColor::red;
	case EState::eLost:				return CColor::yellow;
	case EState::eDamageStart:		return CColor::blue;
	case EState::eDamage:			return CColor::blue;
	case EState::eDamageEnd:		return CColor::blue;
	case EState::eJumpStart:		return CColor::gray;
	case EState::eJump:				return CColor::gray;
	case EState::eJumpEnd:			return CColor::gray;
	case EState::eAttackStart:		return CColor::magenta;
	case EState::eAttack:			return CColor::magenta;
	case EState::eAttackEnd:		return CColor::magenta;
	case EState::eCriticalStart:	return CColor::black;
	case EState::eCritical:			return CColor::black;
	case EState::eCriticalEnd:		return CColor::black;
	case EState::eOpenClose:		return CColor::white;
	}
	return CColor::white;
}

#endif