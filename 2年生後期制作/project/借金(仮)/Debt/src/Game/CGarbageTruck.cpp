#include "CGarbageTruck.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CModel.h"
#include "CNavNode.h"
#include "CTrashPlayer.h"
#include "CTrashEnemy.h"
#include "Primitive.h"
#include "CVehicleManager.h"
#include "CCollector.h"
#include "CGaugeUI3D.h"
#include "CFlamethrower.h"
#include "CTrashBag.h"
#include "Maths.h"
#include "CSound.h"


#define TRUCK_HEIGHT	13.0f	// トラックの高さ
#define TRUCK_WIDTH		30.0f	// トラックの幅
#define TRUCK_RADIUS	15.0f	// トラックの半径
// 本体コライダのオフセット座標
#define BODY_COL_OFFSET_POS CVector(0.0f,0.0f,-2.0f)

// 探知範囲
#define SEARCH_RADIUS	50.0f

// 回収員生成のオフセット座標
#define COLLECTOR_OFFSET_POS VectorX() * 10.0f

// 車両同士の距離の最低値
// 前方コライダ―に他の車両がいるときにこれより近くなれば停止する
#define VEHICLE_DIST 40.0f

// ノードの座標
#define NODE_POS0	CVector( 20.0f,0.0f, 35.0f)
#define NODE_POS1	CVector(-20.0f,0.0f, 35.0f)
#define NODE_POS2	CVector(-20.0f,0.0f,-35.0f)
#define NODE_POS3	CVector( 20.0f,0.0f,-35.0f)

#define ROTATE_SPEED 6.0f	// 回転速度

// 回収を開始できるZの座標の範囲
#define COLLECT_MAX_POSZ  200.0f
#define COLLECT_MIN_POSZ -200.0f

// Hpゲージのオフセット座標
#define GAUGE_OFFSET_POS CVector(0.0f,30.0f,0.0f)
// 通常のHpゲージの画像のパス
#define HP_GAUGE_PATH "UI\\garbageTruck_hp_gauge.png"

// ゴミ袋を落とすオフセット座標
#define TRASH_BAG_OFFSET_POS CVector(20.0f,5.0f,-30.0f)
// ゴミ袋を落とすX座標のランダム範囲
#define TRASH_BAG_RAND_POSX 20.0f
// ゴミ袋を落とす間隔
#define TRASH_BAG_DROP_TIME 0.5f

// 効果音の音量
#define SE_VOLUME 0.5f


