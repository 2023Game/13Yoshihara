#include "CDeliveryPlayer.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CInput.h"
#include "CDeliveryItem.h"
#include "CDeliveryField.h"
#include "CDeliveryHpUI2D.h"
#include "CDeliveryEnemy.h"

#define TRUCK_HEIGHT	13.0f	// トラックの高さ
#define TRUCK_WIDTH		32.5f	// トラックの幅
#define TRUCK_RADIUS	12.5f	// トラックの半径
// 本体コライダ―のオフセット座標
#define BODY_COL_OFFSET_POS CVector(0.0f,0.0f,-3.0f)

#define EYE_HEIGHT 5.0f		// 視点の高さ

#define ROTATE_SPEED 3.0f	// 回転速度

// 初期の方向
#define ROTATION CVector(0.0f,180.0f,0.0f)

// 車線変更時の車線のオフセット座標
#define CHANGE_ROAD_OFFSET_POS_L CVector(-60.0f,0.0f,0.0f)
#define CHANGE_ROAD_OFFSET_POS_R CVector( 60.0f,0.0f,0.0f)
// 閾値
#define CHANGE_ROAD_THRESHOLD 0.1f

// 弾丸のオフセット座標
#define BULLET_OFFSET_POS_L  CVector(-20.0f,14.75f,10.0f)	// 左
#define BULLET_OFFSET_POS_R  CVector( 19.0f,14.75f,10.0f)	// 右
#define BULLET_OFFSET_POS_B  CVector( 0.0f, 14.75f,41.0f)	// 後ろ
// 弾丸の方向
#define BULLET_ROT_LR	CVector(0.0f,90.0f,0.0f) // 左右

// ダメージ後の無敵時間
#define INVINCIBLE_TIME 1.0f
// 点滅間隔
#define HIT_FLASH_INTERVAL 0.25f

// コンストラクタ
CDeliveryPlayer::CDeliveryPlayer()
	: CPlayerBase()
	, CDeliveryPlayerStatus()
	, mInvincibleTime(0.0f)
	, mHitFlashTime(0.0f)
	, mTargetPos(CVector::zero)
	, mDeliveryNum(0)
	, mDestroyEnemyNum(0)
	, mShotNum(0)
	, mHitNum(0)
	, mIsLeftMove(false)
{
	// 移動状態
	ChangeState(EState::eMove);
	// 方向を設定
	Rotation(ROTATION);
	// 移動方向を向かない
	mIsMoveDir = false;
	// 重力無効
	mIsGravity = false;
	mpModel = CResourceManager::Get<CModel>("DeliveryPlayer");

	// HpUIを生成
	mpHpUI = new CDeliveryHpUI2D(this);
	// 現在のHPを設定
	mpHpUI->SetCurrPoint(GetHp());

	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CDeliveryPlayer::~CDeliveryPlayer()
{
	// HpUIが存在したら、一緒に削除する
	if (mpHpUI != nullptr)
	{
		mpHpUI->SetOwner(nullptr);
		mpHpUI->Kill();
	}
}

// オブジェクト削除処理
void CDeliveryPlayer::DeleteObject(CObjectBase* obj)
{
	CPlayerBase::DeleteObject(obj);
	if (obj == mpHpUI)
	{
		mpHpUI = nullptr;
	}
}

// 更新
void CDeliveryPlayer::Update()
{
	switch (mState)
	{
	case EState::eMove:			UpdateMove();			break;
	case EState::eChangeRoad:	UpdateChangeRoad();		break;
	case EState::eDeath:		UpdateDeath();			break;
	}

	// 移動か車線変更状態なら
	if (mState == EState::eMove ||
		mState == EState::eChangeRoad)
	{
		// キー入力可能
		ActionInput();
	}

	// 被弾の点滅
	HitFlash();
	
	// 基底クラスの更新
	CPlayerBase::Update();

	// 現在のHPを設定
	mpHpUI->SetCurrPoint(GetHp());

#if _DEBUG
	CDebugPrint::Print("PlayerState：%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("PlayerHP：%d\n", GetHp());
	CDebugPrint::Print("PlayerTargetPos：%f,%f,%f\n", mTargetPos.X(), mTargetPos.Y(), mTargetPos.Z());
#endif
}

// 衝突処理
void CDeliveryPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 基底クラスの衝突処理
	CPlayerBase::Collision(self, other, hit);
	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// 敵の場合
		if (other->Layer() == ELayer::eEnemy)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);

			// 敵にダメージを与える
			CDeliveryEnemy* enemy = dynamic_cast<CDeliveryEnemy*>(other->Owner());
			enemy->TakeDamage(GetAttackPower(), this);

			// 車線変更状態の場合
			if (mState == EState::eChangeRoad)
			{
				// プレイヤーのX座標
				float playerPosX = Position().X();
				// 敵のX座標
				float enemyPosX = enemy->Position().X();
				// 左移動でプレイヤーの方が右にいるなら
				if (mIsLeftMove &&
					playerPosX > enemyPosX)
				{
					// 自分のいた道へ戻る
					mTargetRoadType = mRoadType;
				}
				// 右移動でプレイヤーの方が左にいるなら
				else if (!mIsLeftMove &&
					playerPosX < enemyPosX)
				{
					// 自分のいた道へ戻る
					mTargetRoadType = mRoadType;
				}
			}
		}
	}
}

