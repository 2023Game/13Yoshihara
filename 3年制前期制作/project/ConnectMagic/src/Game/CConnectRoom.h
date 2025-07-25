#pragma once
#include "CRoomBase.h"

class CBox;
class CSwitch;
class CSwitchMoveWall;

// �����ƕ������q���镔��
class CConnectRoom : public CRoomBase
{
public:
	// �R���X�g���N�^
	CConnectRoom(const CVector& pos);
	// �f�X�g���N�^
	~CConnectRoom();

	// ���̕�����ݒ�
	void SetNextRoom(CRoomBase* room);
	// �O�̕�����ݒ�
	void SetPreRoom(CRoomBase* room);

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
	// �t�B�[���h�I�u�W�F�N�g���폜
	void DeleteFieldObjects() override;

	// �I�u�W�F�N�g
	CBox* mpBox;
	CSwitch* mpNextSwitch;
	CSwitch* mpPreSwitch;
	CSwitchMoveWall* mpNextWall;
	CSwitchMoveWall* mpPreWall;

	// ���̕���
	CRoomBase* mpNextRoom;
	// �O�̕���
	CRoomBase* mpPreRoom;
};