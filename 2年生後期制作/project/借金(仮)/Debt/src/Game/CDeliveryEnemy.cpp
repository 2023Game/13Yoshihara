#include "CDeliveryEnemy.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryField.h"
#include "CDeliveryObstruction.h"
#include "CDeliveryItem.h"
#include "CDeliveryFieldItem.h"
#include "CDeliveryHpUI3D.h"

#define TRUCK_HEIGHT	13.0f	// トラックの高さ
#define TRUCK_WIDTH		32.5f	// トラックの幅
#define TRUCK_RADIUS	12.5f	// トラックの半径
// 本体コライダ―のオフセット座標
#define BODY_COL_OFFSET_POS CVector(0.0f,0.0f,-3.0f)

// 探知コライダー
#define SEARCH_HEIGHT	13.0f	// 高さ
#define SEARCH_WIDTH	32.5f*5.0f	// 幅
#define SEARCH_RADIUS	12.5f*2.0f	// 半径
// 探知コライダーのオフセット座標
#define SEARCH_OFFSET_POS CVector(0.0f,0.0f,50.0f)

#define EYE_HEIGHT 5.0f		// 視点の高さ

#define ROTATE_SPEED 3.0f	// 回転速度

// 初期の方向
#define ROTATION CVector(0.0f,180.0f,0.0f)

// 車線変更時の車線のオフセット座標
#define CHANGE_ROAD_OFFSET_POS_L CVector(-40.0f,0.0f,-10.0f)
#define CHANGE_ROAD_OFFSET_POS_R CVector( 40.0f,0.0f,-10.0f)
// 車線変更時の閾値
#define CHANGE_ROAD_THRESHOLD 0.1f

// 弾丸のオフセット座標
#define BULLET_OFFSET_POS_L  CVector(-20.0f,14.75f,10.0f)	// 左
#define BULLET_OFFSET_POS_R  CVector( 19.0f,14.75f,10.0f)	// 右
#define BULLET_OFFSET_POS_B  CVector( 0.0f, 14.75f,41.0f)	// 後ろ
// 弾丸の方向
#define BULLET_ROT_LR	CVector(0.0f,90.0f,0.0f) // 左右

// ダメージ後の無敵時間
#define INVINCIBLE_TIME 0.5f
// 点滅間隔
#define HIT_BLINK_INTERVAL 0.1f
// 撃てる間隔
#define SHOOT_INTERVAL 0.5f

// 消滅するZ座標
#define DELETE_POSZ 300.0f

// 移動の間隔の時間
#define MOVE_INTERVAL_TIME 0.4f
// プレイヤーが横にいるかの閾値
#define SIDE_PLAYER_THRESHOLD 50.0f

// 敵の定位置のZ座標
#define ENEMY_POSZ -50.0f

// Hpゲージのオフセット座標
#define HP_UI_OFFSET_POS CVector(0.0f,30.0f*2.0f,0.0f)

// コンストラクタ
CDeliveryEnemy::CDeliveryEnemy()
	: CEnemyBase
	(
		0.0f,0.0f,
		{},
		EYE_HEIGHT
	)
	, CDeliveryEnemyStatus()
	, mInvincibleTime(0.0f)
	, mHitBlinkTime(0.0f)
	, mLeftShootTime(0.0f)
	, mRightShootTime(0.0f)
	, mBackShootTime(0.0f)
	, mIsLeft1Danger(false)
	, mIsLeft2Danger(false)
	, mIsRight1Danger(false)
	, mIsRight2Danger(false)
	, mIsLeft1Item(false)
	, mIsLeft2Item(false)
	, mIsRight1Item(false)
	, mIsRight2Item(false)

