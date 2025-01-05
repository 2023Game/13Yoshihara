#include "CTrashEnemy.h"
#include "CDebugFieldOfView.h"
#include "CPlayerBase.h"
#include "CFieldBase.h"
#include "Primitive.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "CVehicleManager.h"

// TODO：後で消すテスト用
#include "CInput.h"

// 衝突相手の車両基底クラスを取得するための
// 車両の基底クラスのインクルード
#include "CVehicleBase.h"

// コライダのインクルード
#include "CColliderCapsule.h"

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

#define FOV_ANGLE 45.0f		// 視野範囲の角度
#define FOV_LENGTH 100.0f	// 視野範囲の距離
#define EYE_HEIGHT 5.0f		// 視点の高さ

#define ROTATE_SPEED 6.0f	// 回転速度
#define ATTACK_RANGE 18.0f	// 攻撃範囲

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


// コンストラクタ
CTrashEnemy::CTrashEnemy()
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
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsOpen(false)
	, mIsJump(false)
{
	// アニメーションとモデルの初期化
	InitAnimationModel("TrashEnemy", &ANIM_DATA);

	// 地形、プレイヤー、敵、攻撃、車両
	// と衝突判定をする本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(BODY_WIDTH - BODY_RADIUS * 10, BODY_HEIGHT, 0.0f),
		CVector(-BODY_WIDTH + BODY_RADIUS * 10, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy, ETag::eVehicle });
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::eObject,
		ELayer::ePlayer, ELayer::eEnemy, ELayer::eAttackCol, ELayer::eVehicle});

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle_Close);
}

// デストラクタ
CTrashEnemy::~CTrashEnemy()
{
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


	// 地面に接しているならジャンプしていない
	if (mIsGrounded)
		mIsJump = false;

	// キャラクターの更新
	CEnemyBase::Update();

#if _DEBUG
	// 現在の状態に合わせて視野範囲の色を変更
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("EnemyIsOpen:%s\n", mIsOpen ? "true" : "false");
	CDebugPrint::Print("EnemyIsJump:%s\n", mIsJump ? "true" : "false");
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
				direction = direction.Normalized();
				direction.Y(0.0f);
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
		// 衝突した相手がプレイヤーなら
		else if (other->Layer() == ELayer::ePlayer)
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
void CTrashEnemy::Render()
{
	CEnemyBase::Render();

	// 巡回状態であれば、
	if (mState == EState::ePatrol)
	{
		float rad = 1.0f;
		// 巡回ポイントを全て描画
		int size = mPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CMatrix m;
			m.Translate(mPatrolPoints[i]->GetPos() + CVector(0.0f, rad * 2.0f, 0.0f));
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawWireSphere(m, rad, c);
		}
	}
	// 見失った状態であれば、
	else if (mState == EState::eLost)
	{
		// プレイヤーを見失った位置にデバッグ表示
		float rad = 2.0f;
		CMatrix m;
		m.Translate(mpLostPlayerNode->GetPos() + CVector(0.0f, rad, 0.0f));
		Primitive::DrawWireSphere(m, rad, CColor::blue);
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
	// プレイヤーが視野範囲内に入ったら、追跡状態へ移行
	if (IsFoundPlayer())
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
	if (IsFoundPlayer())
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

	// プレイヤーが見えなくなったら、見失った状態へ移行
	if (!IsLookPlayer())
	{
		// 見失った位置にノードを配置
		mpLostPlayerNode->SetPos(targetPos);
		ChangeState(EState::eLost);
		mStateStep = 0;
		return;
	}
	// プレイヤーに攻撃できるならば、攻撃状態へ移行
	if (CanAttackPlayer(ATTACK_RANGE))
	{
		ChangeState(EState::eAttackStart);

		// 閉じていたら開くアニメーションを再生
		if (!mIsOpen)
		{
			ChangeAnimation((int)EAnimType::eOpen);
		}
		// 開いていたらすぐ攻撃アニメーションを再生
		else
		{
			ChangeAnimation((int)EAnimType::eAttack_Start);
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
			}
		}
		break;
	}
}

// 被弾開始
void CTrashEnemy::UpdateDamageStart()
{
	// アニメーションが終了したら被弾ノックバックへ
	if (IsAnimationFinished())
	{
		// ダメージを1受ける
		TakeDamage(1, nullptr);
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
void CTrashEnemy::UpdateDamage()
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
void CTrashEnemy::UpdateDamageEnd()
{
	// 被弾終了時は移動をゼロ
	mMoveSpeed = CVector::zero;

	// アニメーションが60%進行したら
	if (GetAnimationFrameRatio() >= 0.60f)
	{
		// 移動可能

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
void CTrashEnemy::UpdateJumpStart()
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

void CTrashEnemy::UpdateJump()
{

}

void CTrashEnemy::UpdateJumpEnd()
{

}

// 攻撃開始
void CTrashEnemy::UpdateAttackStart()
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
void CTrashEnemy::UpdateAttack()
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
void CTrashEnemy::UpdateAttackEnd()
{
	// アニメーションが終了したら待機へ
	if (IsAnimationFinished())
	{
		// 最後は蓋が開いた状態
		ChangeState(EState::eIdle);
		mIsOpen = true;
	}
}

void CTrashEnemy::UpdateCriticalStart()
{
}

void CTrashEnemy::UpdateCritical()
{
}

void CTrashEnemy::UpdateCriticalEnd()
{
}

void CTrashEnemy::UpdateOpenClose()
{
}

// 状態切り替え
void CTrashEnemy::ChangeState(EState state)
{
	// 既に同じ状態であれば、処理しない
	if (state == mState) return;

	mState = state;
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