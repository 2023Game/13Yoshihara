#include "CTestField.h"
#include "CSwitch.h"
#include "CDoor.h"
#include "CBox.h"
#include "CAirConnectObj.h"
#include "CPortal.h"
#include "CRespawnArea.h"
#include "CGrass.h"
#include "CTorch.h"
#include "CCampFire.h"
#include "CBridge.h"
#include "CUpgradeItem.h"
#include "CWater.h"
#include "CPortalFragment.h"
#include "CFragmentMergeArea.h"
#include "CDownWall.h"

// �|�[�^���̐ݒ�
#define PORTAL_POS	CVector(100.0f, 10.0f, -100.0f)

// ���X�|�[���n�_�̐ݒ�
#define RESPAWN_POS_1	CVector(  0.0f,-5.0f,-180.0f)
#define RESPAWN_POS_2	CVector(-90.0f,35.0f,-430.0f)
#define RESPAWN_RADIUS	25.0f

// �����̍��W
#define TORCH_POS_1 CVector(0.0f,0.0f,-50.0f)
#define TORCH_POS_2 CVector(10.0f,0.0f,-50.0f)

// ���̍��W
#define GRASS_POS_1 CVector(-50.0f,0.0f,-50.0f)
#define GRASS_POS_2 CVector(-100.0f,0.0f,-50.0f)

// �c�^�̉��̃I�t�Z�b�g���W
#define VINE_FIRE_OFFSET_POS CVector(0.0f,VINE_HEIGHT/2.0f,0.0f)
// �c�^�̉��̑傫��
#define VINE_FIRE_SCALE FIRE_SCALE * 15.0f

// ���̍��W
#define BRIDGE_POS_1 CVector(50.0f,0.0f,-50.0f)
#define BRIDGE_POS_2 CVector(50.0f,0.0f,-100.0f)
// ���̉�]
#define BRIDGE_ROT_2 CVector(0.0f,180.0f,0.0f)

// ���̍��W
#define BOX_POS CVector(0.0f,0.0f,-25.0f)

// ���̍��W
#define WATER_POS CVector(300.0f,1.0f,0.0f)

// �A�C�e���̍��W
#define ITEM_POS CVector(0.0f,5.0f,30.0f)

// �L�����v�t�@�C���[�̍��W
#define CAMPFIRE_POS CVector(0.0f,0.0f,-70.0f)

// �|�[�^���̌��Ђ̐�
#define PORTAL_FRAGMENT_NUM 4

// ������ǂ̍��W
#define DOWN_WALL_POS CVector(150.0f,0.0f,50.0f)
// �X�C�b�`�̍��W
#define SWITCH_POS CVector(0.0f,0.0f,50.0f)

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
	// ���Ђ̍����ꏊ
	CFragmentMergeArea* mergeArea = new CFragmentMergeArea(PORTAL_FRAGMENT_NUM);
	mergeArea->Position(0.0f, 0.0f, 100.0f);

	// �����A�C�e���𐶐�
	CUpgradeItem* item = new CUpgradeItem();
	item->Position(ITEM_POS);

	for (int i = 0; i < 10; i++)
	{
		item = new CUpgradeItem();
		item->Position(10.0f + 10.0f * i, 5.0f, 0.0f);
	}

	// ���𐶐�
	CWater* water = new CWater(CVector(100.0f,1.0f,100.0f));
	water->Position(WATER_POS);

	// ���𐶐�
	CBox* box = new CBox();
	box->Position(BOX_POS);

	// ���𐶐�
	CBridge* bridge = new CBridge();
	// �ʒu�ݒ�
	bridge->Position(BRIDGE_POS_1);

	// ���𐶐�
	bridge = new CBridge();
	// �ʒu�ݒ�
	bridge->Position(BRIDGE_POS_2);
	// ��]�ݒ�
	bridge->Rotation(BRIDGE_ROT_2);

	// ���𐶐�
	CGrass* grass = new CGrass();
	// �ʒu����
	grass->Position(GRASS_POS_1);
	grass->SetDropItem(mergeArea->GetFragment(3));

	// ���𐶐�
	grass = new CGrass(EGrassType::eVine, VINE_FIRE_OFFSET_POS, VINE_FIRE_SCALE);
	// �ʒu����
	grass->Position(GRASS_POS_2);

	// �����𐶐�
	CTorch* torch = new CTorch();
	// �ʒu����
	torch->Position(TORCH_POS_1);
	torch->SetFire(true);

	// �����𐶐�
	torch = new CTorch();
	// �ʒu����
	torch->Position(TORCH_POS_2);

	// �L�����v�t�@�C���[
	CCampFire* campFire = new CCampFire();
	campFire->Position(CAMPFIRE_POS);
	campFire->SetDropItem(mergeArea->GetFragment(0));

	campFire = new CCampFire();
	campFire->Position(0.0f,0.0f,-80.0f);
	campFire->SetDropItem(mergeArea->GetFragment(1));

	campFire = new CCampFire();
	campFire->Position(0.0f,0.0f,-100.0f);
	campFire->SetDropItem(mergeArea->GetFragment(2));

	// �|�[�^���𐶐�
	CPortal* portal = new CPortal();
	// �ʒu����
	portal->Position(PORTAL_POS);

	// ���X�|�[���n�_�𐶐�
	new CRespawnArea(RESPAWN_POS_1, RESPAWN_RADIUS);
	new CRespawnArea(RESPAWN_POS_2, RESPAWN_RADIUS);

	// ������ǂ𐶐�
	CDownWall* downWall = new CDownWall(DOWN_WALL_POS);
	
	// �X�C�b�`�𐶐�
	CSwitch* switchObj = new CSwitch(SWITCH_POS);
	// ��p����I�u�W�F�N�g�ɉ�����ǂ�ݒ�
	switchObj->SetActionObj(downWall);

	CBox* bigBox = new CBox(9.0f);
	bigBox->Position(0.0f, 0.0f, 100.0f);
}
