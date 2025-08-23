#pragma once
#include "CRoomBase.h"

class CSwitch;
class CSwitchMoveWall;
class CShield;

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

	// �����̗L��������ݒ�
	void SetEnableRoom(bool enable) override;

	// �X�V
	void Update() override;

private:
	// �R���C�_�[�̐���
	void CreateCol() override;
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;

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

	// �I�u�W�F�N�g��ʂ��Ȃ��V�[���h
	CShield* mpShield;
};