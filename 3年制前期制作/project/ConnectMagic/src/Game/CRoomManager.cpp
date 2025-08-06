#include "CRoomManager.h"
#include "CConnectRoom.h"
#include "CRoom1.h"
#include "CRoom2.h"
#include "CRoom3.h"
#include "CRoom4.h"
#include "CRoom5.h"
#include "CLastRoom.h"

CRoomManager* CRoomManager::spInstance = nullptr;

// インスタンスを取得
CRoomManager* CRoomManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CRoomManager::CRoomManager()
{
	spInstance = this;
	CreateRoom();
}

// デストラクタ
CRoomManager::~CRoomManager()
{
}

// 更新
void CRoomManager::Update()
{
}

std::vector<CRoomBase*> CRoomManager::GetRooms() const
{
	return mRooms;
}

// 部屋を作成
void CRoomManager::CreateRoom()
{
	int i = 1;
	CRoomBase* room;					// 部屋
	CVector pos = CVector::zero;		// 前の部屋を生成した座標
	CVector offsetPos = CVector::zero;	// 今の部屋のオフセット座標

	while (true)
	{
		// 部屋を生成
		switch (i)
		{
			// 部屋1
		case 1:
			room = new CRoom1(pos + offsetPos);
			break;
			// 部屋2
		case 2:
			room = new CRoom2(pos + offsetPos);
			break;
			// 部屋3
		case 3:
			room = new CRoom3(pos + offsetPos);
			break;
			// 部屋4
		case 4:
			room = new CRoom4(pos + offsetPos);
			break;
			// 部屋5
		case 5:
			room = new CRoom5(pos + offsetPos);
			break;
			// 全ての部屋の生成が終了した
		default:
			// 最後の部屋を生成
			room = new CLastRoom(pos + offsetPos);
			// リストに追加
			mRooms.push_back(room);
			return;
			break;
		}
		// 座標を更新
		pos = room->Position();
		// オフセット座標を更新
		offsetPos = CVector(0.0f, 0.0f, -room->GetRoomLength());
		// リストに追加
		mRooms.push_back(room);

		// 部屋と部屋の接続用の部屋を生成
		room = new CConnectRoom(pos + offsetPos);
		// 座標を更新
		pos = room->Position();
		// オフセット座標を更新
		offsetPos = CVector(0.0f, 0.0f, -room->GetRoomLength());
		// リストに追加
		mRooms.push_back(room);

		// 次へ
		i++;
	}
}
