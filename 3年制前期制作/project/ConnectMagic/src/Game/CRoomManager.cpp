#include "CRoomManager.h"
#include "CConnectRoom.h"
#include "CRoom1.h"

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
	CRoomBase* room = new CRoom1(CVector::zero);
	CConnectRoom* connectRoom =
		new CConnectRoom(room->Position() + CVector(0.0f, 0.0f, -room->GetRoomLength()));

	mRooms.push_back(room);
	mRooms.push_back(connectRoom);
}
