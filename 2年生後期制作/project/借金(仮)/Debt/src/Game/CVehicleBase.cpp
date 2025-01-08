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