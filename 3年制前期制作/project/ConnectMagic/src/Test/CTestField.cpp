#include "CTestField.h"
#include "CSwitch.h"
#include "CDoor.h"
#include "CBox.h"
#include "CAirConnectObj.h"
#include "CPortal.h"
#include "CRespawnArea.h"
#include "CGrass.h"
#include "CFire.h"
#include "CBridge.h"
#include "CUpgradeItem.h"
#include "CWater.h"

// �|�[�^���̐ݒ�
#define PORTAL_POS	CVector(100.0f, 0.0f + 5.0f * PORTAL_SIZE, -100.0f)
#define PORTAL_SIZE 2.0f

// ���X�|�[���n�_�̐ݒ�
#define RESPAWN_POS_1	CVector(  0.0f,-5.0f,-180.0f)
#define RESPAWN_POS_2	CVector(-90.0f,35.0f,-430.0f)
#define RESPAWN_RADIUS	25.0f

// �����̉��̃I�t�Z�b�g���W
#define TORCH_FIRE_OFFSET_POS CVector(0.0f,9.0f,0.0f)
// �����̃I�t�Z�b�g���W
#define TORCH_OFFSET_POS_1 CVector(0.0f,0.0f,-50.0f)
#define TORCH_OFFSET_POS_2 CVector(10.0f,0.0f,-50.0f)

// ���̃I�t�Z�b�g���W
#define GRASS_OFFSET_POS_1 CVector(-50.0f,0.0f,-50.0f)
#define GRASS_OFFSET_POS_2 CVector(-100.0f,0.0f,-50.0f)

// ���̃I�t�Z�b�g���W
#define BRIDGE_OFFSET_POS_1 CVector(50.0f,0.0f,-50.0f)
#define BRIDGE_OFFSET_POS_2 CVector(50.0f,0.0f,-100.0f)
// ���̃I�t�Z�b�g��]
#define BRIDGE_OFFSET_ROT_2 CVector(0.0f,180.0f,0.0f)

// ���̃I�t�Z�b�g���W
#define BOX_OFFSET_POS CVector(0.0f,0.0f,-25.0f)

// ���̍��W
#define WATER_OFFSET_POS CVector(300.0f,1.0f,0.0f)

// �A�C�e���̍��W
#define ITEM_OFFSET_POS CVector(0.0f,10.0f,30.0f)

// �R���X�g���N�^
CTestField::CTestField()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TestField");

	// �I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
	// �R���C�_�\�𐶐�
	CreateCol("TestField", "", "");
}

// �f�X�g���N�^
CTestField::~CTestField()
{
}

// �t�B�[���h�̃I�u�W�F�N�g�𐶐�
void CTestField::CreateFieldObjects()
{
	// �����A�C�e���𐶐�
	CUpgradeItem* item = new CUpgradeItem();
	item->Position(ITEM_OFFSET_POS);

	// ���𐶐�
	CWater* water = new CWater(CVector(100.0f,1.0f,100.0f));
	water->Position(WATER_OFFSET_POS);

	// ���𐶐�
	CBox* box = new CBox();
	box->Position(BOX_OFFSET_POS);

	// ���𐶐�
	CBridge* bridge = new CBridge();
	// �ʒu�ݒ�
	bridge->Position(BRIDGE_OFFSET_POS_1);

	// ���𐶐�
	bridge = new CBridge();
	// �ʒu�ݒ�
	bridge->Position(BRIDGE_OFFSET_POS_2);
	// ��]�ݒ�
	bridge->Rotation(BRIDGE_OFFSET_ROT_2);

	// ���𐶐�
	CGrass* grass = new CGrass();
	// �ʒu����
	grass->Position(GRASS_OFFSET_POS_1);

	// ���𐶐�
	grass = new CGrass();
	// �ʒu����
	grass->Position(GRASS_OFFSET_POS_2);

	// �����𐶐�
	CFire* fire = new CFire("Torch", TORCH_FIRE_OFFSET_POS);
	// �ʒu����
	fire->Position(TORCH_OFFSET_POS_1);
	fire->SetFire(true);

	// �����𐶐�
	fire = new CFire("Torch", TORCH_FIRE_OFFSET_POS);
	// �ʒu����
	fire->Position(TORCH_OFFSET_POS_2);

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
