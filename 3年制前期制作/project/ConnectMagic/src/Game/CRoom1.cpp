#include "CRoom1.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CSwitchMoveFloor.h"
#include "CWater.h"
#include "CRespawnArea.h"

// ���̍��W
#define BOX_OFFSET_POS CVector(25.0f, 20.0f, -70.0f)
// �X�C�b�`�̍��W
#define SWITCH_OFFSET_POS CVector(25.0f, 0.0f, -35.0f)
// �����̒���
#define ROOM_LENGTH 170.0f

// �������̃I�t�Z�b�g���W
#define MOVE_FLOOR_OFFSET_POS	CVector(-60.0f, -10.0f, -50.0f)
// �������̃X�P�[��
#define MOVE_FLOOR_SCALE		CVector(4.0f, 1.0f, 8.0f)
// �������̈ړ�
#define MOVE_FLOOR_MOVE			CVector(30.0f,0.0f,0.0f)
// �������̈ړ�����	
#define MOVE_FLOOR_MOVE_TIME	1.0f

// ���̃I�t�Z�b�g���W
#define WATER_OFFSET_POS	CVector(0.0f,-5.0f,-85.0f)
// ���̃X�P�[��
#define WATER_SCALE			CVector(50.0f,1.0f,50.0f)

// ���X�|�[���n�_
#define RESPAWN_OFFSET_POS		CVector(0.0f,0.0f,-30.0f)
// ���X�|�[���n�_�̔��a
#define RESPAWN_RADIUS	30.0f

// �R���X�g���N�^
CRoom1::CRoom1(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room1");
	// ���W��ݒ�
	Position(pos);

	// �R���C�_�[�𐶐�
	CreateCol("Room1_Ground_Col", "Room1_Wall_Col", "");

	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CRoom1::~CRoom1()
{
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CRoom1::CreateFieldObjects()
{
	// ���𐶐�
	mpBox = new CBox(Position() + BOX_OFFSET_POS);

	// �X�C�b�`�𐶐�
	mpSwitch = new CSwitch(Position()+ SWITCH_OFFSET_POS);
	// �������𐶐�
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	mpMoveFloor = new CSwitchMoveFloor(model, model,
		Position() + MOVE_FLOOR_OFFSET_POS,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE,
		MOVE_FLOOR_MOVE_TIME);
	// ��p����X�C�b�`�ɐݒ�
	mpMoveFloor->SetSwitchs({ mpSwitch });
	// ��p����I�u�W�F�N�g�ɐݒ�
	mpSwitch->SetActionObj(mpMoveFloor);

	// ���𐶐�
	mpWater = new CWater(WATER_SCALE);
	// ���W��ݒ�
	mpWater->Position(Position() + WATER_OFFSET_POS);

	// ���X�|�[���n�_
	mpRespawnArea = new CRespawnArea(Position() + RESPAWN_OFFSET_POS, RESPAWN_RADIUS);
}

// �t�B�[���h�I�u�W�F�N�g���폜
void CRoom1::DeleteFieldObjects()
{
	mpBox->Kill();
	mpSwitch->DeleteSwitch();
	SAFE_DELETE(mpSwitch);
	mpMoveFloor->Kill();
	mpWater->Kill();
	mpRespawnArea->Kill();
}
