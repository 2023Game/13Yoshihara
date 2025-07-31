#pragma once
#include "CRoomBase.h"

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

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	// �R���C�_�[�̐���
	void CreateCol() override;
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
	// �t�B�[���h�I�u�W�F�N�g���폜
	void DeleteFieldObjects() override;

	// �I�u�W�F�N�g
	CSwitch* mpNextSwitch;
	CSwitchMoveWall* mpNextWall;
	CSwitchMoveWall* mpPreWall;

	// ���̕���
	CRoomBase* mpNextRoom;
	// �O�̕���
	CRoomBase* mpPreRoom;

	// �O�̕����Ɍq�����������t���O�p�̃R���C�_�[
	CCollider* mpCloseCol;
};