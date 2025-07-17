#include "CFire.h"
#include "CFlamethrower.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
#include "CColliderMesh.h"

// 重量（動かさない）
#define WEIGHT 1.0f

// コライダー半径
#define RADIUS 5.0f

// コンストラクタ
CFire::CFire(CVector fireOffsetPos,
	float fireScale, float fireSpeed, CVector targetOffsetPos)
	: CConnectObject(WEIGHT,ETaskPriority::eBillboard)
	, mIsFire(false)
{
	// 接続ターゲットのタグを炎に設定
	SetConnectObjTag(EConnectObjTag::eFire);

	// 重力無効
	mIsGravity = false;

	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector::zero,
		CVector::up
	);
	// 移動速度を設定
	mpFlamethrower->SetFlameMoveSpeed(fireSpeed);
	// スケールを設定
	mpFlamethrower->SetFlameScale(fireScale);
	// スケール値は変化していかない
	mpFlamethrower->SetFlameScaleAnimTime(0.0f);
	// オフセット座標を設定
	mpFlamethrower->SetThrowOffsetPos(fireOffsetPos);

	// 接続ターゲットを生成
	CreateTarget(Position() + fireOffsetPos + targetOffsetPos);
}

// デストラクタ
CFire::~CFire()
{
}

// 繋がったときの処理
void CFire::Connect(CVector wandPointPos, CVector targetPointPos)
{
	//// 炎の接続オブジェクトなら
	//if (other->GetConnectObjTag() == EConnectObjTag::eFire)
	//{
	//	// 炎のクラスを取得
	//	CFire* fire = dynamic_cast<CFire*>(other);
	//	
	//	// 相手の炎がついているなら
	//	if (fire->GetFire())
	//	{
	//		if (!mIsFire)
	//		{
	//			// 自分の炎を付ける
	//			SetFire(true);
	//		}
	//	}
	//}
}

// 炎がついているかを設定
void CFire::SetFire(bool isEnable)
{
	// 炎がついているかを設定
	mIsFire = isEnable;
	// 炎がついているなら
	if (mIsFire)
	{
		// 炎の生成開始
		mpFlamethrower->Start();
		// 燃えたときの処理
		Burning();
	}
	// 炎がついていないなら
	else
	{
		// 炎の生成停止
		mpFlamethrower->Stop();
	}
}

// 炎がついているかを取得
bool CFire::GetFire() const
{
	return mIsFire;
}

void CFire::CreateCol()
{
	mpCol = new CColliderSphere(
		this, ELayer::eObject,
		RADIUS, true
	);
	// 接続オブジェクトの探知用とだけ衝突判定
	mpCol->SetCollisionLayers({ ELayer::eConnectSearch });
}

// 燃えた時の処理
void CFire::Burning()
{
}