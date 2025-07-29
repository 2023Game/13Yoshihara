#include "CBox.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
#include "CConnectPointManager.h"

// 接続ターゲットの座標
#define TARGET_POS_1 CVector( 0.0f,10.0f, 0.0f)
#define TARGET_POS_2 CVector( 0.0f, 0.0f, 0.0f)
#define TARGET_POS_3 CVector( 6.0f, 5.0f, 0.0f)
#define TARGET_POS_4 CVector(-6.0f, 5.0f, 0.0f)
#define TARGET_POS_5 CVector( 0.0f, 5.0f, 6.0f)
#define TARGET_POS_6 CVector( 0.0f, 5.0f,-6.0f)

// 箱の重さ
#define WEIGHT 0.1f

// コライダーの半径
#define RADIUS 2.4f

// スケールの倍率
#define SCALE 2.0f

// 水に当たって返ってくるまでの時間
#define RETURN_TIME 0.5f

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

	std::vector<CVector> targetPosList = { TARGET_POS_1,TARGET_POS_2,TARGET_POS_3,TARGET_POS_4,TARGET_POS_5,TARGET_POS_6 };
	for (int i = 0; i < targetPosList.size(); i++)
	{
		CVector pos = targetPosList[i] * scaleRatio;
		// 1つ目でなければ
		if (i != 0)
		{
			// 元のY座標に設定
			pos.Y(targetPosList[i].Y());
		}
		// 接続ターゲットを生成
		CreateTarget(pos);
	}

	Position(defaultPos);
}

// デストラクタ
CBox::~CBox()
{
	SAFE_DELETE(mpCharaCol);
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
		if (other->Layer() == ELayer::eWater)
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

	CConnectObject::Update();
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
	// フィールド、オブジェクト、コネクトオブジェクトの探知用、スイッチ、水と衝突判定
	mpCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall,
		ELayer::eObject, ELayer::eConnectSearch, ELayer::eSwitch,
		ELayer::eWater});

	// キャラと衝突判定をするコライダー
	mpCharaCol = new CColliderMesh
	(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("Box_Col")
	);
	// プレイヤー、敵、オブジェクトと衝突判定
	mpCharaCol->SetCollisionLayers({ ELayer::ePlayer, ELayer::eEnemy, ELayer::eObject });

	// 接続部の管理クラスの衝突判定するコライダーに追加
	CConnectPointManager::Instance()->AddCollider(mpCharaCol);

	// カメラの衝突判定するコライダーに追加
	CCamera::CurrentCamera()->AddCollider(mpCharaCol);
}
