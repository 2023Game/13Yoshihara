#include "CRoomManager.h"
#include "CConnectRoom.h"
#include "CRoom1.h"
#include "CRoom2.h"
#include "CRoom3.h"
#include "CRoom4.h"
#include "CRoom5.h"
#include "CLastRoom.h"
#include "CSaveManager.h"

CRoomManager* CRoomManager::spInstance = nullptr;

// �C���X�^���X���擾
CRoomManager* CRoomManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CRoomManager::CRoomManager()
	: mOnNum(2)
	, mOffNum(0)
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

// ������L��
void CRoomManager::RoomOn()
{
	// ������L��
	// 2�񏈗�����
	for (int i = 0; i < 2; i++)
	{
		// �T�C�Y�ȏ�Ȃ珈�����I��
		if (mOnNum >= mRooms.size()) return;

		mRooms[mOnNum]->SetEnableRoom(true);
		mOnNum++;
	}
}

// �����𖳌�
void CRoomManager::RoomOff()
{
	// ������
	int num = 2;
	// ����Ȃ�1�񂾂�w
	if (mOffNum == 0) num = 1;

	// �����𖳌�
	// 2�񏈗�����
	for (int i = 0; i < num; i++)
	{
		// �T�C�Y�ȏ�Ȃ珈�����I��
		if (mOffNum >= mRooms.size()) return;

		mRooms[mOffNum]->SetEnableRoom(false);
		mOffNum++;
	}

	// �S�Ẵf�[�^���폜
	CSaveManager::Instance()->AllDelete();
}

// �������쐬
void CRoomManager::CreateRoom()
{
	int i = 1;
	CRoomBase* room = nullptr;					// ����
	CConnectRoom* connectRoom = nullptr;		// �ڑ�����
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
			// �L��
			room->SetEnableRoom(true);
			break;
			// ����2
		case 2:
			room = new CRoom2(pos + offsetPos);
			break;
			// ����3
		case 3:
			room = new CRoom3(pos + offsetPos);
			break;
			// ����4
		case 4:
			room = new CRoom4(pos + offsetPos);
			break;
			// ����5
		case 5:
			room = new CRoom5(pos + offsetPos);
			break;
			// �S�Ă̕����̐������I������
		default:
			// �Ō�̕����𐶐�
			room = new CLastRoom(pos + offsetPos);
			// ���X�g�ɒǉ�
			mRooms.push_back(room);
			// ����
			room->SetEnableRoom(false);

			// �ڑ�����������Ȃ�
			if (connectRoom != nullptr)
			{
				// ���̕�����ݒ�
				connectRoom->SetNextRoom(room);
			}
			return;
			break;
		}

		// ���W���X�V
		pos = room->Position();
		// �I�t�Z�b�g���W���X�V
		offsetPos = CVector(0.0f, 0.0f, -room->GetRoomLength());
		// ���X�g�ɒǉ�
		mRooms.push_back(room);
		// �ŏ��ȊO�͖���
		if (i != 1)
		{
			room->SetEnableRoom(false);
		}

		// �ڑ�����������Ȃ�
		if (connectRoom != nullptr)
		{
			// ���̕�����ݒ�
			connectRoom->SetNextRoom(room);
		}
		// �����ƕ����̐ڑ��p�̕����𐶐�
		connectRoom = new CConnectRoom(pos + offsetPos);
		// �O�̕�����ݒ�
		connectRoom->SetPreRoom(room);
		// ���W���X�V
		pos = connectRoom->Position();
		// �I�t�Z�b�g���W���X�V
		offsetPos = CVector(0.0f, 0.0f, -connectRoom->GetRoomLength());
		// ���X�g�ɒǉ�
		mRooms.push_back(connectRoom);
		// �ŏ��ȊO�͖���
		if (i != 1)
		{
			connectRoom->SetEnableRoom(false);
		}

		// ����
		i++;
	}
}
