#include "CVehicleBase.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CVehicleManager.h"
 
#define FRONT_HEIGHT	13.0f	// 前方判定の高さ
#define FRONT_WIDTH		30.0f	// 前方判定の幅
#define FRONT_RADIUS	12.0f	// 前方判定の半径
#define TURN_SPEED		CVector(0.0f,0.5f,0.0f)	// 車両の方向転換速度
#define TURN_MAX		CVector(0.0f,22.5f,0.0f)// 車両の方向転換の最大値

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
}

// 描画
void CVehicleBase::Render()
{
	mpModel->Render(Matrix());
}

// 移動中かどうか
bool CVehicleBase::IsMove() const
{
	// 移動速度の2乗の長さが0より大きいなら移動中
	if (mMoveSpeed.LengthSqr() > 0.0f) return true;
	// 全て0以下なら移動していない
	return false;
}

// 車線を変更する
void CVehicleBase::ChangeRoad(float moveSpeed, bool& isEnd)
{
	isEnd = false;
	// 正面へ移動する
	mMoveSpeed = VectorZ() * moveSpeed;

	// 左の道同士のX距離
	float xDistLeft = std::abs(CAR_LEFT_POS1.X() - CAR_LEFT_POS2.X());
	// 右の道同士のX距離
	float xDistRight = std::abs(CAR_RIGHT_POS1.X() - CAR_RIGHT_POS2.X());

	// 左の道同士の中間点のX座標
	float leftMid = CAR_LEFT_POS1.X() + xDistLeft;
	// 右の道同士の中間点のX座標
	float rightMid = CAR_RIGHT_POS1.X() - xDistLeft;

	// 今いる道によって処理
	switch (GetRoadType())
	{
	// 左から一番目の道
	case ERoadType::eLeft1:
		// まだ左の道同士の中間点を超えていないかつ
		// 回転が、左の道の初期値 + 方向転換の最大値より小さければ
		if (Position().X() <= leftMid &&
			EulerAngles().Y() <= CAR_LEFT_ROTATION.Y() + TURN_MAX.Y())
		{
			// 右を向く
			Rotation(EulerAngles() + TURN_SPEED);
		}
		// 中間点を超えているかつ
		// 元の方向を超えていない
		else if (Position().X() >= leftMid &&
			EulerAngles().Y() >= CAR_LEFT_ROTATION.Y())
		{
			// 左の方向に戻していく
			Rotation(EulerAngles() - TURN_SPEED);
		}

		// X座標が左から二番目の道の座標以上になれば
		if (Position().X() >= CAR_LEFT_POS2.X())
		{
			// 今いる道の状態を左から二番目の道に変更
			ChangeRoadType(ERoadType::eLeft2);
			Rotation(CAR_LEFT_ROTATION);
			isEnd = true;
		}
		break;
	// 左から二番目の道
	case ERoadType::eLeft2:
		// まだ左の道同士の中間点を超えていないかつ
		// 回転が、左の道の初期値 - 方向転換の最大値より大きければ
		if (Position().X() >= leftMid &&
			EulerAngles().Y() >= CAR_LEFT_ROTATION.Y() - TURN_MAX.Y())
		{
			// 左を向く
			Rotation(EulerAngles() - TURN_SPEED);
		}
		// 中間点を超えているかつ
		// 元の方向を超えていない
		else if (Position().X() <= leftMid &&
			EulerAngles().Y() <= CAR_LEFT_ROTATION.Y())
		{
			// 右の方向に戻していく
			Rotation(EulerAngles() + TURN_SPEED);
		}

		// X座標が左から一番目の道の座標以下になれば
		if (Position().X() <= CAR_LEFT_POS1.X())
		{
			// 今いる道の状態を左から一番目の道に変更
			ChangeRoadType(ERoadType::eLeft1);
			Rotation(CAR_LEFT_ROTATION);
			isEnd = true;
		}
		break;
	// 右から一番目の道
	case ERoadType::eRight1:
		// まだ右の道同士の中間点を超えていないかつ
		// 回転が、右の道の初期値 + 方向転換の最大値より小さければ
		if (Position().X() >= rightMid &&
			EulerAngles().Y() <= CAR_RIGHT_ROTATION.Y() + TURN_MAX.Y())
		{
			// 右を向く
			Rotation(EulerAngles() + TURN_SPEED);
		}
		// 中間点を超えているかつ
		// 元の方向を超えていない
		else if (Position().X() <= rightMid &&
			EulerAngles().Y() <= CAR_RIGHT_ROTATION.Y())
		{
			// 左の方向に戻していく
			Rotation(EulerAngles() - TURN_SPEED);
		}

		// X座標が右から二番目の道の座標以下になれば
		if (Position().X() <= CAR_RIGHT_POS2.X())
		{
			// 今いる道の状態を右から二番目の道に変更
			ChangeRoadType(ERoadType::eRight2);
			Rotation(CAR_RIGHT_ROTATION);
			isEnd = true;
		}
		break;
	// 右から二番目の道
	case ERoadType::eRight2:
		// まだ右の道同士の中間点を超えていないかつ
		// 回転が、右の道の初期値 - 方向転換の最大値より大きければ
		if (Position().X() <= rightMid &&
			EulerAngles().Y() >= CAR_RIGHT_ROTATION.Y() - TURN_MAX.Y())
		{
			// 左を向く
			Rotation(EulerAngles() - TURN_SPEED);
		}
		// 中間点を超えているかつ
		// 元の方向を超えていない
		else if (Position().X() >= rightMid &&
			EulerAngles().Y() >= CAR_RIGHT_ROTATION.Y())
		{
			// 右の方向に戻していく
			Rotation(EulerAngles() + TURN_SPEED);
		}

		// まだ中間点を超えていないかつ
		// 回転が、右の道の初期値 - 方向転換の最大値より小さくなければ
		else if (!(EulerAngles().Y() <= CAR_RIGHT_ROTATION.Y() - TURN_MAX.Y()))
		{
			// 左を向く
			Rotation(EulerAngles() - TURN_SPEED);
		}

		// X座標が右から一番目の道の座標以上になれば
		if (Position().X() >= CAR_RIGHT_POS1.X())
		{
			// 今いる道の状態を右から一番目の道に変更
			ChangeRoadType(ERoadType::eRight1);
			Rotation(CAR_RIGHT_ROTATION);
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
