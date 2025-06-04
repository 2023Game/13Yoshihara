#include "CMap1.h"
#include "CBridge.h"
#include "CWater.h"
#include "CFragmentMergeArea.h"
#include "CPortalFragment.h"
#include "CCampFire.h"
#include "CTorch.h"
#include "CGrass.h"
#include "CUpgradeItem.h"

// ���̍��W
#define BRIDGE_POS_1 CVector(-325.0f,0.0f,252.5f)
#define BRIDGE_POS_2 CVector(-325.0f,0.0f,127.5f)

// ���̉�]
#define BRIDGE_ROT_2 CVector(0.0f,180.0f,0.0f)

// ���̃f�t�H���g�p�x
#define BRIDGE_ANGLE_1 90.0f

// ���ʂ̍��W
#define WATER_POS CVector(-250.0f,-10.0f,50.0f)
// ���ʂ̃X�P�[��
#define WATER_SCALE CVector(500.0f,1.0f,300.0f)

// �|�[�^���̌��Ђ̐�
#define PORTAL_FRAGMENT_NUM 4
// �����n�̍��W
#define MERGE_AREA_POS CVector(-420.0f,10.0f,-200.0f)

// �L�����v�t�@�C���[�̍��W
#define CAMPFIRE_POS CVector(-260.0f,0.0f,-290.0f)

// �����̍��W
#define TORCH_POS_1 CVector(-240.0f,0.0f,-280.0f)

// ���̍��W
#define GRASS_POS_1 CVector(-230.0f,0.0f,-280.0f)
#define GRASS_POS_2 CVector(-230.0f,0.0f,-280.0f)
#define GRASS_POS_3 CVector(-230.0f,0.0f,-280.0f)
// �c�^�̍��W
#define VINE_POS_1 CVector(-15.0f,0.0f,170.0f)
// �c�^�̉��̃I�t�Z�b�g���W
#define VINE_FIRE_OFFSET_POS CVector(0.0f,VINE_HEIGHT/2.0f,0.0f)
// �c�^�̉��̑傫��
#define VINE_FIRE_SCALE FIRE_SCALE * 15.0f

// �ŏ��ɔz�u����A�C�e���̍��W
#define FIRST_ITEM_POS	15.0f
// �ŏ��ɔz�u����A�C�e���̐�
#define FIRST_ITEM_NUM	10

// �R���X�g���N�^
CMap1::CMap1()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("Map1");

	// �R���C�_�[�𐶐�
	CreateCol("Map1_Ground_Col", "Map1_Wall_Col", "");

	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CMap1::~CMap1()
{
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CMap1::CreateFieldObjects()
{
	// ���Ђ̍����ꏊ
	CFragmentMergeArea* mergeArea = new CFragmentMergeArea(PORTAL_FRAGMENT_NUM);
	// ���W��ݒ�
	mergeArea->Position(MERGE_AREA_POS);

	// ���ʂ𐶐�
	CWater* water = new CWater(WATER_SCALE);
	// ���W�ݒ�
	water->Position(WATER_POS);

	// ���𐶐�
	CBridge* bridge = new CBridge();
	// �ʒu�ݒ�
	bridge->Position(BRIDGE_POS_1);
	// �����p�x��ݒ�
	bridge->SetAngle(BRIDGE_ANGLE_1);

	// ���𐶐�
	bridge = new CBridge();
	// �ʒu�ݒ�
	bridge->Position(BRIDGE_POS_2);
	// ��]�ݒ�
	bridge->Rotate(BRIDGE_ROT_2);

	// �L�����v�t�@�C���[
	CCampFire* campFire = new CCampFire();
	// ���W�ݒ�
	campFire->Position(CAMPFIRE_POS);
	// ���Ƃ��A�C�e����ݒ�
	campFire->SetDropItem(mergeArea->GetFragment(0));	
	
	// �����𐶐�
	CTorch* torch = new CTorch();
	// �ʒu����
	torch->Position(TORCH_POS_1);
	torch->SetFire(true);

	// ���𐶐�
	CGrass* grass = new CGrass();
	// �ʒu����
	grass->Position(GRASS_POS_1);
	grass->SetDropItem(mergeArea->GetFragment(1));
	// ���𐶐�
	grass = new CGrass();
	// �ʒu����
	grass->Position(GRASS_POS_2);
	grass->SetDropItem(mergeArea->GetFragment(2));
	// ���𐶐�
	grass = new CGrass();
	// �ʒu����
	grass->Position(GRASS_POS_3);
	grass->SetDropItem(mergeArea->GetFragment(3));

	// �c�^�𐶐�
	grass = new CGrass(EGrassType::eVine, VINE_FIRE_OFFSET_POS, VINE_FIRE_SCALE);
	// �ʒu����
	grass->Position(VINE_POS_1);

	// �A�C�e���𐶐�
	CUpgradeItem* item;
	for (int i = 0; i < FIRST_ITEM_NUM; i++)
	{
		item = new CUpgradeItem();
		// ���W��ݒ�
		item->Position(0.0f, 0.0f, FIRST_ITEM_POS * i);
	}
}

// �o�H�T���p�̃m�[�h�𐶐�
void CMap1::CreateNavNodes()
{
}
