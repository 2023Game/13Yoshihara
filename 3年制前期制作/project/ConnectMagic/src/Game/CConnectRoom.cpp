#include "CConnectRoom.h"
#include "CSwitch.h"
#include "CSwitchMoveWall.h"
#include "CColliderSphere.h"
#include "CShield.h"
#include "CRoomManager.h"

// 箱のオフセット座標
#define BOX_OFFSET_POS CVector(25.0f,0.0f,-40.0f)
// スイッチのオフセット座標
#define SWITCH_OFFSET_POS_NEXT	CVector(-25.0f,0.0f,-40.0f)
// 部屋の長さ
#define ROOM_LENGTH 80.0f

// 動く壁のオフセット座標
#define MOVE_WALL_OFFSET_POS_NEXT	CVector(0.0f,0.0f,-70.0f)
#define MOVE_WALL_OFFSET_POS_PRE	CVector(0.0f,0.0f,0.0f)
// 動く壁のスケール
#define MOVE_WALL_SCALE CVector(2.0f,4.0f,1.0f)
// 動く壁の移動
#define MOVE_WALL_MOVE	CVector(0.0f,40.0f,0.0f)
// 動く壁の移動時間
#define MOVE_WALL_MOVE_TIME 0.5f

// 閉じるフラグ用のコライダーの半径
#define COL_RADIUS 15.0f
// コライダーの位置
#define COL_POS CVector(0.0f,0.0f,-45.0f)

// シールドのオフセット座標
#define SHIELD_OFFSET_POS CVector(0.0f,0.0f,-75.0f)

// コンストラクタ
CConnectRoom::CConnectRoom(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
	, mpNextRoom(nullptr)
	, mpPreRoom(nullptr)
{
	mpModel = CResourceManager::Get<CModel>("ConnectRoom");
	// 座標を設定
	Position(pos);

	// コライダー生成
	CreateCol();

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CConnectRoom::~CConnectRoom()
{
	SAFE_DELETE(mpCloseCol);
}

// 次の部屋を設定
void CConnectRoom::SetNextRoom(CRoomBase* room)
{
	mpNextRoom = room;
}

// 前の部屋を設定
void CConnectRoom::SetPreRoom(CRoomBase* room)
{
	mpPreRoom = room;
}

// 衝突処理
void CConnectRoom::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 基底クラスの衝突処理
	CFieldBase::Collision(self, other, hit);

	// 閉じるフラグ用のコライダー
	if (self == mpCloseCol)
	{
		// プレイヤーなら
		if (other->Layer() == ELayer::ePlayer)
		{
			// 前の部屋に繋がる壁を閉じる
			mpPreWall->SetOnOff(false);
		}
	}
}

// 部屋の有効無効を設定
void CConnectRoom::SetEnableRoom(bool enable)
{
	CRoomBase::SetEnableRoom(enable);
	mpNextSwitch->SetEnableSwitch(enable);
}

// 更新
void CConnectRoom::Update()
{
	// 次が無効かつ
	// 壁が開いているなら
	if (!mpNextRoom->IsEnable() &&
		mpNextWall->IsOpen())
	{
		// 部屋管理クラス
		CRoomManager* roomMgr = CRoomManager::Instance();
		// 部屋を有効
		roomMgr->RoomOn();
	}

	// 前が有効かつ
	// 壁が開いていないなら
	if (mpPreRoom->IsEnable() &&
		!mpPreWall->IsOpen())
	{
		// 部屋管理クラス
		CRoomManager* roomMgr = CRoomManager::Instance();
		// 部屋を無効
		roomMgr->RoomOff();
	}
}

// コライダーの生成
void CConnectRoom::CreateCol()
{
	// コライダーを生成
	CFieldBase::CreateCol("ConnectRoom_Ground_Col", "ConnectRoom_Wall_Col", "");

	// 閉じるフラグ用のコライダー
	mpCloseCol = new CColliderSphere(
		this, ELayer::eSwitch,
		COL_RADIUS, true
	);
	mpCloseCol->Position(COL_POS);
	mpCloseCol->SetCollisionLayers({ ELayer::ePlayer });
}

// フィールドオブジェクトを生成
void CConnectRoom::CreateFieldObjects()
{
	// スイッチを生成
	mpNextSwitch = new CSwitch(Position() + SWITCH_OFFSET_POS_NEXT, true);
	// 動く壁を生成
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	CModel* colModel = CResourceManager::Get<CModel>("MoveObject_Col");
	mpNextWall = new CSwitchMoveWall(model, colModel,
		Position() + MOVE_WALL_OFFSET_POS_NEXT, 
		MOVE_WALL_SCALE, 
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME, true);
	mpPreWall = new CSwitchMoveWall(model, colModel,
		Position() + MOVE_WALL_OFFSET_POS_PRE,
		MOVE_WALL_SCALE,
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME, true);
	// 最初から開いている
	mpPreWall->SetIsOpen(true);
	// 作用するスイッチに設定
	mpNextWall->SetSwitchs({ mpNextSwitch });
	// 作用するオブジェクトに設定
	mpNextSwitch->SetActionObj(mpNextWall);
	// リストに追加
	mObjs.push_back(mpNextWall);
	mObjs.push_back(mpPreWall);

	// シールドを生成
	mpShield = new CShield();
	mpShield->Position(Position() + SHIELD_OFFSET_POS);
	// リストに追加
	mObjs.push_back(mpShield);
}