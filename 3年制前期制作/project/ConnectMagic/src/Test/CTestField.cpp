#include "CTestField.h"
#include "CSwitch.h"
#include "CDoor.h"
#include "CBox.h"
#include "CAirConnectObj.h"

// �X�C�b�`�̍��W
#define SWITCH_POS	CVector(20.0f,0.0f,20.0f)
// �h�A�̍��W
#define DOOR_POS	CVector(8.5f,0.0f,-42.5f)
// ���̍��W
#define BOX_POS		CVector(-25.0f,0.0f,-25.0f)

// �󒆂̐ڑ��I�u�W�F�N�g�̍��W
#define AIR_POS_1	CVector(-2.5f,30.0f,-90.0f)

// �R���X�g���N�^
CTestField::CTestField()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TestField");

	// �I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CTestField::~CTestField()
{
}

// �t�B�[���h�̃I�u�W�F�N�g�𐶐�
void CTestField::CreateFieldObjects()
{
	// �h�A�𐶐�
	mpDoor = new CDoor(CVector::zero);
	// �h�A�̈ʒu����
	mpDoor->Position(DOOR_POS);

	// �X�C�b�`�𐶐�
	mpSwitch = new CSwitch(SWITCH_POS);
	// �h�A���X�C�b�`�̍�p�I�u�W�F�N�g�ɐݒ�
	mpSwitch->SetActionObj(mpDoor);

	// ���𐶐�
	mpBox = new CBox();
	// ���̈ʒu����
	mpBox->Position(BOX_POS);

	// �󒆂̐ڑ��I�u�W�F�N�g�𐶐�
	CAirConnectObj* airConnectObj = new CAirConnectObj();
	// �d�͂��|���Ȃ�
	airConnectObj->SetGravity(false);
	// �ʒu����
	airConnectObj->Position(AIR_POS_1);
	// ���X�g�ɒǉ�
	mAirConnectObjs.push_back(airConnectObj);
}

// �t�B�[���h�̃R���C�_�\�𐶐�
void CTestField::CreateCol()
{
	mpGroundColliderMesh = new CColliderMesh(this, ELayer::eGround, CResourceManager::Get<CModel>("TutorialMap_Ground_Col"));
	mpWallColliderMesh = new CColliderMesh(this, ELayer::eWall, CResourceManager::Get<CModel>("TutorialMap_Wall_Col"));
}