// 描画
void CDeliveryPlayer::Render()
{
	mpModel->Render(Matrix());
}

// 状態切り替え
void CDeliveryPlayer::ChangeState(EState state)
{
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// コライダ―を生成
void CDeliveryPlayer::CreateCol()
{
	// 本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, TRUCK_HEIGHT, TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS
	);
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// 地形、敵、障害物、発射物の攻撃判定、アイテム
	// と衝突判定をする
	mpBodyCol->SetCollisionTags({ ETag::eField,ETag::eEnemy,
		ETag::eObstruction,ETag::eBullet,ETag::eItem });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,ELayer::eEnemy,
		ELayer::eObstruction,ELayer::eAttackCol,ELayer::eItem });
}

#if _DEBUG
// 状態の文字列を取得
std::string CDeliveryPlayer::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eMove:			return "移動";
	case EState::eChangeRoad:	return "車線変更";
	case EState::eDeath:		return "死亡";
	}
	return "";
}
#endif

// 移動の更新処理
void CDeliveryPlayer::UpdateMove()
{
	// 今いる道のX座標
	float targetPosX = 0.0f;
	switch (mRoadType)
	{
	case ERoadType::eLeft1:
		targetPosX = ROAD_LEFT1_POSX;
		break;
	case ERoadType::eLeft2:
		targetPosX = ROAD_LEFT2_POSX;
		break;
	case ERoadType::eRight1:
		targetPosX = ROAD_RIGHT1_POSX;
		break;
	case ERoadType::eRight2:
		targetPosX = ROAD_RIGHT2_POSX;
		break;
	}
	// 使用しないので
	// 目的地を自分に設定しておく
	mTargetPos = CVector(targetPosX, Position().Y(), Position().Z());
	Position(mTargetPos);

	mMoveSpeed = CVector::zero;
	CVector forward = CVector::Slerp(VectorZ(), CVector::back, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 横移動は0にする
	move.X(0.0f);
	// 移動がプラス方向なら
	if (move.Z() > 0.0f)
	{
		// プラス方向の範囲外なら
		if (Position().Z() >= ROAD_Z_AREA_P)
		{
			// Z座標を範囲に設定して
			Position(Position().X(), Position().Y(), ROAD_Z_AREA_P);
			// Z移動を0
			move.Z(0.0f);
		}
	}
	// 移動がマイナス方向なら
	else if (move.Z() < 0.0f)
	{
		// マイナス方向の範囲外なら
		if (Position().Z() <= ROAD_Z_AREA_M)
		{
			// Z座標を範囲に設定して
			Position(Position().X(), Position().Y(), ROAD_Z_AREA_M);
			// Z移動を0
			move.Z(0.0f);
		}
	}
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		// 基礎移動速度で移動
		mMoveSpeed += move * GetBaseMoveSpeed() * Times::DeltaTime();
	}
}

// 車線変更の更新処理
void CDeliveryPlayer::UpdateChangeRoad()
{
	mMoveSpeed = CVector::zero;
	// 目的地のX座標
	float targetPosX = 0.0f;
	switch (mTargetRoadType)
	{
	case ERoadType::eLeft1:
		targetPosX = ROAD_LEFT1_POSX;
		break;
	case ERoadType::eLeft2:
		targetPosX = ROAD_LEFT2_POSX;
		break;
	case ERoadType::eRight1:
		targetPosX = ROAD_RIGHT1_POSX;
		break;
	case ERoadType::eRight2:
		targetPosX = ROAD_RIGHT2_POSX;
		break;
	}
	// 目的地を設定
	mTargetPos = CVector(targetPosX, Position().Y(), Position().Z());

	// 目的地へ移動
	MoveTo(mTargetPos, GetBaseMoveSpeed(), ROTATE_SPEED);

	// 自分のX座標が閾値内に入ったら
	if (Position().X() <= mTargetPos.X() + CHANGE_ROAD_THRESHOLD &&
		Position().X() >= mTargetPos.X() - CHANGE_ROAD_THRESHOLD)
	{
		// 目的地の車道のタイプに変更
		mRoadType = mTargetRoadType;
		// 移動状態へ
		ChangeState(EState::eMove);
		return;
	}
}

