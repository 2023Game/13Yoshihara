#include "CConnectRoom.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CSwitchMoveWall.h"

// ���̃I�t�Z�b�g���W
#define BOX_OFFSET_POS CVector(25.0f,0.0f,-40.0f)
// �X�C�b�`�̃I�t�Z�b�g���W
#define SWITCH_OFFSET_POS_NEXT	CVector(-25.0f,0.0f,-40.0f)
#define SWITCH_OFFSET_POS_PRE	CVector(25.0f,0.0f,-40.0f)
// �����̒���
#define ROOM_LENGTH 80.0f

// �����ǂ̃I�t�Z�b�g���W
#define MOVE_WALL_OFFSET_POS_NEXT	CVector(0.0f,0.0f,-70.0f)
#define MOVE_WALL_OFFSET_POS_PRE	CVector(0.0f,0.0f,0.0f)
// �����ǂ̃X�P�[��
#define MOVE_WALL_SCALE CVector(2.0f,4.0f,1.0f)
// �����ǂ̈ړ�
#define MOVE_WALL_MOVE	CVector(0.0f,40.0f,0.0f)
// �����ǂ̈ړ�����
#define MOVE_WALL_MOVE_TIME 0.5f

// �R���X�g���N�^
CConnectRoom::CConnectRoom(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("ConnectRoom");
	// ���W��ݒ�
	Position(pos);

	// �R���C�_�[�𐶐�
	CreateCol("ConnectRoom_Ground_Col", "ConnectRoom_Wall_Col", "");

	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CConnectRoom::~CConnectRoom()
{
}

// ���̕�����ݒ�
void CConnectRoom::SetNextRoom(CRoomBase* room)
{
	mpNextRoom = room;
}

// �O�̕�����ݒ�
void CConnectRoom::SetPreRoom(CRoomBase* room)
{
	mpPreRoom = room;
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CConnectRoom::CreateFieldObjects()
{
	// ���𐶐�
	mpBox = new CBox(Position() + BOX_OFFSET_POS);

	// �X�C�b�`�𐶐�
	mpNextSwitch = new CSwitch(Position() + SWITCH_OFFSET_POS_NEXT);
	mpPreSwitch = new CSwitch(Position() + SWITCH_OFFSET_POS_PRE);
	// �����ǂ𐶐�
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	mpNextWall = new CSwitchMoveWall(model,model,
		Position() + MOVE_WALL_OFFSET_POS_NEXT, 
		MOVE_WALL_SCALE, 
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME);
	mpPreWall = new CSwitchMoveWall(model,model,
		Position() + MOVE_WALL_OFFSET_POS_PRE,
		MOVE_WALL_SCALE,
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME);
	// ��p����X�C�b�`�ɐݒ�
	mpNextWall->SetSwitchs({ mpNextSwitch });
	mpPreWall->SetSwitchs({ mpPreSwitch });
	// ��p����I�u�W�F�N�g�ɐݒ�
	mpNextSwitch->SetActionObj(mpNextWall);
	mpPreSwitch->SetActionObj(mpPreWall);
}

// �t�B�[���h�I�u�W�F�N�g���폜
void CConnectRoom::DeleteFieldObjects()
{
	mpBox->Kill();
	mpNextSwitch->DeleteSwitch();
	SAFE_DELETE(mpNextSwitch);
	mpPreSwitch->DeleteSwitch();
	SAFE_DELETE(mpPreSwitch);
	mpNextWall->Kill();
	mpPreWall->Kill();
	mpNextRoom = nullptr;
	mpPreRoom = nullptr;
}