{
	// 移動状態
	ChangeState(EState::eMove);
	// 方向を設定
	Rotation(ROTATION);
	// 移動方向を向かない
	mIsMoveDir = false;
	// 重力無効
	mIsGravity = false;
	mpModel = CResourceManager::Get<CModel>("DeliveryEnemy");

	// HpUIを生成
	mpHpUI = new CDeliveryHpUI3D(this);
	// 現在のHPを設定
	mpHpUI->SetCurrPoint(GetHp());
	// 無効
	mpHpUI->SetEnable(false);
	mpHpUI->SetShow(false);

	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CDeliveryEnemy::~CDeliveryEnemy()
{
	SAFE_DELETE(mpSearchCol);

	// HpUIが存在したら、一緒に削除する
	if (mpHpUI != nullptr)
	{
		mpHpUI->SetOwner(nullptr);
		mpHpUI->Kill();
	}
}

// オブジェクト削除処理
void CDeliveryEnemy::DeleteObject(CObjectBase* obj)
{
	CEnemyBase::DeleteObject(obj);
	if (obj == mpHpUI)
	{
		mpHpUI = nullptr;
	}
}

// ダメージを受ける
void CDeliveryEnemy::TakeDamage(int damage, CObjectBase* causer, bool isShot)
{
	// ダメージを受けている時は処理しない
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

// 更新
void CDeliveryEnemy::Update()
{
	switch (mState)
	{
	case EState::eMove:			UpdateMove();			break;
	case EState::eChangeRoad:	UpdateChangeRoad();		break;
	case EState::eDeath:		UpdateDeath();			break;
	}

	// ダメージを受けていたら点滅する
	HitBlink();

	// 移動か車線変更状態なら
	if (mState == EState::eMove ||
		mState == EState::eChangeRoad)
	{
		// 条件を満たしたとき射撃
		Shoot();
	}

	// 道が危険かをリセット
	SetRoadDanger(ERoadType::eLeft1, false);
	SetRoadDanger(ERoadType::eLeft2, false);
	SetRoadDanger(ERoadType::eRight1, false);
	SetRoadDanger(ERoadType::eRight2, false);
	// 道にアイテムがあるかをリセット
	SetRoadItem(ERoadType::eLeft1, false);
	SetRoadItem(ERoadType::eLeft2, false);
	SetRoadItem(ERoadType::eRight1, false);
	SetRoadItem(ERoadType::eRight2, false);

	// 基底クラスの更新
	CEnemyBase::Update();

	// HPUIを更新
	mpHpUI->Position(Position() + HP_UI_OFFSET_POS);
	mpHpUI->SetCurrPoint(GetHp());

#if _DEBUG
	CDebugPrint::Print("EnemyState：%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("EnemyHp：%d\n", GetHp());
	CDebugPrint::Print("EnemyTargetPos：%f,%f,%f\n", mTargetPos.X(), mTargetPos.Y(), mTargetPos.Z());
#endif
}

// 衝突処理
void CDeliveryEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 基底クラスの衝突処理
	CEnemyBase::Collision(self, other, hit);
	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// プレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(other->Owner());
			player->TakeDamage(GetAttackPower(), this);
		}
	}
	// 探知コライダ―
	else if (self == mpSearchCol)
	{
		// 障害物の場合
		if (other->Layer() == ELayer::eObstruction)
		{
			// 障害物の取得
			CDeliveryObstruction* obstruction = dynamic_cast<CDeliveryObstruction*>(other->Owner());
			// 存在する道
			ERoadType roadType = obstruction->GetRoadType();
			// 存在する道が危険
			SetRoadDanger(roadType, true);
		}
		// アイテムの場合
		else if (other->Layer() == ELayer::eItem)
		{
			// フィールドアイテムクラスの取得
			CDeliveryFieldItem* fieldItem = dynamic_cast<CDeliveryFieldItem*>(other->Owner());
			// 存在する道
			ERoadType roadType = fieldItem->GetRoadType();
			// 道にアイテムがあることを設定
			SetRoadItem(roadType, true);
		}
	}
}

// 描画
void CDeliveryEnemy::Render()
{
	mpModel->Render(Matrix());
}

