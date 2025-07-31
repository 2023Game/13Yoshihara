#include "CRoom5.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CSwitchMoveWall.h"
#include "CMoveObj.h"
#include "CWater.h"
#include "CRespawnArea.h"

// �����̒���
#define ROOM_LENGTH 220.0f

// ���̍��W
#define BOX_OFFSET_POS1		CVector(30.0f,0.0f,-110.0f)
#define BOX_OFFSET_POS2		CVector(30.0f,0.0f,-200.0f)

// �������̃I�t�Z�b�g���W
#define MOVE_FLOOR_OFFSET_POS	CVector(0.0f,-10.0f,-40.0f)
// �������̃X�P�[��
#define MOVE_FLOOR_SCALE		CVector(4.0f,1.0f,4.0f)
// �������̈ړ�
#define MOVE_FLOOR_MOVE			CVector(0.0f,0.0f,-100.0f)
// �������̈ړ�����
#define MOVE_FLOOR_MOVE_TIME	2.5f

// �X�C�b�`�̃I�t�Z�b�g���W
#define SWITCH_OFFSET_POS	CVector(-30.0f,0.0f,-110.0f)

// �����ǂ̃I�t�Z�b�g���W
#define MOVE_WALL_OFFSET_POS	CVector(0.0f,0.0f,-150.0f)
// �����ǂ̃X�P�[��
#define MOVE_WALL_SCALE			CVector(8.0f,4.0f,1.0f)
// �����ǂ̈ړ�
#define MOVE_WALL_MOVE			CVector(-80.0f,0.0f,0.0f)
// �����ǂ̈ړ�����
#define MOVE_WALL_MOVE_TIME		0.5f

// ���̃I�t�Z�b�g���W
#define WATER_OFFSET_POS	CVector(0.0f,-5.0f,-100.0f)
// ���̃X�P�[��
#define WATER_SCALE			CVector(50.0f,1.0f,100.0f)

// ���X�|�[���n�_
#define RESPAWN_OFFSET_POS		CVector(0.0f,0.0f,-30.0f)
// ���X�|�[���n�_�̔��a
#define RESPAWN_RADIUS	30.0f

// �R���X�g���N�^
CRoom5::CRoom5(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room5");
	// ���W��ݒ�
	Position(pos);

	// �R���C�_�[�𐶐�
	CreateCol("Room5_Ground_Col", "Room5_Wall_Col", "");

	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CRoom5::~CRoom5()
{
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CRoom5::CreateFieldObjects()
{
	// ���𐶐�
	mpBox1 = new CBox(Position() + BOX_OFFSET_POS1);
	mpBox2 = new CBox(Position() + BOX_OFFSET_POS2);

	// �������𐶐�
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	mpMoveFloor = new CMoveObj(model,
		Position() + MOVE_FLOOR_OFFSET_POS,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eGround);

	// �X�C�b�`�𐶐�
	mpSwitch = new CSwitch(Position() + SWITCH_OFFSET_POS);
	// �����ǂ𐶐�
	mpMoveWall = new CSwitchMoveWall(model,
		Position() + MOVE_WALL_OFFSET_POS,
		MOVE_WALL_SCALE,
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME);
	// ��p����I�u�W�F�N�g�ɐݒ�
	mpSwitch->SetActionObj(mpMoveWall);
	// ��p����X�C�b�`�ɐݒ�
	mpMoveWall->SetSwitchs({ mpSwitch });

	// ���𐶐�
	mpWater = new CWater(WATER_SCALE);
	// ���W��ݒ�
	mpWater->Position(Position() + WATER_OFFSET_POS);

	// ���X�|�[���n�_
	mpRespawnArea = new CRespawnArea(Position() + RESPAWN_OFFSET_POS, RESPAWN_RADIUS);
}

// �t�B�[���h�I�u�W�F�N�g���폜
void CRoom5::DeleteFieldObjects()
{
	mpBox1->Kill();
	mpBox2->Kill();
	mpMoveFloor->Kill();
	mpSwitch->DeleteSwitch();
	SAFE_DELETE(mpSwitch);
	mpMoveWall->Kill();
	mpWater->Kill();
	mpRespawnArea->Kill();
}
