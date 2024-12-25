#include "CCar.h"
#include "CColliderCapsule.h"
#include "CModel.h"

#define CAR_HEIGHT		9.0f	// 車の高さ
#define CAR_WIDTH		25.0f	// 車の幅
#define CAR_RADIUS	10.0f	// 車の半径

CCar::CCar(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road)
	: CVehicleBase(model, pos, rotation, road)
	, mState(EState::eMove)
{
	// プレイヤー、敵、生成場所、車両、地形
	// と衝突判定する本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eVehicle,
		CVector(0.0f, CAR_HEIGHT, CAR_WIDTH - CAR_RADIUS),
		CVector(0.0f, CAR_HEIGHT, -CAR_WIDTH + CAR_RADIUS),
		CAR_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,ETag::eSpawnZone,
		ETag::eVehicle,ETag::eField, });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eSpawnZone,ELayer::eVehicle,
		ELayer::eField,ELayer::eWall,ELayer::eObject });

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
	case EState::eMove:		UpdateMove();	break;
	// 停止
	case EState::eStop:		UpdateStop();	break;
	// 壊れた
	case EState::eBroken:	UpdateBroken();	break;
	// 車線変更
	case EState::eChangeRoad: UpdateChangeRoad(); break;
	}

	CVehicleBase::Update();

#if _DEBUG
	CDebugPrint::Print("CarState:%s\n", GetStateStr(mState).c_str());
#endif
}

// 衝突処理
void CCar::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CVehicleBase::Collision(self, other, hit);

	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// 衝突した相手がプレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// 壊れた状態に変更
			ChangeState(EState::eBroken);
		}
	}
	// 前方判定コライダ―
	if (self == mpFrontCol)
	{
		// 衝突した相手が車両の場合
		if (other->Layer() == ELayer::eVehicle)
		{
			// 車両クラスを取得
			CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());
			// 相手が動いていなければ
			if (!vehicle->IsMove())
			{
				// 車線変更状態へ
				ChangeState(EState::eChangeRoad);
			}
		}
	}
}

// 移動処理
void CCar::UpdateMove()
{
	// 正面へ移動
	mMoveSpeed = VectorZ() * GetBaseMoveSpeed();
}

// 停止処理
void CCar::UpdateStop()
{
	// 移動速度をゼロにする
	mMoveSpeed = CVector::zero;
}

// 壊れた処理
// 移動を停止して消滅時間が経ったら表示を消す
void CCar::UpdateBroken()
{
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
void CCar::UpdateChangeRoad()
{
	bool isEnd = false;
	// 車線変更移動
	ChangeRoad(GetBaseMoveSpeed(), isEnd);

	// trueならば、車線変更が終わった
	if (isEnd)
	{
		// 移動状態に戻す
		ChangeState(EState::eMove);
	}
}

// 状態切り替え
void CCar::ChangeState(EState state)
{
	// 同じなら処理しない
	if (state == mState) return;

	mState = state;
}

#if _DEBUG
// 状態の文字列を取得
std::string CCar::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eMove:		return "移動中";
	case EState::eStop:		return "停止中";
	case EState::eBroken:	return "壊れている";
	default:				return "";
	}
}
#endif
