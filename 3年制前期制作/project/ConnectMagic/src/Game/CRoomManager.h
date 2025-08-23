#pragma once

class CRoomBase;

class CRoomManager
{
public:
	// �C���X�^���X���擾
	static CRoomManager* Instance();

	// �R���X�g���N�^
	CRoomManager();
	// �f�X�g���N�^
	~CRoomManager();

	// �X�V
	void Update();

	// �����̃��X�g���擾
	std::vector<CRoomBase*> GetRooms() const;

	// ������L��
	void RoomOn();
	// �����𖳌�
	void RoomOff();

private:
	// �C���X�^���X
	static CRoomManager* spInstance;

	// �������쐬
	void CreateRoom();

	// �����̃��X�g
	std::vector<CRoomBase*> mRooms;

	// �L���ɂ��镔���̔ԍ�
	int mOnNum;
	// �����ɂ��镔���̔ԍ�
	int mOffNum;
};