#include "CRoom3.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CSwitchMoveFloor.h"
#include "CMoveObj.h"
#include "CWater.h"
#include "CRespawnArea.h"

// �����̒���
#define ROOM_LENGTH 240.0f

// ���̍��W
#define BOX_OFFSET_POS1	CVector(160.0f,60.0f,-220.0f)
#define BOX_OFFSET_POS2	CVector(80.0f,0.0f,-100.0f)

// �X�C�b�`�̍��W
#define SWITCH_OFFSET_POS1	CVector(10.0f,0.0f,-30.0f)
#define SWITCH_OFFSET_POS2	CVector(120.0f,60.0f,-60.0f)

// �X�C�b�`�œ������̃I�t�Z�b�g���W
#define SWITCH_MOVE_FLOOR_OFFSET_POS1	CVector(80.0f,50.0f,-40.0f)
#define SWITCH_MOVE_FLOOR_OFFSET_POS2	CVector(0.0f,-10.0f,-40.0f)
// �X�C�b�`�œ������̈ړ�
#define SWITCH_MOVE_FLOOR_MOVE1			CVector(0.0f,-60.0f,0.0f)
#define SWITCH_MOVE_FLOOR_MOVE2			CVector(0.0f,0.0f,-120.0f)

// �������̃I�t�Z�b�g���W
#define MOVE_FLOOR_OFFSET_POS1	CVector(120.0f,-10.0f,0.0f)
#define MOVE_FLOOR_OFFSET_POS2	CVector(160.0f,-10.0f,0.0f)
#define MOVE_FLOOR_OFFSET_POS3	CVector(160.0f,50.0f,-160.0f)
// �������̃X�P�[��
#define MOVE_FLOOR_SCALE		CVector(4.0f,1.0f,4.0f)
// �������̈ړ�
#define MOVE_FLOOR_MOVE1		CVector(-80.0f,0.0f,0.0f)
#define MOVE_FLOOR_MOVE2		CVector(0.0f,0.0f,-120.0f)
#define MOVE_FLOOR_MOVE3		CVector(0.0f,-60.0f,0.0f)
// �������̈ړ�����
#define MOVE_FLOOR_MOVE_TIME	2.5f

// ���̃I�t�Z�b�g���W
#define WATER_OFFSET_POS	CVector(80.0f,-5.0f,-120.0f)
// ���̃X�P�[��
#define WATER_SCALE			CVector(100.0f,1.0f,120.0f)

// ���X�|�[���n�_
#define RESPAWN_OFFSET_POS		CVector(0.0f,0.0f,-30.0f)
// ���X�|�[���n�_�̔��a
#define RESPAWN_RADIUS	30.0f

// �R���X�g���N�^
CRoom3::CRoom3(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room3");
	// ���W��ݒ�
	Position(pos);

	// �R���C�_�[�𐶐�
	CreateCol("Room3_Ground_Col", "Room3_Wall_Col", "");

	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CRoom3::~CRoom3()
{
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CRoom3::CreateFieldObjects()
{
	// ���𐶐�
	mpBox1 = new CBox(Position() + BOX_OFFSET_POS1);
	mpBox2 = new CBox(Position() + BOX_OFFSET_POS2);

	// �X�C�b�`�𐶐�
	mpSwitch1 = new CSwitch(Position() + SWITCH_OFFSET_POS1);
	mpSwitch2 = new CSwitch(Position() + SWITCH_OFFSET_POS2);

	// �X�C�b�`�œ������𐶐�
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	CModel* colModel = CResourceManager::Get<CModel>("MoveObject_Col");
	mpSwitchMoveFloor1 = new CSwitchMoveFloor(model, colModel,
		Position() + SWITCH_MOVE_FLOOR_OFFSET_POS1,
		MOVE_FLOOR_SCALE,
		SWITCH_MOVE_FLOOR_MOVE1,
		MOVE_FLOOR_MOVE_TIME);
	mpSwitchMoveFloor2 = new CSwitchMoveFloor(model, colModel,
		Position() + SWITCH_MOVE_FLOOR_OFFSET_POS2,
		MOVE_FLOOR_SCALE,
		SWITCH_MOVE_FLOOR_MOVE2,
		MOVE_FLOOR_MOVE_TIME);
	mpSwitch1->SetActionObj(mpSwitchMoveFloor1);
	mpSwitch2->SetActionObj(mpSwitchMoveFloor2);
	mpSwitchMoveFloor1->SetSwitchs({ mpSwitch1 });
	mpSwitchMoveFloor2->SetSwitchs({ mpSwitch2 });

	// �������𐶐�
	mpMoveFloor1 = new CMoveObj(model, colModel,
		Position() + MOVE_FLOOR_OFFSET_POS1,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE1,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eGround);
	mpMoveFloor2 = new CMoveObj(model, colModel,
		Position() + MOVE_FLOOR_OFFSET_POS2,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE2,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eGround);
	mpMoveFloor3 = new CMoveObj(model, colModel,
		Position() + MOVE_FLOOR_OFFSET_POS3,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE3,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eGround);

	// ���𐶐�
	mpWater = new CWater(WATER_SCALE);
	// ���W��ݒ�
	mpWater->Position(Position() + WATER_OFFSET_POS);

	// ���X�|�[���n�_
	mpRespawnArea = new CRespawnArea(Position() + RESPAWN_OFFSET_POS, RESPAWN_RADIUS);
}

// �t�B�[���h�I�u�W�F�N�g���폜
void CRoom3::DeleteFieldObjects()
{
}