// 有効無効を切り替える
void CDeliveryEnemy::SetOnOff(bool setOnOff)
{
	// 有効無効を設定
	SetEnable(setOnOff);
	SetShow(setOnOff);
	// 移動状態に戻す
	ChangeState(EState::eMove);
	// Hpをリセット
	ResetHp();
	// HpUIの位置を設定
	mpHpUI->Position(Position() + HP_UI_OFFSET_POS);
	// HpUIの有効無効を設定
	mpHpUI->SetEnable(setOnOff);
	mpHpUI->SetShow(setOnOff);
}

// 現在の車道を設定
void CDeliveryEnemy::SetRoadType(ERoadType roadType)
{
	mRoadType = roadType;
}

// 現在の車道を取得
ERoadType CDeliveryEnemy::GetRoadType() const
{
	return ERoadType();
}

// 状態切り替え
void CDeliveryEnemy::ChangeState(EState state)
{
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

#if _DEBUG
// 状態の文字列を取得
std::string CDeliveryEnemy::GetStateStr(EState state) const
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
void CDeliveryEnemy::UpdateMove()
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

	// プレイヤー取得
	CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(CDeliveryPlayer::Instance());
	// プレイヤーの座標
	CVector playerPos = player->Position();
	// プレイヤーのいる道
	ERoadType playerRoadType = player->GetRoadType();
	// 同じ道かつ、プレイヤーの方が奥にいるかつ、
	// エリア内かつ、ダメージを受けていない場合
	if (mRoadType == playerRoadType &&
		Position().Z() > playerPos.Z() &&
		playerPos.Z() >= ROAD_Z_AREA_M &&
		!player->IsDamaging())
	{
		// プレイヤーを目的地にする
		mTargetPos = CVector(targetPosX, Position().Y(), playerPos.Z());
	}
	// それ以外は
	else
	{
		// 目的地は定位置
		mTargetPos = CVector(targetPosX, Position().Y(), ENEMY_POSZ);
	}
	// 目的地へ移動
	// 移動方向は向かない
	if (MoveTo(mTargetPos, GetBaseMoveSpeed(), 0.0f))
	{

	}

	CVector forward = CVector::Slerp(VectorZ(), CVector::back, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// 経過時間を計算
	mElapsedTime += Times::DeltaTime();

	// 今いる道が危険なら車線変更する
	if (GetNowRoadDanger())
	{
		switch (mRoadType)
		{
		case ERoadType::eLeft1:
			// 右へ移動
			GetTargetPos(false);
			break;
		case ERoadType::eLeft2:
			// 左の道が危険でない場合
			if (!GetRoadDanger(ERoadType::eLeft1))
			{
				// 左へ移動
				GetTargetPos(true);
			}
			// それ以外右へ移動
			else
			{
				// 右へ移動
				GetTargetPos(false);
			}
			break;
		case ERoadType::eRight1:
			// 左へ移動
			GetTargetPos(true);
			break;
		case ERoadType::eRight2:
			// 右の道が危険でない場合
			if (!GetRoadDanger(ERoadType::eRight1))
			{
				// 右へ移動
				GetTargetPos(false);
			}
			// それ以外右へ移動
			else
			{
				// 左へ移動
				GetTargetPos(true);
			}
			break;
		}
		// 車線変更状態へ
		ChangeState(EState::eChangeRoad);
		return;
	}

	// 左右にアイテムがあり、安全の場合は車線を変更する
	ChangeRoadToItem();
}

// 車線変更の更新処理
void CDeliveryEnemy::UpdateChangeRoad()
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
void CDeliveryEnemy::UpdateDeath()
{
	// 奥側を向かせる
	CVector forward = CVector::Slerp(VectorZ(), CVector::back, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));
	// 手前へ移動
	float moveSpeed = GetBaseMoveSpeed() * 0.2f * Times::DeltaTime();
	mMoveSpeed = CVector::forward * moveSpeed;
	// 削除座標を超えたら
	if (Position().Z() > DELETE_POSZ)
	{
		// 無効
		SetOnOff(false);
	}
}