// コンストラクタ
CGarbageTruck::CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation,
	ERoadType road, std::vector<CNavNode*> patrolPoints, bool punisher)
	: CVehicleBase(model, pos, rotation, road, patrolPoints)
	, CGarbageTruckStatus(punisher)
	, mState(EState::eMove)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsReturn(false)
	, mpSearchCol(nullptr)
{
	// 通常の場合
	if(!punisher)
	{
		// 通常のHpゲージを設定
		mpHpGauge = new CGaugeUI3D(this, HP_GAUGE_PATH);
		mpHpGauge->SetMaxPoint(GetMaxHp());
		mpHpGauge->SetCurrPoint(GetHp());
		// 最初は無効
		mpHpGauge->SetEnable(false);
		mpHpGauge->SetShow(false);

		// プレイヤー探知用コライダ―作成
		mpSearchCol = new CColliderSphere
		(
			this, ELayer::eCharaSearch,
			SEARCH_RADIUS,
			true
		);
		// プレイヤーと衝突判定
		mpSearchCol->SetCollisionTags({ ETag::ePlayer });
		mpSearchCol->SetCollisionLayers({ ELayer::ePlayer });

		int num = GetCollectorsNum();
		// 回収員を全て生成し、無効にしておく
		for (int i = 0; i < num; i++)
		{
			mpCollectors.push_back(new CCollector(punisher, this,
				{ mpNode0,mpNode1,mpNode2,mpNode3 }));
			// 無効にする
			mpCollectors[i]->SetOnOff(false);
		}
	}

	// 本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eVehicle,
		CVector(0.0f,TRUCK_HEIGHT, TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f,TRUCK_HEIGHT,-TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS
	);
	// 座標を設定
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// プレイヤー、敵、回収員、攻撃、車両、ゴミ収集車、車両探知用、地形、ゴミ袋
	// と衝突判定する
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,
		ETag::eVehicle,ETag::eField,ETag::eTrashBag });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,ELayer::eCollector,ELayer::eAttackCol,
		ELayer::eSpawnZone,ELayer::eVehicle,ELayer::eVehicleSearch,
		ELayer::eGround,ELayer::eWall,ELayer::eObject,ELayer::eTrashBag });

	// 前方向コライダ―
	mpFrontCol = new CColliderCapsule
	(
		this, ELayer::eVehicleSearch,
		CVector(0.0f, TRUCK_HEIGHT,  TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS, true
	);
	// 座標を設定
	mpFrontCol->Position(FRONT_COL_OFFSET_POS);
	// 車両と衝突判定する
	mpFrontCol->SetCollisionTags({ ETag::eVehicle });
	mpFrontCol->SetCollisionLayers({ ELayer::eVehicle });

	// 横方向コライダ―
	// 他より長く設定しておく
	mpSideCol = new CColliderCapsule
	(
		this, ELayer::eVehicleSearch,
		CVector(0.0f, TRUCK_HEIGHT,  TRUCK_WIDTH - TRUCK_RADIUS * 5.0f),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH + TRUCK_RADIUS * 5.0f),
		TRUCK_RADIUS, true
	);
	// もう一つの車道が左にある車道
	if (mRoadType == ERoadType::eLeft2 ||
		mRoadType == ERoadType::eRight1)
	{
		// 座標を設定
		mpSideCol->Position(LEFT_COL_OFFSET_POS);
	}
	// もう一つの車道が右にある車道
	else
	{
		// 座標を設定
		mpSideCol->Position(RIGHT_COL_OFFSET_POS);
	}
	// 車両と衝突判定する
	mpSideCol->SetCollisionTags({ ETag::eVehicle });
	mpSideCol->SetCollisionLayers({ ELayer::eVehicle });

	// 経路探索用のコライダ―作成
	mpNavCol = new CColliderCapsule
	(
		this, ELayer::eNone,
		CVector(0.0f, TRUCK_HEIGHT, TRUCK_WIDTH * 1.2f - TRUCK_RADIUS),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH * 1.2f + TRUCK_RADIUS),
		TRUCK_RADIUS, true
	);

	// ゴミ袋の数の初期値を設定
	SetTrashBag(GetDefaultBagNum());
	SetGoldTrashBag(GetDefaultGoldBagNum());
}

// デストラクタ
CGarbageTruck::~CGarbageTruck()
{
	// コライダーの削除
	SAFE_DELETE(mpSearchCol);

	// リストのクリア
	mpCollectors.clear();

	// HPゲージが存在したら、一緒に削除する
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
	}
}

// ダメージを受ける
void CGarbageTruck::TakeDamage(int damage, CObjectBase* causer)
{
	// ダメージを受ける
	CCharaStatusBase::TakeDamage(damage, causer);
}

// クリティカルダメージを受ける
void CGarbageTruck::TakeCritical(int damage, CObjectBase* causer)
{
	// 攻撃力の2倍のダメージ
	int criticalDamage = damage * 2;
	// ダメージを受ける
	CCharaStatusBase::TakeDamage(criticalDamage, causer);
}

// オブジェクト削除処理
void CGarbageTruck::DeleteObject(CObjectBase* obj)
{
	// 削除されたオブジェクトがHpゲージであれば
	// ポインタを空にする
	if (obj == mpHpGauge)
	{
		mpHpGauge = nullptr;
	}
}

