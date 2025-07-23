#include "CRoom1.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CMoveFloor.h"

// ���̍��W
#define BOX_POS CVector(25.0f, 30.0f, 20.0f)
// �X�C�b�`�̍��W
#define SWITCH_POS CVector(25.0f, 0.0f, 5.0f)
// �����̒���
#define ROOM_LENGTH 80.0f

// �R���X�g���N�^
CRoom1::CRoom1(CVector pos)
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
	CBox* box = new CBox();
	// ���W��ݒ�
	box->Position(Position() + BOX_POS);

	// �X�C�b�`�𐶐�
	CSwitch* switchObj = new CSwitch(Position()+ SWITCH_POS);

	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	CMoveFloor* moveFloor = new CMoveFloor(model, Position() + CVector(-60.0f, -10.0f, -90.0f), CVector(4.0f, 1.0f, 8.0f), CVector(30.0f,0.0f,0.0f), 5.0f);
}