// 死亡
void CDeliveryEnemy::Death()
{
	// 壊した敵の数を増やす
	CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(CDeliveryPlayer::Instance());
	player->IncreaseDestroyEnemyNum();
	// 攻撃を受けていない
	mIsDamage = false;
	// 死亡状態へ
	ChangeState(EState::eDeath);
}

// 死んでいるかどうか
bool CDeliveryEnemy::IsDeath() const
{
	// 死亡状態の場合
	if (mState == EState::eDeath)
	{
		return true;
	}
	return false;
}

// 道が危険かを設定
void CDeliveryEnemy::SetRoadDanger(ERoadType roadType, bool danger)
{
	switch (roadType)
	{
	case ERoadType::eLeft1:
		mIsLeft1Danger = danger;
		break;
	case ERoadType::eLeft2:
		mIsLeft2Danger = danger;
		break;
	case ERoadType::eRight1:
		mIsRight1Danger = danger;
		break;
	case ERoadType::eRight2:
		mIsRight2Danger = danger;
		break;
	}
}

// コライダ―を生成
void CDeliveryEnemy::CreateCol()
{
	// 本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eEnemy,
		CVector(0.0f, TRUCK_HEIGHT, TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS
	);
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// 地形、プレイヤー、障害物、発射物の攻撃判定、アイテム
	// と衝突判定をする
	mpBodyCol->SetCollisionTags({ ETag::eField,ETag::ePlayer,
		ETag::eObstruction,ETag::eBullet,ETag::eItem });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,ELayer::ePlayer,
		ELayer::eObstruction,ELayer::eAttackCol,ELayer::eItem });

	// 探知コライダー
	mpSearchCol = new CColliderCapsule
	(
		this, ELayer::eSearch,
		CVector(SEARCH_WIDTH - SEARCH_RADIUS, SEARCH_HEIGHT, 0.0f),
		CVector(-SEARCH_WIDTH + SEARCH_RADIUS, SEARCH_HEIGHT, 0.0f),
		SEARCH_RADIUS,
		true
	);
	mpSearchCol->Position(SEARCH_OFFSET_POS);
	// 障害物、アイテムと衝突判定
	mpSearchCol->SetCollisionTags({ ETag::eObstruction,ETag::eItem });
	mpSearchCol->SetCollisionLayers({ ELayer::eObstruction,ELayer::eItem });
}

// 指定した位置まで移動する
bool CDeliveryEnemy::MoveTo(const CVector& targetPos, float speed, float rotateSpeed)
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

// ダメージの点滅と無敵時間の処理
void CDeliveryEnemy::HitBlink()
{
	if (IsDamaging())
	{
		// 点滅間隔が経過したら
		if (mHitBlinkTime > HIT_BLINK_INTERVAL)
		{
			mHitBlinkTime -= HIT_BLINK_INTERVAL;
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
			mHitBlinkTime = 0.0f;
			mInvincibleTime = 0.0f;
		}
		mHitBlinkTime += Times::DeltaTime();
		mInvincibleTime += Times::DeltaTime();
	}
}

