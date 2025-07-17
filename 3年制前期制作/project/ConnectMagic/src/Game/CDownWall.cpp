#include "CDownWall.h"
#include "CColliderMesh.h"
#include "CConnectPointManager.h"

// 移動量
#define MOVE_DIST 50.0f
// 最大移動量
#define MAX_MOVE_DIST 125.0f

// コンストラクタ
CDownWall::CDownWall(CVector defaultPos)
	: CSwitchObject(ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mDefaultPosY(defaultPos.Y())
{
	mpModel = CResourceManager::Get<CModel>("DownWall");

	// コライダーを生成
	CreateCol();

	Position(defaultPos);
}

// デストラクタ
CDownWall::~CDownWall()
{
}

// コライダーを生成
void CDownWall::CreateCol()
{
	mpCol = new CColliderMesh(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("DownWall"),
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
void CDownWall::UpdateOff()
{
	// 初期値よりY座標が小さければ
	if (Position().Y() < mDefaultPosY)
	{
		// 上に移動
		CVector pos = Position();
		pos.Y(pos.Y() + MOVE_DIST * Times::DeltaTime());
		Position(pos);
	}
	// 初期値よりY座標が大きければ
	else if(Position().Y() > mDefaultPosY)
	{
		// Y座標を初期値に設定
		CVector pos = Position();
		pos.Y(mDefaultPosY);
		Position(pos);
	}
}

// 作用している時の処理
void CDownWall::UpdateOn()
{
	// 初期値-最大移動量よりY座標が大きければ
	if (Position().Y() > mDefaultPosY - MAX_MOVE_DIST)
	{
		// 下に移動
		CVector pos = Position();
		pos.Y(pos.Y() - MOVE_DIST * Times::DeltaTime());
		Position(pos);
	}
	// 初期値-最大量よりY座標が小さければ
	else if (Position().Y() < mDefaultPosY - MAX_MOVE_DIST)
	{
		// Y座標を初期値-最大量に設定
		CVector pos = Position();
		pos.Y(mDefaultPosY - MAX_MOVE_DIST);
		Position(pos);
	}
}
