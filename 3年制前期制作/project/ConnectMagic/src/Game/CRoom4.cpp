#include "CRoom4.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CMoveObj.h"
#include "CWater.h"
#include "CRespawnArea.h"
#include "CSaveManager.h"

// �����̒���
#define ROOM_LENGTH 240.0f

// ���̍��W
#define BOX_OFFSET_POS		CVector(30.0f,0.0f,-20.0f)

// �������̃I�t�Z�b�g���W
#define MOVE_FLOOR_OFFSET_POS	CVector(0.0f,-10.0f,-40.0f)
// �������̃X�P�[��
#define MOVE_FLOOR_SCALE		CVector(4.0f,1.0f,4.0f)
// �������̈ړ�
#define MOVE_FLOOR_MOVE			CVector(0.0f,0.0f,-120.0f)
// �������̈ړ�����
#define MOVE_FLOOR_MOVE_TIME	2.0f

// �����ǂ̃I�t�Z�b�g���W
#define MOVE_WALL_OFFSET_POS	CVector(-25.0f,0.0f,-120.0f)
// �����ǂ̃X�P�[��
#define MOVE_WALL_SCALE			CVector(3.0f,4.0f,1.0f)
// �����ǂ̈ړ�
#define MOVE_WALL_MOVE			CVector(50.0f,0.0f,0.0f)
// �����ǂ̈ړ�����
#define MOVE_WALL_MOVE_TIME		2.5f

// ���̃I�t�Z�b�g���W
#define WATER_OFFSET_POS	CVector(0.0f,-5.0f,-100.0f)
// ���̃X�P�[��
#define WATER_SCALE			CVector(50.0f,1.0f,100.0f)

// ���X�|�[���n�_
#define RESPAWN_OFFSET_POS		CVector(0.0f,0.0f,-30.0f)
// ���X�|�[���n�_�̔��a
#define RESPAWN_RADIUS	30.0f

// �R���X�g���N�^
CRoom4::CRoom4(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room4");
	// ���W��ݒ�
	Position(pos);

	// �R���C�_�[�𐶐�
	CreateCol("Room4_Ground_Col", "Room4_Wall_Col", "");

	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CRoom4::~CRoom4()
{
}

// �����̗L��������ݒ�
void CRoom4::SetEnableRoom(bool enable)
{
	CRoomBase::SetEnableRoom(enable);

	// �L�����̂�
	if (enable) {
		// ���̃t���O��ύX
		mpBox->SetEnable(enable);
		// �ۑ��Ǘ��N���X
		CSaveManager* saveMgr = CSaveManager::Instance();
		// ����ǉ�
		saveMgr->AddBox(mpBox);
		// �ړ�����ǉ�
		saveMgr->AddMoveObj(mpMoveFloor);
	}
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CRoom4::CreateFieldObjects()
{
	// ���𐶐�
	mpBox = new CBox(Position() + BOX_OFFSET_POS);
	// �ŏ��͖���
	mpBox->SetEnable(false);

	// �������𐶐�
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	CModel* colModel = CResourceManager::Get<CModel>("MoveObject_Col");
	mpMoveFloor = new CMoveObj(model, colModel,
		Position() + MOVE_FLOOR_OFFSET_POS,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eGround);
	// ���X�g�ɒǉ�
	mObjs.push_back(mpMoveFloor);

	// �����ǂ𐶐�
	mpMoveWall= new CMoveObj(model, colModel,
		Position() + MOVE_WALL_OFFSET_POS,
		MOVE_WALL_SCALE,
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME,
		ELayer::eWall);
	// ���X�g�ɒǉ�
	mObjs.push_back(mpMoveWall);

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
