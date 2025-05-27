#include "CFire.h"
#include "CFlamethrower.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"

// 重量（動かさない）
#define WEIGHT 1.0f

// 接続ターゲットのオフセット座標
#define TARGET_OFFSET_POS CVector(0.0f,2.0f,0.0f)

// コライダーの半径
#define RADIUS 5.0f

// 炎の移動速度
#define MOVE_SPEED 2.0f

// 松明の縦の長さ
#define TORCH_HEIGHT 7.0f

// コンストラクタ
CFire::CFire(std::string modelName, CVector fireOffsetPos, float fireScale)
	: CConnectObject(WEIGHT,ETaskPriority::eBillboard)
	, mIsFire(false)
{
	// 接続ターゲットのタグを炎に設定
	SetConnectObjTag(EConnectObjTag::eFire);

	// モデルの名前が設定されているなら
	if (modelName != "")
	{
		// モデルを設定
		mpModel = CResourceManager::Get<CModel>(modelName.c_str());
	}
	// 重力無効
	mIsGravity = false;

	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector::zero,
		CVector::up
	);
	// 移動速度を設定
	mpFlamethrower->SetFlameMoveSpeed(MOVE_SPEED);
	// スケールを設定
	mpFlamethrower->SetFlameScale(fireScale);
	// スケール値は変化していかない
	mpFlamethrower->SetFlameScaleAnimTime(0.0f);
	// オフセット座標を設定
	mpFlamethrower->SetThrowOffsetPos(fireOffsetPos);

	// 接続ターゲットを生成
	CreateTarget(Position() + fireOffsetPos + TARGET_OFFSET_POS);

	// コライダーを生成
	CreateCol(modelName);
}

// デストラクタ
CFire::~CFire()
{
}

// 繋がったときの処理
void CFire::Connect(CConnectObject* other)
{
	// 炎の接続オブジェクトなら
	if (other->GetConnectObjTag() == EConnectObjTag::eFire)
	{
		// 炎のクラスを取得
		CFire* fire = dynamic_cast<CFire*>(other);
		
		// 相手の炎がついているなら
		if (fire->GetFire())
		{
			if (!mIsFire)
			{
				// 自分の炎を付ける
				SetFire(true);
			}
		}
	}
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

// コライダーを生成
void CFire::CreateCol(std::string modelName)
{
	// 空の場合
	if (modelName == "")
	{
		mpCol = new CColliderSphere
		(
			this, ELayer::eObject,
			RADIUS, true
		);
		// 接続オブジェクトの探知用とだけ衝突判定
		mpCol->SetCollisionLayers({ ELayer::eConnectSearch });
	}
	// それ以外の場合
	else
	{
		mpCol = new CColliderCapsule
		(
			this, ELayer::eObject,
			CVector(0.0f, 0.0f, 0.0f) , CVector(0.0f, TORCH_HEIGHT, 0.0f),
			RADIUS, true
		);
		// プレイヤーと接続オブジェクトの探知用とだけ衝突判定
		mpCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eConnectSearch });
	}
}
