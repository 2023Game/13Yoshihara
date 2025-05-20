#include "CTutorialMap.h"
#include "CSwitch.h"
#include "CDoor.h"
#include "CBox.h"
#include "CAirConnectObj.h"
#include "CPortal.h"
#include "CRespawnArea.h"

// �X�C�b�`�̍��W
#define SWITCH_POS	CVector(20.0f,0.0f,20.0f)
// �h�A�̍��W
#define DOOR_POS	CVector(8.5f,0.0f,-42.5f)
// ���̍��W
#define BOX_POS		CVector(-25.0f,0.0f,-25.0f)

// �󒆂̐ڑ��I�u�W�F�N�g�̍��W
#define AIR_POS_1	CVector( -2.5f, 30.0f, -90.0f)
#define AIR_POS_2	CVector(-42.5f, 50.0f,-250.0f)
#define AIR_POS_3	CVector(-62.5f, 90.0f,-360.0f)
#define AIR_POS_4	CVector(-22.5f,100.0f,-420.0f)
#define AIR_POS_5	CVector( 82.5f,130.0f,-440.0f)
#define AIR_POS_6	CVector(182.5f,150.0f,-440.0f)
#define AIR_POS_7	CVector(272.5f,180.0f,-480.0f)
#define AIR_POS_8	CVector(262.5f,210.0f,-540.0f)

// �󒆂̐ڑ��I�u�W�F�N�g�̉�]�l
#define AIR_ROT_2	CVector(0.0f,- 25.0f,0.0f)
#define AIR_ROT_4	CVector(0.0f,- 90.0f,0.0f)
#define AIR_ROT_5	CVector(0.0f,  90.0f,0.0f)
#define AIR_ROT_6	CVector(0.0f,-115.0f,0.0f)
#define AIR_ROT_7	CVector(0.0f,-205.0f,0.0f)
#define AIR_ROT_8	CVector(0.0f,  90.0f,0.0f)

// �|�[�^���̐ݒ�
#define PORTAL_POS	CVector(90.0f, 220.0f + 5.0f * PORTAL_SIZE, -700.0f)
#define PORTAL_SIZE 2.0f

// ���X�|�[���n�_�̐ݒ�
#define RESPAWN_POS_1	CVector(  0.0f,-5.0f,-180.0f)
#define RESPAWN_POS_2	CVector(-90.0f,35.0f,-430.0f)
#define RESPAWN_RADIUS	25.0f

// �R���X�g���N�^
CTutorialMap::CTutorialMap()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TutorialMap");

	// �I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
	// �R���C�_�\�𐶐�
	CreateCol("TutorialMap_Ground_Col", "TutorialMap_Wall_Col", "");
}

// �f�X�g���N�^
CTutorialMap::~CTutorialMap()
{
}

// �t�B�[���h�̃I�u�W�F�N�g�𐶐�
void CTutorialMap::CreateFieldObjects()
{
	// �h�A�𐶐�
	CDoor* door = new CDoor(CVector::zero);
	// �h�A�̈ʒu����
	door->Position(DOOR_POS);

	// �X�C�b�`�𐶐�
	CSwitch* doorSwitch = new CSwitch(SWITCH_POS);
	// �h�A���X�C�b�`�̍�p�I�u�W�F�N�g�ɐݒ�
	doorSwitch->SetActionObj(door);

	// ���𐶐�
	CBox* box = new CBox();
	// ���̈ʒu����
	box->Position(BOX_POS);

	// �󒆂̐ڑ��I�u�W�F�N�g�𐶐�
	CAirConnectObj* airConnectObj = new CAirConnectObj();
	// �d�͂��|���Ȃ�
	airConnectObj->SetGravity(false);
	// �ʒu����
	airConnectObj->Position(AIR_POS_1);

	// �󒆂̐ڑ��I�u�W�F�N�g�𐶐�
	airConnectObj = new CAirConnectObj();
	// �d�͂��|���Ȃ�
	airConnectObj->SetGravity(false);
	// �ʒu��]����
	airConnectObj->Position(AIR_POS_2);
	airConnectObj->Rotation(AIR_ROT_2);

	// �󒆂̐ڑ��I�u�W�F�N�g�𐶐�
	airConnectObj = new CAirConnectObj();
	// �d�͂��|���Ȃ�
	airConnectObj->SetGravity(false);
	// �ʒu����
	airConnectObj->Position(AIR_POS_3);

	// �󒆂̐ڑ��I�u�W�F�N�g�𐶐�
	airConnectObj = new CAirConnectObj();
	// �d�͂��|���Ȃ�
	airConnectObj->SetGravity(false);
	// �ʒu��]����
	airConnectObj->Position(AIR_POS_4);
	airConnectObj->Rotation(AIR_ROT_4);

	// �󒆂̐ڑ��I�u�W�F�N�g�𐶐�
	airConnectObj = new CAirConnectObj();
	// �d�͂��|���Ȃ�
	airConnectObj->SetGravity(false);
	// �ʒu��]����
	airConnectObj->Position(AIR_POS_5);
	airConnectObj->Rotation(AIR_ROT_5);

	// �󒆂̐ڑ��I�u�W�F�N�g�𐶐�
	airConnectObj = new CAirConnectObj();
	// �d�͂��|���Ȃ�
	airConnectObj->SetGravity(false);
	// �ʒu��]����
	airConnectObj->Position(AIR_POS_6);
	airConnectObj->Rotation(AIR_ROT_6);

	// �󒆂̐ڑ��I�u�W�F�N�g�𐶐�
	airConnectObj = new CAirConnectObj();
	// �d�͂��|���Ȃ�
	airConnectObj->SetGravity(false);
	// �ʒu��]����
	airConnectObj->Position(AIR_POS_7);
	airConnectObj->Rotation(AIR_ROT_7);

	// �󒆂̐ڑ��I�u�W�F�N�g�𐶐�
	airConnectObj = new CAirConnectObj();
	// �d�͂��|���Ȃ�
	airConnectObj->SetGravity(false);
	// �ʒu��]����
	airConnectObj->Position(AIR_POS_8);
	airConnectObj->Rotation(AIR_ROT_8);

	// �|�[�^���𐶐�
	CPortal* portal = new CPortal();
	// �ʒu����
	portal->Position(PORTAL_POS);
	// �T�C�Y����
	portal->SetSize(portal->GetSize() * PORTAL_SIZE);

	// ���X�|�[���n�_�𐶐�
	new CRespawnArea(RESPAWN_POS_1, RESPAWN_RADIUS);
	new CRespawnArea(RESPAWN_POS_2, RESPAWN_RADIUS);
}