// 更新
void CGarbageTruck::Update()
{
	switch (mState)
	{
	// 移動
	case EState::eMove:			UpdateMove();		break;
	// 停止
	case EState::eStop:			UpdateStop();		break;
	// 壊れた
	case EState::eBroken:		UpdateBroken();		break;
	// 車線変更
	case EState::eChangeRoad:	UpdateChangeRoad();	break;
	// 回収
	case EState::eCollect:		UpdateCollect();	break;
	}

	CVehicleBase::Update();

	// HPゲージを更新
	mpHpGauge->Position(Position() + GAUGE_OFFSET_POS);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());

	// 移動しているもしくは、自身が有効でない場合
	if (mIsMove || !IsEnable())
	{
		// ノードを無効にする
		mpNode0->SetEnable(false);
		mpNode1->SetEnable(false);
		mpNode2->SetEnable(false);
		mpNode3->SetEnable(false);
	}
	// 自身が有効で移動していない場合
	else
	{
		// ノードを有効にする
		mpNode0->SetEnable(true);
		mpNode1->SetEnable(true);
		mpNode2->SetEnable(true);
		mpNode3->SetEnable(true);
		// ノードを車両が止まっている座標の周りに設定する
		mpNode0->SetPos(Position() + NODE_POS0);
		mpNode1->SetPos(Position() + NODE_POS1);
		mpNode2->SetPos(Position() + NODE_POS2);
		mpNode3->SetPos(Position() + NODE_POS3);
	}

#if _DEBUG

	std::string roadType;
	if (mRoadType == ERoadType::eLeft1)
		roadType = "L1";
	else if (mRoadType == ERoadType::eLeft2)
		roadType = "L2";
	else if (mRoadType == ERoadType::eRight1)
		roadType = "R1";
	else
		roadType = "R2";
	CDebugPrint::Print("TruckState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("TruckRoadType:%s\n" ,roadType.c_str());
#endif
}

// 衝突処理
void CGarbageTruck::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CVehicleBase::Collision(self, other, hit);

	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// 衝突した相手がプレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// 自分が移動していたら
			if (IsMove())
			{
				// プレイヤークラスを取得
				CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(other->Owner());

				if (player != nullptr)
				{
					// 攻撃を受けているなら処理しない
					if (player->IsDamaging()) return;
					// 自分から相手の方向
					CVector direction = player->Position() - Position();
					direction = direction.Normalized();
					direction.Y(0.0f);
					// 相手が受けるノックバック速度に、
					// 自分が与えるノックバック速度を自分から相手の方向に設定
					player->SetKnockbackReceived(direction * GetKnockbackDealt());
					// クリティカルダメージを与える
					player->TakeCritical(GetAttackPower(), this, GetPower());
					// クリティカル音を再生
					mpCriticalSE->Play(SE_VOLUME, true);
				}
			}
		}
		// 衝突した相手が敵の場合
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 自分が移動していたら
			if (IsMove())
			{
				// 敵クラスを取得
				CTrashEnemy* enemy = dynamic_cast<CTrashEnemy*>(other->Owner());

				if (enemy != nullptr)
				{
					// 攻撃を受けているなら処理しない
					if (enemy->IsDamaging()) return;
					// 自分から相手の方向
					CVector direction = enemy->Position() - Position();
					direction = direction.Normalized();
					direction.Y(0.0f);
					// 相手が受けるノックバック速度に、
					// 自分が与えるノックバック速度を自分から相手の方向に設定
					enemy->SetKnockbackReceived(direction * enemy->GetKnockbackDealt());
					// クリティカルダメージを与える
					enemy->TakeCritical(GetAttackPower(), this, GetPower());
					// クリティカル音を再生
					mpCriticalSE->Play(SE_VOLUME, true);
				}
			}
		}
		// 衝突した相手が回収員の場合
		else if (other->Layer() == ELayer::eCollector)
		{
			// 自分が移動していたら
			if (IsMove())
			{
				// 回収員クラスを取得
				CCollector* collector = dynamic_cast<CCollector*>(other->Owner());

				if (collector != nullptr)
				{
					// 攻撃力分のダメージを与える
					collector->TakeDamage(GetAttackPower(), this);
				}
			}
		}
		// 衝突した相手が車両の場合
		else if (other->Layer() == ELayer::eVehicle)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
		}
	}
	// 前方コライダ―
	else if (self == mpFrontCol)
	{
		// 相手が車両の場合
		if (other->Layer() == ELayer::eVehicle)
		{
			// 車両クラスを取得
			CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());

			// 相手と自分の座標の距離を求める
			float dist = CVector::Distance(vehicle->Position(), Position());

			// 相手が動いていないかつ壊れていない場合
			if (!vehicle->IsMove()&&
				!vehicle->IsBroken())
			{
				mIsFrontVehicle = true;
				// 横方向に車両がいないかつ
				// 前方の車のさらに前に停止している車両がない場合
				if (!mIsSideVehicle&&
					!vehicle->GetIsFrontVehicle())
				{
					// 車線変更の移動先を設定
					SetChangeRoadPoint(vehicle);
					// 車線変更状態へ移行
					ChangeState(EState::eChangeRoad);
				}
				// 横方向に車両がいるなら
				else
				{
					// 停止状態へ移行
					ChangeState(EState::eStop);
				}
			}
			// 相手が動いていないかつ壊れている場合
			else if (!vehicle->IsMove() &&
				vehicle->IsBroken())
			{
				mIsFrontVehicle = true;
				// いなくなれば進めるのでそれまで停止
				ChangeState(EState::eStop);
			}
			// 相手が動いていても近い場合は停止する
			else if (vehicle->IsMove() &&
				dist < VEHICLE_DIST)
			{
				mIsFrontVehicle = true;
				// 停止状態へ移行
				ChangeState(EState::eStop);
			}
		}
	}
	// 横方向コライダ―
	else if (self == mpSideCol)
	{
		// 相手が車両の場合
		if (other->Layer() == ELayer::eVehicle)
		{
			mIsSideVehicle = true;
		}
	}
	// 探知用コライダ―
	else if (self == mpSearchCol)
	{
		// 相手がプレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// 撤退中でなければ
			if (!mIsReturn)
			{
				CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
				// 回収を開始できる場所にいるかつ
				// 壊れた状態でないかつ
				// プレイヤーがエリア内なら
				if (CanCollectPosZ() &&
					mState != EState::eBroken&&
					!player->AreaOutX())
				{
					// 回収状態へ
					ChangeState(EState::eCollect);
				}
			}
		}
	}
}

