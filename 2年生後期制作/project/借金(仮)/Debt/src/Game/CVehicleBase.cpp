#include "CVehicleBase.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CVehicleManager.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "Maths.h"
 
#define FRONT_HEIGHT	13.0f	// 前方判定の高さ
#define FRONT_WIDTH		40.0f	// 前方判定の幅
#define FRONT_RADIUS	12.0f	// 前方判定の半径
#define TURN_SPEED		CVector(0.0f,0.5f,0.0f)	// 車両の方向転換速度
#define TURN_MAX		CVector(0.0f,22.5f,0.0f)// 車両の方向転換の最大値

// コンストラクタ
CVehicleBase::CVehicleBase(CModel* model, const CVector& pos, const CVector& rotation,
	ERoadType road, std::vector<CNavNode*> patrolPoints)
	: CCharaBase(ETag::eVehicle, ETaskPriority::eVehicle)
	, mpModel(model)
	, mpBodyCol(nullptr)
	, mpFrontCol(nullptr)
	, mpSideCol(nullptr)
	, mIsMove(false)
	, mIsBroken(false)
	, mRoadType(road)
	, mCurrentRoadRotation(rotation)
	, mNextPatrolIndex(-1)
	, mNextMoveIndex(0)
	, mPatrolPoints(patrolPoints)
	, mIsMoveEnd(false)
	, mIsMovePause(false)
	, mIsFrontVehicle(false)
	, mIsSideVehicle(false)
{
	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);
	mpNavNode->SetEnable(false);

	// 車両の周り用のノードを生成
	mpNode0 = new CNavNode(Position());
	mpNode1 = new CNavNode(Position());
	mpNode2 = new CNavNode(Position());
	mpNode3 = new CNavNode(Position());
	// 最初はノードを無効
	mpNode0->SetEnable(false);
	mpNode1->SetEnable(false);
	mpNode2->SetEnable(false);
	mpNode3->SetEnable(false);

	// 車線変更用のノードを生成
	mpChangeRoadPoint = new CNavNode(Position(), true);
	// 最初はノードを無効
	mpChangeRoadPoint->SetEnable(false);

	// 最初は描画、更新しない
	SetEnable(false);
	SetShow(false);

	Position(pos);
	Rotation(rotation);
}

// デストラクタ
CVehicleBase::~CVehicleBase()
{
	// コライダ―の削除
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpFrontCol);
	SAFE_DELETE(mpSideCol);
	SAFE_DELETE(mpNavCol);
}

// 更新
void CVehicleBase::Update()
{
	CVector moveSpeed = mMoveSpeed;

	Position(Position() + moveSpeed);

	mIsFrontVehicle = false;
	mIsSideVehicle = false;
}

// 衝突処理
void CVehicleBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// 描画
void CVehicleBase::Render()
{
	mpModel->Render(Matrix());
}

// 移動中かどうか
bool CVehicleBase::IsMove() const
{
	return mIsMove;
}

// 最後の巡回ポイントまでの移動が終了したかどうか
bool CVehicleBase::GetMoveEnd() const
{
	return mIsMoveEnd;
}

// 壊れているか
bool CVehicleBase::IsBroken() const
{
	return mIsBroken;
}

// 前方に停止している車両がいるかどうか
bool CVehicleBase::GetIsFrontVehicle() const
{
	return mIsFrontVehicle;
}

// 車線を変更する
void CVehicleBase::ChangeRoad(bool& isEnd)
{

}

// どの道にいる状態かを変更する
void CVehicleBase::ChangeRoadType(ERoadType roadType)
{
	// 同じなら処理しない
	if (roadType == mRoadType) return;

	mRoadType = roadType;
	// 横方向コライダ―の位置も設定
	// もう一つの車道が左にある車道
	if (mRoadType == ERoadType::eLeft2 ||
		mRoadType == ERoadType::eRight1)
	{
		mpSideCol->Position(LEFT_COL_OFFSET_POS);
	}
	// もう一つの車道が右にある車道
	else
	{
		mpSideCol->Position(RIGHT_COL_OFFSET_POS);
	}
}

// 今どの道にいるか取得する
CVehicleBase::ERoadType CVehicleBase::GetRoadType() const
{
	return mRoadType;
}

// 本体コライダ―を取得する
CCollider* CVehicleBase::GetBodyCol() const
{
	return mpBodyCol;
}

// 経路探索用コライダ―を取得する
CCollider* CVehicleBase::GetNavCol() const
{
	return mpNavCol;
}

