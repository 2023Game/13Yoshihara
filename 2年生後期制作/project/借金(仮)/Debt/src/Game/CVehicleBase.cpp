#include "CVehicleBase.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CVehicleManager.h"
 
#define FRONT_HEIGHT	13.0f	// 前方判定の高さ
#define FRONT_WIDTH		30.0f	// 前方判定の幅
#define FRONT_RADIUS	12.0f	// 前方判定の半径

// コンストラクタ
CVehicleBase::CVehicleBase(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road)
	: CCharaBase(ETag::eVehicle, ETaskPriority::eVehicle)
	, mpModel(model)
	, mpBodyCol(nullptr)
	, mpFrontCol(nullptr)
	, mpSideCol(nullptr)
	, mRoadType(road)
{
	// 車両と衝突判定する前方判定コライダ―
	mpFrontCol = new CColliderCapsule
	(
		this, ELayer::eVehicle,
		CVector(0.0f, FRONT_HEIGHT, FRONT_WIDTH),
		CVector(0.0f, FRONT_HEIGHT, -FRONT_WIDTH),
		FRONT_RADIUS
	);
	mpFrontCol->Position(0.0f, 0.0f, FRONT_WIDTH * 2);
	mpFrontCol->SetCollisionTags({ETag::eVehicle });
	mpFrontCol->SetCollisionLayers({ELayer::eVehicle,});

	Position(pos);
	Rotation(rotation);
}

// デストラクタ
CVehicleBase::~CVehicleBase()
{
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpFrontCol);
	SAFE_DELETE(mpSideCol);
}

// 更新
void CVehicleBase::Update()
{
	CVector moveSpeed = mMoveSpeed;

	Position(Position() + moveSpeed);
}

// 衝突処理
void CVehicleBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// 衝突した相手がプレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// 横方向だけに押し戻す
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
		}
	}
}

// 描画
void CVehicleBase::Render()
{
	mpModel->Render(Matrix());
}

// 移動中かどうか
bool CVehicleBase::IsMove() const
{
	// 各軸への移動速度のいずれかが0より大きいなら移動中
	if (mMoveSpeed.X() > 0.0f) return true;
	if (mMoveSpeed.Y() > 0.0f) return true;
	if (mMoveSpeed.Z() > 0.0f) return true;
	// 全て0以下なら移動していない
	return false;
}

// 車線を変更する
void CVehicleBase::ChangeRoad(float moveSpeed, bool& isEnd)
{
	isEnd = false;
	// 今いる道によって処理
	switch (GetRoadType())
	{
	// 左から一番目の道
	case ERoadType::eLeft1:
		// 自分から見て右へ移動
		mMoveSpeed = VectorX() * moveSpeed;
		// X座標が左から二番目の道の座標以上になれば
		if (Position().X() >= CAR_LEFT_POS2.X())
		{
			// 今いる道の状態を左から二番目の道に変更
			ChangeRoadType(ERoadType::eLeft2);
			isEnd = true;
		}
		break;
	// 左から二番目の道
	case ERoadType::eLeft2:
		// 自分から見て左へ移動
		mMoveSpeed = -VectorX() * moveSpeed;
		// X座標が左から一番目の道の座標以下になれば
		if (Position().X() <= CAR_LEFT_POS1.X())
		{
			// 今いる道の状態を左から一番目の道に変更
			ChangeRoadType(ERoadType::eLeft1);
			isEnd = true;
		}
		break;
	// 右から一番目の道
	case ERoadType::eRight1:
		// 自分から見て左へ移動
		mMoveSpeed = VectorX() * moveSpeed;
		// X座標が右から二番目の道の座標以下になれば
		if (Position().X() <= CAR_RIGHT_POS2.X())
		{
			// 今いる道の状態を右から二番目の道に変更
			ChangeRoadType(ERoadType::eRight2);
			isEnd = true;
		}
		break;
	// 右から二番目の道
	case ERoadType::eRight2:
		// 自分から見て右へ移動
		mMoveSpeed = -VectorX() * moveSpeed;
		// X座標が右から一番目の道の座標以上になれば
		if (Position().X() >= CAR_RIGHT_POS1.X())
		{
			// 今いる道の状態を右から一番目の道に変更
			ChangeRoadType(ERoadType::eRight1);
			isEnd = true;
		}
		break;
	}
}

// どの道にいる状態かを変更する
void CVehicleBase::ChangeRoadType(ERoadType roadType)
{
	// 同じなら処理しない
	if (roadType == mRoadType) return;

	mRoadType = roadType;
}

// 今どの道にいるか取得する
CVehicleBase::ERoadType CVehicleBase::GetRoadType() const
{
	return mRoadType;
}