// 描画
void CGarbageTruck::Render()
{
	CVehicleBase::Render();
#if _DEBUG
	// 移動状態であれば
	if (mState == EState::eMove)
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
		// 巡回ポイントまで移動のステップなら
		if (mStateStep == 1)
		{
			CVector offsetPos = CVector(0.0f, TRUCK_HEIGHT, 0.0f);
			CVector selfPos = Position() + offsetPos;
			CVector targetPos = mpMoveRoute[mNextMoveIndex]->GetPos() + offsetPos;
			// 次巡回するポイントまで緑線を描画
			Primitive::DrawLine
			(
				selfPos, targetPos,
				CColor::green,
				2.0f
			);
		}
	}
	// 車線変更状態の場合
	else if (mState == EState::eChangeRoad)
	{
		// 車線変更で移動するポイントを描画
		Primitive::DrawWireBox
		(
			mpChangeRoadPoint->GetPos() + CVector(0.0f, 1.0f, 0.0f),
			CVector::one,
			CColor::gray
		);
		CVector offsetPos = CVector(0.0f, TRUCK_HEIGHT, 0.0f);
		CVector selfPos = Position() + offsetPos;
		CVector targetPos = mpChangeRoadPoint->GetPos() + offsetPos;
		// 車線変更の移動先ポイントまで青線を描画
		Primitive::DrawLine
		(
			selfPos, targetPos,
			CColor::blue,
			2.0f
		);
	}
#endif
}

// 車両の有効無効を切り替える
void CGarbageTruck::SetOnOff(bool setOnOff)
{
	CVehicleBase::SetOnOff(setOnOff);
	// 有効にする場合
	if (setOnOff)
	{
		// HPゲージを更新
		mpHpGauge->Position(Position() + GAUGE_OFFSET_POS);
		mpHpGauge->SetMaxPoint(GetMaxHp());
		mpHpGauge->SetCurrPoint(GetHp());
	}

	mpHpGauge->SetEnable(setOnOff);
	mpHpGauge->SetShow(setOnOff);
	mpFlamethrower->Stop();
}