// 条件を満たしたときに射撃する
void CDeliveryEnemy::Shoot()
{
	// Hpが5以上ある場合しか撃たない
	if (GetHp() <= 5) return;

	// プレイヤー取得
	CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(CDeliveryPlayer::Instance());
	// プレイヤー座標
	CVector playerPos = player->Position();
	// プレイヤーのいる道
	ERoadType playerRoadType = player->GetRoadType();
	// インターバルを経過させる
	mLeftShootTime -= Times::DeltaTime();
	mRightShootTime -= Times::DeltaTime();
	mBackShootTime -= Times::DeltaTime();
	// 閾値内で横にいる
	if (Position().Z() + SIDE_PLAYER_THRESHOLD >= playerPos.Z()&&
		Position().Z() - SIDE_PLAYER_THRESHOLD <= playerPos.Z())
	{
		// 左右どちらに撃つか
		bool isLeftShoot = false;
		// 自分の道によって処理
		switch (mRoadType)
		{
		case ERoadType::eLeft1:
			// 右に撃つ
			isLeftShoot = false;
			break;
		case ERoadType::eLeft2:
			// プレイヤーが左1にいるなら
			if (playerRoadType == ERoadType::eLeft1)
			{
				// 左に撃つ
				isLeftShoot = true;
			}
			// それ以外
			else
			{
				// 右に撃つ
				isLeftShoot = false;
			}
			break;
		case ERoadType::eRight1:
			// 左に撃つ
			isLeftShoot = true;
			break;
		case ERoadType::eRight2:
			// プレイヤーが右1にいるなら
			if (playerRoadType == ERoadType::eRight1)
			{
				// 右に撃つ
				isLeftShoot = false;
			}
			// それ以外
			else
			{
				// 左に撃つ
				isLeftShoot = true;
			}
			break;
		}
		// 左に撃つ時インターバルが終わっていたら
		if (isLeftShoot && mLeftShootTime <= 0.0f)
		{
			// インターバルを設定
			mLeftShootTime = SHOOT_INTERVAL;
			// 配達物を生成
			CDeliveryItem* item = new CDeliveryItem(this);
			// 座標を設定
			item->Position(Position() + BULLET_OFFSET_POS_L);
			// 回転を設定
			item->Rotation(BULLET_ROT_LR);
			// 移動速度
			float moveSpeedZ = GetThrowSpeed() * Times::DeltaTime();
			// 移動を設定
			item->SetMoveSpeed(-VectorX() * moveSpeedZ);

			// Hpを減らす
			TakeDamage(1, nullptr, true);
		}
		// 右に撃つ時インターバルが終わっていたら
		else if (!isLeftShoot && mRightShootTime <= 0.0f)
		{
			// インターバルを設定
			mRightShootTime = SHOOT_INTERVAL;
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

	// 同じ道かつ、後ろにいるかつ、
	// 撃つインターバルが終わっている場合
	if (mRoadType == playerRoadType &&
		Position().Z() < playerPos.Z() &&
		mBackShootTime <= 0.0f)
	{
		// インターバルを設定
		mBackShootTime = SHOOT_INTERVAL;
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

// 車線変更先の座標を求める
CVector CDeliveryEnemy::GetTargetPos(bool isLeft)
{
	// 自分の座標を取得
	CVector targetPos = Position();
	// 車線変更中の場合
	if (mState == EState::eChangeRoad)
	{
		// 移動の方向が逆なら
		if (mIsLeftMove != isLeft)
		{
			// 自分の道に戻す
			mTargetRoadType = mRoadType;
			// 座標を設定
			switch (mTargetRoadType)
			{
			case ERoadType::eLeft1:
				targetPos.X(ROAD_LEFT1_POSX);
				break;
			case ERoadType::eLeft2:
				targetPos.X(ROAD_LEFT2_POSX);
				break;
			case ERoadType::eRight1:
				targetPos.X(ROAD_RIGHT1_POSX);
				break;
			case ERoadType::eRight2:
				targetPos.X(ROAD_RIGHT2_POSX);
				break;
			}
			// 移動方向を設定
			mIsLeftMove = isLeft;
			// 目標座標を返す
			return targetPos;
		}
	}
	// 左右どちらへ移動するかを設定
	mIsLeftMove = isLeft;
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

// 今いる道が危険か
bool CDeliveryEnemy::GetNowRoadDanger() const
{
	// 自分がいる道が危険か
	switch (mRoadType)
	{
	case ERoadType::eLeft1:
		return mIsLeft1Danger;
		break;
	case ERoadType::eLeft2:
		return mIsLeft2Danger;
		break;
	case ERoadType::eRight1:
		return mIsRight1Danger;
		break;
	case ERoadType::eRight2:
		return mIsRight2Danger;
		break;
	}
}

// 指定した道が危険か
bool CDeliveryEnemy::GetRoadDanger(ERoadType roadType) const
{
	switch (roadType)
	{
	case ERoadType::eLeft1:
		return mIsLeft1Danger;
		break;
	case ERoadType::eLeft2:
		return mIsLeft2Danger;
		break;
	case ERoadType::eRight1:
		return mIsRight1Danger;
		break;
	case ERoadType::eRight2:
		return mIsRight2Danger;
		break;
	}
}

// 指定した道にアイテムがあるかを設定する
void CDeliveryEnemy::SetRoadItem(ERoadType roadType, bool isItem)
{
	switch (roadType)
	{
	case ERoadType::eLeft1:
		mIsLeft1Item = isItem;
		break;
	case ERoadType::eLeft2:
		mIsLeft2Item = isItem;
		break;
	case ERoadType::eRight1:
		mIsRight1Item = isItem;
		break;
	case ERoadType::eRight2:
		mIsRight2Item = isItem;
		break;
	}
}

// 指定した道にアイテムがあるか
bool CDeliveryEnemy::GetRoadItem(ERoadType roadType) const
{
	switch (roadType)
	{
	case ERoadType::eLeft1:
		return mIsLeft1Item;
		break;
	case ERoadType::eLeft2:
		return mIsLeft2Item;
		break;
	case ERoadType::eRight1:
		return mIsRight1Item;
		break;
	case ERoadType::eRight2:
		return mIsRight2Item;
		break;
	}
}

// 左右にアイテムがある場合に車線を変更する
void CDeliveryEnemy::ChangeRoadToItem()
{
	// 車線変更をするか
	bool isChangeRoad = false;
	switch (mRoadType)
	{
	case ERoadType::eLeft1:
		// 右の道にアイテムがあり、安全な場合
		if (GetRoadItem(ERoadType::eLeft2) &&
			!GetRoadDanger(ERoadType::eLeft2))
		{
			// 右へ移動
			GetTargetPos(false);
			isChangeRoad = true;
		}
		break;
	case ERoadType::eLeft2:
		// 右の道にアイテムがあり、安全な場合
		if (GetRoadItem(ERoadType::eRight2) &&
			!GetRoadDanger(ERoadType::eRight2))
		{
			// 右へ移動
			GetTargetPos(false);
			isChangeRoad = true;
		}
		// 左の道にアイテムがあり、安全な場合
		else if (GetRoadItem(ERoadType::eLeft1) &&
			!GetRoadDanger(ERoadType::eLeft1))
		{
			// 左へ移動
			GetTargetPos(true);
			isChangeRoad = true;
		}
		break;
	case ERoadType::eRight1:
		// 左の道にアイテムがあり、安全な場合
		if (GetRoadItem(ERoadType::eRight2) &&
			!GetRoadDanger(ERoadType::eRight2))
		{
			// 左へ移動
			GetTargetPos(true);
			isChangeRoad = true;
		}
		break;
	case ERoadType::eRight2:
		// 右の道にアイテムがあり、安全な場合
		if (GetRoadItem(ERoadType::eRight1) &&
			!GetRoadDanger(ERoadType::eRight1))
		{
			// 右へ移動
			GetTargetPos(false);
			isChangeRoad = true;
		}
		// 左の道にアイテムがあり、安全な場合
		else if (GetRoadItem(ERoadType::eLeft2) &&
			!GetRoadDanger(ERoadType::eLeft2))
		{
			// 左へ移動
			GetTargetPos(true);
			isChangeRoad = true;
		}
		break;
	}

	// 車線変更するなら
	if (isChangeRoad)
	{
		// 車線変更状態へ
		ChangeState(EState::eChangeRoad);
	}
}
