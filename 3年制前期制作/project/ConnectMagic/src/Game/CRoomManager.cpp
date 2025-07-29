#include "CRoomManager.h"
#include "CConnectRoom.h"
#include "CRoom1.h"
#include "CRoom2.h"
#include "CLastRoom.h"

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
	int i = 1;
	CRoomBase* room;					// ����
	CVector pos = CVector::zero;		// �O�̕����𐶐��������W
	CVector offsetPos = CVector::zero;	// ���̕����̃I�t�Z�b�g���W

	while (true)
	{
		// �����𐶐�
		switch (i)
		{
			// ����1
		case 1:
			room = new CRoom1(pos + offsetPos);
			break;
			// ����2
		case 2:
			room = new CRoom2(pos + offsetPos);
			break;
			// ����3
		case 3:

			break;
			// �S�Ă̕����̐������I������
		default:
			// �Ō�̕����𐶐�
			room = new CLastRoom(pos + offsetPos);
			return;
			break;
		}
		// ���W���X�V
		pos = room->Position();
		// �I�t�Z�b�g���W���X�V
		offsetPos = CVector(0.0f, 0.0f, -room->GetRoomLength());
		// ���X�g�ɒǉ�
		mRooms.push_back(room);

		// �����ƕ����̐ڑ��p�̕����𐶐�
		room = new CConnectRoom(pos + offsetPos);
		// ���W���X�V
		pos = room->Position();
		// �I�t�Z�b�g���W���X�V
		offsetPos = CVector(0.0f, 0.0f, -room->GetRoomLength());
		// ���X�g�ɒǉ�
		mRooms.push_back(room);

		// ����
		i++;
	}
}
