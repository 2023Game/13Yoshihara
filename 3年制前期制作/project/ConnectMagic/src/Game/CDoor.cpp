#include "CDoor.h"
#include "CColliderMesh.h"
#include "CConnectPointManager.h"

// 開いたときの回転
#define OPEN_ROT 90.0f
// 回転速度
#define ROTATE_SPEED 2.0f

// 閾値
#define THRESHOLD 1.0f

// コンストラクタ
CDoor::CDoor(const CVector& rot)
	: CSwitchObject()
	, mDefaultRotation(rot)
{
	mpModel = CResourceManager::Get<CModel>("Door");

	// コライダーを生成
	CreateCol();

	// 回転を設定
	Rotation(rot);
}

// デストラクタ
CDoor::~CDoor()
{
}

// コライダーを生成
void CDoor::CreateCol()
{
	mpCol = new CColliderMesh
	(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("Door_Col"),
		true
	);
	// プレイヤー、敵、オブジェクトと衝突判定
	mpCol->SetCollisionLayers({ ELayer::ePlayer,
		ELayer::eEnemy,ELayer::eObject });

	// 接続部の管理クラスの衝突判定するコライダーに追加
	CConnectPointManager::Instance()->AddCollider(mpCol);

	// カメラの衝突判定するコライダーに追加
	CCamera::CurrentCamera()->AddCollider(mpCol);
}

// 作用していない時の処理
void CDoor::UpdateOff()
{
	// 閉じた状態より回転が大きければ
	if (EulerAngles().Y() > mDefaultRotation.Y() &&
		EulerAngles().Y() <= mDefaultRotation.Y() + OPEN_ROT + THRESHOLD)
	{
		RotateAxis(CVector::up, -ROTATE_SPEED);
	}
	// 小さいなら閉じた状態を設定
	else
	{
		// 回転角度を取得
		CVector rot = EulerAngles();
		// 初期値を設定
		rot.Y(mDefaultRotation.Y());
		Rotation(rot);
	}
}

// 作用している時の処理
void CDoor::UpdateOn()
{
	// 開いた状態より回転が小さければ
	if (EulerAngles().Y() < mDefaultRotation.Y() + OPEN_ROT - THRESHOLD)
	{
		RotateAxis(CVector::up, ROTATE_SPEED);
	}
	// 大きいなら開いた状態を設定
	else
	{
		// 回転角度を取得
		CVector rot = EulerAngles();
		// 初期値＋開いたときの回転を設定
		rot.Y(mDefaultRotation.Y() + OPEN_ROT);
		Rotation(rot);
	}
}
