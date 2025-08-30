#include "CRoom3.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CSwitchMoveFloor.h"
#include "CMoveObj.h"
#include "CWater.h"
#include "CRespawnArea.h"
#include "CSaveManager.h"

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

// �����̗L��������ݒ�
void CRoom3::SetEnableRoom(bool enable)
{
	CRoomBase::SetEnableRoom(enable);
	mpSwitch1->SetEnableSwitch(enable);
	mpSwitch2->SetEnableSwitch(enable);

	// �ۑ��Ǘ��N���X
	CSaveManager* saveMgr = CSaveManager::Instance();
	// �L�����̂�
	if (enable) {
		// ���̃t���O��ύX
		mpBox1->SetEnable(enable);
		mpBox2->SetEnable(enable);
		// ����ǉ�
		saveMgr->AddBox(mpBox1);
		saveMgr->AddBox(mpBox2);
		// �ړ�����ǉ�
		saveMgr->AddMoveFloor(mpSwitchMoveFloor1);
		saveMgr->AddMoveFloor(mpSwitchMoveFloor2);
		saveMgr->AddMoveObj(mpMoveFloor1);
		saveMgr->AddMoveObj(mpMoveFloor2);
		saveMgr->AddMoveObj(mpMoveFloor3);
	}
	// ������
	else
	{
		// �����X�C�b�`�ɒ���t���Ă��Ȃ��Ȃ�
		if (!mpBox1->GetIsAttach())
		{
			// ���̃t���O��ύX
			mpBox1->SetEnable(enable);
		}
		// �����X�C�b�`�ɒ���t���Ă��Ȃ��Ȃ�
		if (!mpBox2->GetIsAttach())
		{
			// ���̃t���O��ύX
			mpBox2->SetEnable(enable);
		}
		// �����폜
		saveMgr->DeleteBox(mpBox1);
		saveMgr->DeleteBox(mpBox2);
		// �ړ������폜
		saveMgr->DeleteMoveFloor(mpSwitchMoveFloor1);
		saveMgr->DeleteMoveFloor(mpSwitchMoveFloor2);
		saveMgr->DeleteMoveObj(mpMoveFloor1);
		saveMgr->DeleteMoveObj(mpMoveFloor2);
		saveMgr->DeleteMoveObj(mpMoveFloor3);
	}
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CRoom3::CreateFieldObjects()
{
	// ���𐶐�
	mpBox1 = new CBox(Position() + BOX_OFFSET_POS1);
	mpBox2 = new CBox(Position() + BOX_OFFSET_POS2);
	// �ŏ��͖���
	mpBox1->SetEnable(false);
	mpBox2->SetEnable(false);

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
	// ���X�g�ɒǉ�
	mObjs.push_back(mpSwitchMoveFloor1);
	mObjs.push_back(mpSwitchMoveFloor2);
	// �X�C�b�`��ݒ�
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
	// ���X�g�ɒǉ�
	mObjs.push_back(mpMoveFloor1);
	mObjs.push_back(mpMoveFloor2);
	mObjs.push_back(mpMoveFloor3);

	// ���𐶐�
	mpWater = new CWater(WATER_SCALE);
	// ���W��ݒ�
	mpWater->Position(Position() + WATER_OFFSET_POS);
	// ���X�g�ɒǉ�
	mObjs.push_back(mpWater);

	// ���X�|�[���n�_
	mpRespawnArea = new CRespawnArea(Position() + RESPAWN_OFFSET_POS, RESPAWN_RADIUS);
	// ���X�g�ɒǉ�
	mObjs.push_back(mpRespawnArea);
}