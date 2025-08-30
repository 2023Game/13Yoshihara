#include "CBox.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
#include "CConnectPointManager.h"

// 接続ターゲットの座標
#define TARGET_POS_1 CVector( 0.0f,10.0f, 0.0f)

// 箱の重さ
#define WEIGHT 0.1f

// コライダーの半径
#define RADIUS 2.4f

// スケールの倍率
#define SCALE 2.0f

// 水に当たって返ってくるまでの時間
#define RETURN_TIME 0.5f

// 削除されるY座標
#define DELETE_POS_Y -50.0f

// コンストラクタ
CBox::CBox(CVector defaultPos, float scaleRatio)
	: CConnectObject(WEIGHT)
	, mDefaultPos(defaultPos)
	, mIsRespawn(false)
	, mElapsedTime(0.0f)
{
	mpModel = CResourceManager::Get<CModel>("Box");

	Scale(Scale() * SCALE * scaleRatio);

	// コライダーを生成
	CreateCol();
	
	// 接続ターゲットを生成
	CreateTarget(TARGET_POS_1);

	Position(defaultPos);
}

// デストラクタ
CBox::~CBox()
{
}

// 衝突処理
void CBox::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CConnectObject::Collision(self, other, hit);

	if (mpCol == self)
	{
		// 相手が地面なら
		if (other->Layer() == ELayer::eGround)
		{
			// 乗ることが出来るオブジェクトなら
			if (other->Tag() == ETag::eRideableObject)
			{
				// 乗っているオブジェクトに設定する
				mpRideObject = other->Owner();
			}
		}
		// 相手が水なら
		if (other->Layer() == ELayer::eCrushed)
		{
			mIsRespawn = true;
		}
	}
}

// 更新
void CBox::Update()
{
	// リスポーンするなら
	if (mIsRespawn)
	{
		// 時間を経過
		mElapsedTime += Times::DeltaTime();
		if (mElapsedTime >= RETURN_TIME)
		{
			// リスポーン無効
			mIsRespawn = false;
			// 経過時間リセット
			mElapsedTime = 0.0f;
			// 初期座標に戻す
			Position(mDefaultPos);
		}
	}

	// 削除される座標以下なら
	if (Position().Y() <= DELETE_POS_Y)
	{
		SetEnable(false);
	}

	CConnectObject::Update();
	// 箱が張り付いていない
	SetIsAttach(false);
}

// 箱がスイッチに張り付いているかを設定
void CBox::SetIsAttach(bool enable)
{
	mIsAttach = enable;
}

// 箱がスイッチに張り付いているか
bool CBox::GetIsAttach() const
{
	return mIsAttach;
}

// コライダーを生成
void CBox::CreateCol()
{
	// フィールドやオブジェクトと衝突判定をするコライダー
	mpCol = new CColliderSphere
	(
		this, ELayer::eObject,
		RADIUS
	);
	// 座標を調整
	mpCol->Position(Position() + CVector(0.0f, RADIUS / 2, 0.0f));
	// プレイヤー、敵、フィールド、オブジェクト、コネクトオブジェクトの探知用、スイッチ、水と衝突判定
	mpCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eGround, ELayer::eWall,
		ELayer::eObject, ELayer::eConnectSearch, ELayer::eSwitch,
		ELayer::eCrushed});
}
