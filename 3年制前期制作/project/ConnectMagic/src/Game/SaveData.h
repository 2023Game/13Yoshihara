#pragma once
#include "MoveState.h"

class CPlayer;
class CBox;
class CMoveObj;
class CSwitchMoveFloor;
class CSwitchMoveWall;
class CConnectTarget;

// �v���C���[�̃f�[�^
struct PlayerData {
	CVector pos;		// ���W
	CVector vec;		// ����
	CConnectTarget* target;	// �ڑ����̃^�[�Q�b�g
	int animationType;		// �A�j���[�V�����̔ԍ�
	float animationFrame;	// �A�j���[�V�����̐i�s����

	// �R���X�g���N�^
	PlayerData(const CVector& p, const CVector& v, CConnectTarget* t, int type, float frame)
		: pos(p), vec(v), target(t), animationType(type), animationFrame(frame) {}
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

// �X�C�b�`�ňړ�����ǂ̃f�[�^
struct MoveWallData {
	CVector pos;			// ���W
	EMoveState state;		// ���
	EMoveState preState;	// 1�O�̏��
	float elapsedTime;		// �ړ��̌o�ߎ���
	CSwitchMoveWall* moveWall;	// ��

	// �R���X�g���N�^
	MoveWallData(const CVector& p, EMoveState s, EMoveState preS, float t,
		CSwitchMoveWall* o)
		: pos(p), state(s), preState(preS), elapsedTime(t), moveWall(o) {}
};

// �Q�[���S�̂̃f�[�^
struct GameData {
	PlayerData player = PlayerData(CVector::zero, CVector::zero, nullptr, 0, 0.0f);
	std::vector<BoxData> boxes;
	std::vector<MoveObjData> moveObjs;
	std::vector<MoveFloorData> moveFloor;
	std::vector<MoveWallData> moveWall;
};