// 変数をリセット
void CGarbageTruck::Reset()
{
	CVehicleBase::Reset();

	mStateStep = 0;
	mElapsedTime = 0.0f;
	mIsReturn = false;

	mState = EState::eMove;
}

// 回収をできるZの範囲内にいるかどうか
bool CGarbageTruck::CanCollectPosZ()
{
	// 回収を開始できるZの範囲内ならtrue
	if (Position().Z() <= COLLECT_MAX_POSZ &&
		Position().Z() >= COLLECT_MIN_POSZ)
	{
		return true;
	}
	// 範囲外ならfalse
	return false;
}

// 移動処理
void CGarbageTruck::UpdateMove()
{
	// 死んでいる場合
	if (IsDeath())
	{
		mElapsedTime -= Times::DeltaTime();
		// ゴミ袋を落とす間隔の時間が経過したら
		if (mElapsedTime <= TRASH_BAG_DROP_TIME)
		{
			// ゴミ袋を落とす
			DropTrashBag(1, Position(), VectorZ(), VectorX(), TRASH_BAG_OFFSET_POS);
			mElapsedTime = TRASH_BAG_DROP_TIME;
		}
	}
	// 動いている
	mIsMove = true;
	// 全ての巡回ポイントへの移動が終わっていないなら
	if (!mIsMoveEnd)
	{
		// ステップごとに処理を切り替える
		switch (mStateStep)
		{
			// ステップ0：移動開始時の巡回ポイントを求める
		case 0:
			// 巡回ポイントを求める
			ChangePatrolPoint();
			mStateStep++;
			break;

			// ステップ1：巡回ポイントまで移動
		case 1:
		{
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
		// 次の巡回ポイントを求める
		case 2:
			ChangePatrolPoint();
			mStateStep = 1;
			break;
		}
	}
}

// 停止処理
void CGarbageTruck::UpdateStop()
{
	switch (mStateStep)
	{
		// 設定を変更する
	case 0:
		// 動いていない
		mIsMove = false;
		// 移動速度をゼロにする
		mMoveSpeed = CVector::zero;
		mStateStep++;
		break;

		// 前に車両がいなくなったら移動状態へ
	case 1:
		// 前に車両がいないなら
		if (!mIsFrontVehicle)
		{
			// 移動状態へ
			ChangeState(EState::eMove);
		}
		break;
	}
}

// 壊れた処理
void CGarbageTruck::UpdateBroken()
{
	switch (mStateStep)
	{
		// ステップ0：設定を変更する
	case 0:
		// 動いていない
		mIsMove = false;
		// 移動速度をゼロにする
		mMoveSpeed = CVector::zero;
		// 炎のエフェクトを発生させる
		mpFlamethrower->Start();
		mStateStep++;
		break;

		// ステップ1：全ての回収員を撤退
	case 1:
	{
		int size = mpCollectors.size();
		// 全ての回収員の状態を撤退状態へ
		for (int i = 0; i < size; i++)
		{
			// 無効ならスルー
			if (!mpCollectors[i]->IsEnable()) continue;
			// 撤収状態に変更
			mpCollectors[i]->ChangeStateReturn();
		}
		// 次のステップへ
		mStateStep++;

		break;
	}

		// ステップ2：回収員がいなくなるまで待機
	case 2:
		// 回収員の数がゼロなら
		if (GetCollectorsNum() <= 0)
		{
			// 自分の撤退を開始
			// 撤退の移動である
			mIsReturn = true;
			// 移動状態へ
			ChangeState(EState::eMove);
		}
		break;
	}
}

// 車線変更処理
void CGarbageTruck::UpdateChangeRoad()
{
	switch (mStateStep)
	{
	// 移動していることを設定
	case 0:
		mIsMove = true;
		mStateStep++;
		break;

	// 車線変更の移動先のノードまで移動
	case 1:
		// 移動が終わったら
		if (MoveTo(mpChangeRoadPoint->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{
			CVehicleManager* vehicleMgr = CVehicleManager::Instance();
			if (vehicleMgr == nullptr) return;

			/*
			どの道にいるかの状態の変更
			*/
			// 左から1番目の道の場合
			if (mRoadType == ERoadType::eLeft1)
			{
				// 左から2番目の道に移動したので変更
				ChangeRoadType(ERoadType::eLeft2);
			}
			// 左から2番目の道の場合
			else if (mRoadType == ERoadType::eLeft2)
			{
				// 左から1番目の道に移動したので変更
				ChangeRoadType(ERoadType::eLeft1);
			}
			// 右から1番目の道の場合
			else if (mRoadType == ERoadType::eRight1)
			{
				// 左から2番目の道に移動したので変更
				ChangeRoadType(ERoadType::eRight2);
			}
			// 右から2番目の道の場合
			else
			{
				// 左から2番目の道に移動したので変更
				ChangeRoadType(ERoadType::eRight1);
			}

			// 巡回ポイントのリストを変更された道のものに変更する
			mpPatrolPoints = vehicleMgr->GetPatrolPoints(mRoadType);
			mStateStep++;
		}
		break;

	// 車線変更が終了
	case 2:
		// 終わったので移動状態に移行
		ChangeState(EState::eMove);
		break;
	}
}


// 回収処理
void CGarbageTruck::UpdateCollect()
{
	switch (mStateStep)
	{
		// ステップ0：設定を変更する
	case 0:
	{
		// 動いていない
		mIsMove = false;
		// 移動速度をゼロにする
		mMoveSpeed = CVector::zero;
		// 撤退までの時間を初期値に戻す
		SetReturnTime();
		// 回収員の人数を初期値に戻す
		SetCollectorsNum();

		int size = mpCollectors.size();
		// 回収員を全て有効にする
		for (int i = 0; i < size; i++)
		{
			// 有効にする
			mpCollectors[i]->SetOnOff(true);
			// 自分の座標＋オフセット座標を設定
			mpCollectors[i]->Position(Position() + COLLECTOR_OFFSET_POS * (i + 1));
		}

		mStateStep++;
		break;
	}

	// ステップ1：時間が経過するまでカウントダウン
	case 1:
		// 撤退までの時間をカウントダウンする
		CountReturnTime();

		// 撤退までの時間が経過したら次のステップへ
		if (IsElapsedReturnTime())
		{
			int size = mpCollectors.size();
			// 全ての回収員の状態を撤退状態へ
			for (int i = 0; i < size; i++)
			{
				// 無効ならスルー
				if (!mpCollectors[i]->IsEnable()) continue;
				// 撤収状態に変更
				mpCollectors[i]->ChangeStateReturn();
			}

			mStateStep++;
		}
		// もしくは、回収員が全滅したら次のステップへ
		else if (GetCollectorsNum() <= 0)
		{
			mStateStep++;
		}
		break;

		// ステップ2：回収員がいなくなるまで待機
	case 2:

		// 回収員の数がゼロなら
		if (GetCollectorsNum() <= 0)
		{
			// 自分の撤退を開始
			// 撤退の移動である
			mIsReturn = true;
			// 移動状態へ
			ChangeState(EState::eMove);
		}
		break;
	}
}

// 死亡(壊れた)
void CGarbageTruck::Death()
{
	// 壊れた状態へ
	ChangeState(EState::eBroken);
}

// 状態切り替え
void CGarbageTruck::ChangeState(EState state)
{
	// 同じなら処理しない
	if (state == mState) return;
	// 壊れた状態から車線変更、止まるには変更しない
	if ((state == EState::eChangeRoad || state == EState::eStop) &&
		mState == EState::eBroken)
	{
		return;
	}

	// 移動から他の状態に変更する時
	// 移動の中断中
	if (mState == EState::eMove)
	{
		mIsMovePause = true;
	}

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

#if _DEBUG
// 状態の文字列を取得
std::string CGarbageTruck::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eMove:			return "移動中";
	case EState::eStop:			return "停止中";
	case EState::eBroken:		return "壊れている";
	case EState::eChangeRoad:	return "車線変更";
	case EState::eCollect:		return "回収中";
	default:					return "";
	}
}
#endif

