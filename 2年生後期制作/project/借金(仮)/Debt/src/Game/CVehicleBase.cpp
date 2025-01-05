#include "CVehicleBase.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CVehicleManager.h"
#include "CNavManager.h"
#include "CNavNode.h"
 
#define FRONT_HEIGHT	13.0f	// 前方判定の高さ
#define FRONT_WIDTH		40.0f	// 前方判定の幅
#define FRONT_RADIUS	12.0f	// 前方判定の半径
#define TURN_SPEED		CVector(0.0f,0.5f,0.0f)	// 車両の方向転換速度
#define TURN_MAX		CVector(0.0f,22.5f,0.0f)// 車両の方向転換の最大値

// コンストラクタ
CVehicleBase::CVehicleBase(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road)
	: CCharaBase(ETag::eVehicle, ETaskPriority::eVehicle)
	, mpModel(model)
	, mpBodyCol(nullptr)
	, mRoadType(road)
	, mCurrentRoadRotation(rotation)
	, mState(EState::eMove)
{
	// 経路管理クラスがあるなら車両の周り用のノードを生成
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		mpNode0 = new CNavNode(Position());
		mpNode1 = new CNavNode(Position());
		mpNode2 = new CNavNode(Position());
		mpNode3 = new CNavNode(Position());
	}
	// 最初はノードを無効
	mpNode0->SetEnable(false);
	mpNode1->SetEnable(false);
	mpNode2->SetEnable(false);
	mpNode3->SetEnable(false);

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
	SAFE_DELETE(mpNavCol);
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
	return mIsMove;
}

// 車線を変更する
void CVehicleBase::ChangeRoad(bool& isEnd)
{
	isEnd = false;
	// 正面へ移動する
	mMoveSpeed = VectorZ() * GetBaseMoveSpeed();

	// 左の道同士のX距離
	float xDistLeft = std::abs(CAR_LEFT_POS1.X() - CAR_LEFT_POS2.X());
	// 右の道同士のX距離
	float xDistRight = std::abs(CAR_RIGHT_POS1.X() - CAR_RIGHT_POS2.X());

	// 左の道同士の中間点のX座標
	float leftMid = CAR_LEFT_POS1.X() + xDistLeft / 2;
	// 右の道同士の中間点のX座標
	float rightMid = CAR_RIGHT_POS1.X() - xDistLeft / 2;

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
		else if (Position().X() <= rightMid)
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
			mCurrentRoadRotation = CAR_RIGHT_ROTATION;
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

// 移動処理
void CVehicleBase::UpdateMove()
{
	// 動いている
	mIsMove = true;
	// 正面へ移動
	mMoveSpeed = VectorZ() * GetBaseMoveSpeed();
}

// 停止処理
void CVehicleBase::UpdateStop()
{
	// 動いていない
	mIsMove = false;
	// 移動速度をゼロにする
	mMoveSpeed = CVector::zero;
}

// 壊れた処理
// 移動を停止して消滅時間が経ったら表示を消す
void CVehicleBase::UpdateBroken()
{
	// 動いていない
	mIsMove = false;
	// 移動速度をゼロにする
	mMoveSpeed = CVector::zero;

	// 消滅するまでの時間をカウントダウン
	CountDeleteTime();

	// 消滅までの時間が経過したら
	if (IsElapsedDeleteTime())
	{
		// 消滅までの時間を初期値に戻す
		SetDeleteTime();
		// 状態を移動に戻しておく
		ChangeState(EState::eMove);

		// 非表示
		SetEnable(false);
		SetShow(false);
	}
}

// 車線変更処理
void CVehicleBase::UpdateChangeRoad()
{
	// 動いている
	mIsMove = true;
	bool isEnd = false;
	// 車線変更移動
	ChangeRoad(isEnd);

	// trueならば、車線変更が終わった
	if (isEnd)
	{
		// 移動状態に戻す
		ChangeState(EState::eMove);
	}
}

// 状態切り替え
void CVehicleBase::ChangeState(EState state)
{
	// 同じなら処理しない
	if (state == mState) return;

	mState = state;
}

#if _DEBUG
// 状態の文字列を取得
std::string CVehicleBase::GetStateStr(EState state) const
{
	switch (state)
	{
	// 共通
	case EState::eMove:			return "移動中";
	case EState::eStop:			return "停止中";
	case EState::eBroken:		return "壊れている";
	case EState::eChangeRoad:	return "車線変更";

	// トラック
	case EState::eCollect:		return "回収中";
	default:					return "";
	}
}
#endif