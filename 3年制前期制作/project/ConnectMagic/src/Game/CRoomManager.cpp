#include "CRoomManager.h"
#include "CConnectRoom.h"
#include "CRoom1.h"

CRoomManager* CRoomManager::spInstance = nullptr;

// �C���X�^���X���擾
CRoomManager* CRoomManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CRoomManager::CRoomManager()
{
	spInstance = this;
	CreateRoom();
}

// �f�X�g���N�^
CRoomManager::~CRoomManager()
{
}

// �X�V
void CRoomManager::Update()
{
}

std::vector<CRoomBase*> CRoomManager::GetRooms() const
{
	return mRooms;
}

// �������쐬
void CRoomManager::CreateRoom()
{
	CRoomBase* room = new CRoom1(CVector::zero);
	CConnectRoom* connectRoom =
		new CConnectRoom(room->Position() + CVector(0.0f, 0.0f, -room->GetRoomLength()));

	mRooms.push_back(room);
	mRooms.push_back(connectRoom);
}