// 巡回ポイントのリストを設定する
void CVehicleBase::SetPatrolPoints(std::vector<CNavNode*> patrolPoints)
{
	mPatrolPoints = patrolPoints;
}

// 車両の有効無効を切り替える
void CVehicleBase::SetOnOff(bool setOnOff)
{
	SetEnable(setOnOff);
	SetShow(setOnOff);
	mpNavNode->SetEnable(setOnOff);
}

// 変数をリセット
void CVehicleBase::Reset()
{
	mNextPatrolIndex = -1;
	mIsMoveEnd = false;

	// 経路探索用のノードがあれば、座標を更新
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}
}

// 指定した位置まで移動する
bool CVehicleBase::MoveTo(const CVector& targetPos, float speed, float rotateSpeed)
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

// 次に巡回するポイントを変更
void CVehicleBase::ChangePatrolPoint()
{
	// 巡回ポイントが設定されていない場合は、処理しない
	int size = mPatrolPoints.size();
	if (size == 0) return;

	// 巡回開始時であれば、一番最初の巡回ポイントを選択
	if (mNextPatrolIndex == -1)
	{
		// 一番最初の巡回ポイントに設定する
		mNextPatrolIndex = 0;
	}
	// 移動が中断されていたら、
	// 巡回ポイントの番号を変更しない
	else if (mIsMovePause)
	{
		mIsMovePause = false;
	}
	// 巡回中だった場合、次の巡回ポイントを指定
	// 全ての巡回ポイントを通った場合移動が終了
	else
	{
		mNextPatrolIndex++;
		if (mNextPatrolIndex >= size)
		{
			mIsMoveEnd = true;
			return;
		}
	}

	// 次に巡回するポイントが決まった場合
	if (mNextPatrolIndex >= 0)
	{
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			// 経路探索用のノードの座標を更新
			mpNavNode->SetPos(Position());

			// 次に巡回するポイントの経路を設定
			mMoveRoute.clear();
			mMoveRoute.push_back(mpNavNode);
			mMoveRoute.push_back(mPatrolPoints[mNextPatrolIndex]);
			// 次の目的地インデックスを設定
			mNextMoveIndex = 1;
		}
	}
}

// 車線変更で移動するノードの座標を設定する
void CVehicleBase::SetChangeRoadPoint(CVehicleBase* frontVehicle)
{
	// 前の車の座標
	CVehicleManager* vehicleMgr = CVehicleManager::Instance();
	if (vehicleMgr == nullptr) return;
	CVector frontVehiclePos = frontVehicle->Position();
	CVector pos;
	float roadPosX;

	switch (mRoadType)
	{
	// 左から1番目の道
	case ERoadType::eLeft1:
		// 自分と前の車の中間地点を計算
		pos = CVector::Lerp(Position(), frontVehiclePos, 0.5f);

		// 変更先の道のX座標
		roadPosX = vehicleMgr->GetRoadPosX(ERoadType::eLeft2);
		pos = CVector(roadPosX, 0.0f, pos.Z());
		// 車線変更用のノードの座標を設定
		mpChangeRoadPoint->SetPos(pos);
		break;

	// 左から2番目の道
	case ERoadType::eLeft2:
		// 自分と前の車の中間地点を計算
		pos = CVector::Lerp(Position(), frontVehiclePos, 0.5f);

		// 変更先の道のX座標
		roadPosX = vehicleMgr->GetRoadPosX(ERoadType::eLeft1);
		pos = CVector(roadPosX, 0.0f, pos.Z());
		// 車線変更用のノードの座標を設定
		mpChangeRoadPoint->SetPos(pos);
		break;

	// 右から1番目の道
	case ERoadType::eRight1:
		// 自分と前の車の中間地点を計算
		pos = CVector::Lerp(Position(), frontVehiclePos, 0.5f);

		// 変更先の道のX座標
		roadPosX = vehicleMgr->GetRoadPosX(ERoadType::eRight2);
		pos = CVector(roadPosX, 0.0f, pos.Z());
		// 車線変更用のノードの座標を設定
		mpChangeRoadPoint->SetPos(pos);
		break;

	// 右から2番目の道
	case ERoadType::eRight2:
		// 自分と前の車の中間地点を計算
		pos = CVector::Lerp(Position(), frontVehiclePos, 0.5f);

		// 変更先の道のX座標
		roadPosX = vehicleMgr->GetRoadPosX(ERoadType::eRight1);
		pos = CVector(roadPosX, 0.0f, pos.Z());
		// 車線変更用のノードの座標を設定
		mpChangeRoadPoint->SetPos(pos);
		break;
	}
}