// 死亡の更新処理
void CDeliveryPlayer::UpdateDeath()
{
	// 移動をゼロ
	mMoveSpeedY = 0.0f;
	mMoveSpeed = CVector::zero;
	// 衝突無効
	SetEnableCol(false);
	// ゲームシーンでプレイヤーが死亡したことによる
	// ゲーム終了を取得するための変数設定
	SetGameEnd(true);
}

// 死亡
void CDeliveryPlayer::Death()
{
	// 死亡状態へ
	ChangeState(EState::eDeath);
}

// 指定した位置まで移動する
bool CDeliveryPlayer::MoveTo(const CVector& targetPos, float speed, float rotateSpeed)
{
	// 目的地までのベクトルを求める
	CVector pos = Position();
	CVector vec = targetPos - pos;
	vec.Y(0.0f);
	// 移動方向ベクトルを求める
	CVector moveDir = vec.Normalized();

	// 徐々に移動方向へ向ける
	CVector forward = CVector::Slerp
	(
		VectorZ(),	// 現在の正面方向
		moveDir,	// 移動方向
		rotateSpeed * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

	// 今回の移動距離を求める
	float moveDist = speed * Times::DeltaTime();
	// 目的地までの残りの距離を求める
	float remainDist = vec.Length();
	// 残りの距離が移動距離より短い
	if (remainDist <= moveDist)
	{
		pos = CVector(targetPos.X(), pos.Y(), targetPos.Z());
		Position(pos);
		return true;	// 目的地に到着したので、trueを返す
	}

	// 残りの距離が移動距離より長い場合は
	// 移動距離分目的地へ移動
	pos += moveDir * moveDist;
	Position(pos);

	// 目的地には到着しなかった
	return false;
}

// アクションのキー入力
void CDeliveryPlayer::ActionInput()
{
	// Aキーで、左へ移動
	if (CInput::PushKey('A'))
	{
		// 一番左の車道なら処理しない
		if (mTargetPos.X() <= ROAD_LEFT1_POSX + 30.0f) return;
		// 目的地を設定
		mTargetPos = GetTargetPos(true);
		// 車線変更状態へ
		ChangeState(EState::eChangeRoad);
		return;
	}
	// Dキーで、右へ移動
	else if (CInput::PushKey('D'))
	{
		// 一番右の車道なら処理しない
		if (mTargetPos.X() >= ROAD_RIGHT1_POSX - 30.0f) return;
		// 目的地を設定
		mTargetPos = GetTargetPos(false);
		// 車線変更状態へ
		ChangeState(EState::eChangeRoad);
		return;
	}

	// 左クリックで、左方向へ射撃
	if (CInput::PushKey(VK_LBUTTON))
	{
		// Hpが1以上ある場合
		if (GetHp() >= 1)
		{
			// 配達物を生成
			CDeliveryItem* item = new CDeliveryItem(this);
			// 座標を設定
			item->Position(Position() + BULLET_OFFSET_POS_L);
			// 回転を設定
			item->Rotation(BULLET_ROT_LR);
			// 移動速度
			float moveSpeedX = GetThrowSpeed() * Times::DeltaTime();
			// 移動を設定
			item->SetMoveSpeed(-VectorX() * moveSpeedX);

			// Hpを減らす
			TakeDamage(1, nullptr, true);
		}
	}
	// 右クリックで、右方向へ射撃
	if (CInput::PushKey(VK_RBUTTON))
	{
		// Hpが1以上ある場合
		if (GetHp() >= 1)
		{
			// 配達物を生成
			CDeliveryItem* item = new CDeliveryItem(this);
			// 座標を設定
			item->Position(Position() + BULLET_OFFSET_POS_R);
			// 回転を設定
			item->Rotation(BULLET_ROT_LR);
			// 移動速度
			float moveSpeedX = GetThrowSpeed() * Times::DeltaTime();
			// 移動を設定
			item->SetMoveSpeed(VectorX() * moveSpeedX);

			// Hpを減らす
			TakeDamage(1, nullptr, true);
		}
	}
	// スペースキーで、後方へ射撃
	if (CInput::PushKey(VK_SPACE))
	{
		// Hpが1以上ある場合
		if (GetHp() >= 1)
		{
			// 配達物1を生成
			CDeliveryItem* item1 = new CDeliveryItem(this);
			// 座標を設定
			item1->Position(Position() + BULLET_OFFSET_POS_B);
			// 移動速度
			float moveSpeedZ = GetThrowSpeed() * Times::DeltaTime();
			// 移動を設定
			item1->SetMoveSpeed(-VectorZ() * moveSpeedZ);

			// Hpを減らす
			TakeDamage(1, nullptr, true);
		}
	}
}

// ダメージの点滅と無敵時間の処理
void CDeliveryPlayer::HitFlash()
{
	if (IsDamaging())
	{
		// 点滅間隔が経過したら
		if (mHitFlashTime > HIT_FLASH_INTERVAL)
		{
			mHitFlashTime -= HIT_FLASH_INTERVAL;
			// 描画するかを反転
			SetShow(!IsShow());
		}
		// 無敵時間が経過したら
		if (mInvincibleTime > INVINCIBLE_TIME)
		{
			// 描画する
			SetShow(true);
			// 衝突判定有効
			SetEnableCol(true);
			// ダメージを受けていない
			mIsDamage = false;
			// 計測用の変数をリセット
			mHitFlashTime = 0.0f;
			mInvincibleTime = 0.0f;
		}
		mHitFlashTime += Times::DeltaTime();
		mInvincibleTime += Times::DeltaTime();
	}
}

// 車線変更先の座標を求める
CVector CDeliveryPlayer::GetTargetPos(bool isLeft)
{
	// 左右どちらへ移動するかを設定
	mIsLeftMove = isLeft;
	// 自分の座標を取得
	CVector targetPos = Position();
	// 自分が今いる道と左右どちらかの移動なのか
	// から目的地を決定
	switch (mRoadType)
	{
	case ERoadType::eLeft1:
		// 左移動
		if (isLeft)
		{
			targetPos.X(ROAD_LEFT1_POSX);
			mTargetRoadType = ERoadType::eLeft1;
		}
		// 右移動
		else
		{
			targetPos.X(ROAD_LEFT2_POSX);
			mTargetRoadType = ERoadType::eLeft2;
		}
		break;
	case ERoadType::eLeft2:
		// 左移動
		if (isLeft)
		{
			targetPos.X(ROAD_LEFT1_POSX);
			mTargetRoadType = ERoadType::eLeft1;
		}
		// 右移動
		else
		{
			targetPos.X(ROAD_RIGHT2_POSX);
			mTargetRoadType = ERoadType::eRight2;
		}
		break;
	case ERoadType::eRight1:
		// 左移動
		if (isLeft)
		{
			targetPos.X(ROAD_RIGHT2_POSX);
			mTargetRoadType = ERoadType::eRight2;
		}
		// 右移動
		else
		{
			targetPos.X(ROAD_RIGHT1_POSX);
			mTargetRoadType = ERoadType::eRight1;
		}
		break;
	case ERoadType::eRight2:
		// 左移動
		if (isLeft)
		{
			targetPos.X(ROAD_LEFT2_POSX);
			mTargetRoadType = ERoadType::eLeft2;
		}
		// 右移動
		else
		{
			targetPos.X(ROAD_RIGHT1_POSX);
			mTargetRoadType = ERoadType::eRight1;
		}
		break;
	}
	return targetPos;
}

// 配達した数を1増やす
void CDeliveryPlayer::IncreaseDeliveryNum()
{
	mDeliveryNum++;
}

// 配達した数を取得する
int CDeliveryPlayer::GetDeliveryNum() const
{
	return mDeliveryNum;
}

// 壊したトラックの数を1増やす
void CDeliveryPlayer::IncreaseDestroyEnemyNum()
{
	mDestroyEnemyNum++;
}

// 壊したトラックの数を取得する
int CDeliveryPlayer::GetDestroyEnemyNum() const
{
	return mDestroyEnemyNum;
}

// 発射した数を1増やす
void CDeliveryPlayer::IncreaseShotNum()
{
	mShotNum++;
}

// 発射した数を取得する
int CDeliveryPlayer::GetShotNum() const
{
	return mShotNum;
}

// 当たった数を1増やす
void CDeliveryPlayer::IncreaseHitNum()
{
	mHitNum++;
}

// 当たった数を取得する
int CDeliveryPlayer::GetHitNum() const
{
	return mHitNum;
}

// 現在の車道を設定
void CDeliveryPlayer::SetRoadType(ERoadType roadType)
{
	mRoadType = roadType;
}

// 現在の車道を取得
ERoadType CDeliveryPlayer::GetRoadType() const
{
	return mRoadType;
}

// ダメージを受ける
void CDeliveryPlayer::TakeDamage(int damage, CObjectBase* causer, bool isShot)
{
	// ダメージを受けている時はダメージを受けない
	if (IsDamaging()) return;

	// 死亡状態なら、ダメージを受けない
	if (mState == EState::eDeath) return;

	// 自分の射撃による減少ではない場合
	if (!isShot)
	{
		// ダメージを受けている
		mIsDamage = true;
	}

	// Hpが0の時攻撃を受けたら死亡
	if (GetHp() == 0)
	{
		Death();
		return;
	}

	// Hpをダメージ分減らす
	SetHp(-damage);
}