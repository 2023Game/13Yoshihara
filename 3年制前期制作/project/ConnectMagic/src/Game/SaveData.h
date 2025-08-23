#pragma once
#include "MoveState.h"

class CPlayer;
class CBox;
class CMoveObj;
class CSwitchMoveFloor;

// �v���C���[�̃f�[�^
struct PlayerData {
	CVector pos;		// ���W
	CVector vec;		// ����
};

// ���̃f�[�^
struct BoxData {
	CVector pos;	// ���W
	CBox* box;		// ��

	// �R���X�g���N�^
	BoxData(const CVector& p, CBox* b)
		: pos(p), box(b) {}
};

// �ړ�����I�u�W�F�N�g�̃f�[�^
struct MoveObjData {
	CVector pos;			// ���W
	EMoveState state;		// ���
	EMoveState preState;	// 1�O�̏��
	float elapsedTime;	// �ړ��̌o�ߎ���
	CMoveObj* moveObj;	// �ړ��I�u�W�F�N�g

	// �R���X�g���N�^
	MoveObjData(const CVector& p, EMoveState s, EMoveState preS, float t,
		CMoveObj* o)
		: pos(p), state(s), preState(preS), elapsedTime(t), moveObj(o) {}
};

// �X�C�b�`�ňړ����鏰�̃f�[�^
struct MoveFloorData {
	CVector pos;		// ���W
	EMoveState state;		// ���
	EMoveState preState;	// 1�O�̏��
	float elapsedTime;	// �ړ��̌o�ߎ���
	CSwitchMoveFloor* moveFloor;	// ��

	// �R���X�g���N�^
	MoveFloorData(const CVector& p, EMoveState s, EMoveState preS, float t,
		CSwitchMoveFloor* o)
		: pos(p), state(s), preState(preS), elapsedTime(t), moveFloor(o) {}
};

// �Q�[���S�̂̃f�[�^
struct GameData {
	PlayerData player;
	std::vector<BoxData> boxes;
	std::vector<MoveObjData> moveObjs;
	std::vector<MoveFloorData> moveFloor;
};