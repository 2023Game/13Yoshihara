#include "CRoom2.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CSwitchMoveWall.h"
#include "CMoveFloor.h"
#include "CWater.h"
#include "CRespawnArea.h"

// �����̒���
#define ROOM_LENGTH 220.0f

// ���̍��W
#define BOX_OFFSET_POS		CVector(30.0f,0.0f,-110.0f)
// �X�C�b�`�̍��W
#define SWITCH_OFFSET_POS	CVector(-30.0f,0.0f,-110.0f)

// �����ǂ̃I�t�Z�b�g���W
#define MOVE_WALL_OFFSET_POS	CVector(-5.0f,0.0f,-150.0f)
// �����ǂ̃X�P�[��
#define MOVE_WALL_SCALE			CVector(9.0f,4.0f,1.0f)
// �����ǂ̈ړ�
#define MOVE_WALL_MOVE			CVector(-65.0f,0.0f,0.0f)
// �����ǂ̈ړ�����
#define MOVE_WALL_MOVE_TIME		1.0f

// �������̃I�t�Z�b�g���W
#define MOVE_FLOOR_OFFSET_POS	CVector(0.0f,-10.0f,-90.0f)
// �������̃X�P�[��
#define MOVE_FLOOR_SCALE		CVector(4.0f,1.0f,4.0f)
// �������̈ړ�
#define MOVE_FLOOR_MOVE			CVector(0.0f,0.0f,50.0f)
// �������̈ړ�����
#define MOVE_FLOOR_MOVE_TIME	10.0f

// ���̃I�t�Z�b�g���W
#define WATER_OFFSET_POS	CVector(0.0f,-5.0f,-85.0f)
// ���̃X�P�[��
#define WATER_SCALE			CVector(50.0f,1.0f,100.0f)

// ���X�|�[���n�_
#define RESPAWN_OFFSET_POS		CVector(0.0f,0.0f,-30.0f)
// ���X�|�[���n�_�̔��a
#define RESPAWN_RADIUS	30.0f

// �R���X�g���N�^
CRoom2::CRoom2(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room2");
	// ���W��ݒ�
	Position(pos);

	// �R���C�_�[�𐶐�
	CreateCol("Room2_Ground_Col", "Room2_Wall_Col", "");

	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CRoom2::~CRoom2()
{
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CRoom2::CreateFieldObjects()
{
	// ���𐶐�
	mpBox = new CBox(Position() + BOX_OFFSET_POS);

	// �X�C�b�`�𐶐�
	mpSwitch = new CSwitch(Position() + SWITCH_OFFSET_POS);
	// �����ǂ𐶐�
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	mpMoveWall = new CSwitchMoveWall(model,
		Position() + MOVE_WALL_OFFSET_POS,
		MOVE_WALL_SCALE,
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME);
	// ��p����X�C�b�`�ɐݒ�
	mpMoveWall->SetSwitchs({ mpSwitch });
	// ��p����I�u�W�F�N�g�ɐݒ�
	mpSwitch->SetActionObj(mpMoveWall);

	// �������𐶐�
	mpMoveFloor = new CMoveFloor(model,
		Position() + MOVE_FLOOR_OFFSET_POS,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE,
		MOVE_FLOOR_MOVE_TIME);

	// ���𐶐�
	mpWater = new CWater(WATER_SCALE);
	// ���W��ݒ�
	mpWater->Position(Position() + WATER_OFFSET_POS);

	// ���X�|�[���n�_
	mpRespawnArea = new CRespawnArea(Position() + RESPAWN_OFFSET_POS, RESPAWN_RADIUS);
}

// �t�B�[���h�I�u�W�F�N�g���폜
void CRoom2::DeleteFieldObjects()
{
	mpBox->Kill();
	mpSwitch->DeleteSwitch();
	SAFE_DELETE(mpSwitch);
	mpMoveWall->Kill();
	mpMoveFloor->Kill();
	mpWater->Kill();
	mpRespawnArea->Kill();
}
