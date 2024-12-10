#include "CCar.h"
#include "CColliderCapsule.h"
#include "CModel.h"

#define CAR_HEIGHT 5.0f
#define CAR_WIDTH 5.0f
#define CAPSULE_RADIUS 5.0f

CCar::CCar(CModel* model, const CVector& pos, const CVector& rotation)
	: CCharaBase(ETag::eCar, ETaskPriority::eDefault)
	, mpModel(model)
	, mState(EState::eMove)
{
	Position(pos);
	Rotation(rotation);

	// プレイヤー、敵、スポーンゾーンとだけ衝突判定をする
	mpColliderCapsule = new CColliderCapsule
	{
		this,ELayer::eVehicle,
		CVector(CAR_HEIGHT - CAPSULE_RADIUS * 10,CAR_HEIGHT,0.0f),
		CVector(-CAR_HEIGHT + CAPSULE_RADIUS * 10,CAR_HEIGHT,0.0f),
		CAPSULE_RADIUS
	};
	mpColliderCapsule->SetCollisionLayers({ ELayer::ePlayer, ELayer::eEnemy, ELayer::eSpawnZone });

	// 最初は描画、更新しない
	SetEnable(false);
	SetShow(false);
}

CCar::~CCar()
{
}

void CCar::Update()
{
	switch (mState)
	{
	// 移動
	case EState::eMove:	UpdateMove();	break;
	// 壊れた
	case EState::eBroken:	UpdateBroken();	break;


		break;
	}

	CDebugPrint::Print("CarState:%s\n", GetStateStr(mState).c_str());
}

// 衝突処理
void CCar::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderCapsule)
	{

	}
}

void CCar::Render()
{
	mpModel->Render(Matrix());
}

// 移動処理
void CCar::UpdateMove()
{
	// 正面へ移動
	Position(Position() + VectorZ() * mCarStatus.GetBaseMoveSpeed());
}

// 壊れた処理
// 移動を停止して消滅時間が経ったら表示を消す
void CCar::UpdateBroken()
{
	// 消滅するまで
	mCarStatus.CountDeleteTime();
	
	if (mCarStatus.IsElapsedDeleteTime())
	{
		SetEnable(false);
		SetShow(false);
	}
}

// ステータス切り替え
void CCar::ChangeState(EState state)
{
	// 同じなら処理しない
	if (state == mState) return;

	mState = state;
}

// 状態の文字列を取得
std::string CCar::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eMove:		return "移動中";
	case EState::eStop:		return "停止中";
	case EState::eBroken:	return "壊れている";
	case EState::eCollect:	return "回収中";
	}
	return "";
}
