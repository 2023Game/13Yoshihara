#include "CMap3.h"
#include "CBridge.h"
#include "CWater.h"
#include "CFragmentMergeArea.h"
#include "CPortalFragment.h"
#include "CCampFire.h"
#include "CTorch.h"
#include "CGrass.h"
#include "CUpgradeItem.h"
#include "Maths.h"

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
#define TORCH_POS_1 CVector(-50.0f,0.0f,150.0f)
#define TORCH_POS_2	CVector(-65.0f,0.0f,245.0f)
#define TORCH_POS_3 CVector(-145.0f,0.0f,335.0f)
#define TORCH_POS_4 CVector(-230.0f,0.0f,250.0f)
#define TORCH_POS_5 CVector(-250.0f,0.0f,120.0f)
#define TORCH_POS_6 CVector(-275.0f,0.0f,-90.0f)
#define TORCH_POS_7 CVector(-250.0f,0.0f,40.0f)
#define TORCH_POS_8 CVector(-375.0f,0.0f,80.0f)
#define TORCH_POS_9 CVector(-465.0f,0.0f,80.0f)
#define TORCH_POS_10 CVector(-465.0f,0.0f,-40.0f)
#define TORCH_POS_11 CVector(-70.0f,0.0f,-125.0f)
#define TORCH_POS_12 CVector(-170.0f,0.0f,-10.0f)
#define TORCH_POS_13 CVector(-380.0f,0.0f,-135.0f)
#define TORCH_POS_14 CVector(-180.0f,0.0f,-210.0f)

// ���̍��W
#define GRASS_POS_1 CVector(-370.0f,0.0f,-140.0f)
#define GRASS_POS_2 CVector(-200.0f,0.0f,-160.0f)
#define GRASS_POS_3 CVector(-5.0f,0.0f,-180.0f)
#define GRASS_POS_4 CVector(-460.0f,0.0f,25.0f)
#define GRASS_POS_5 CVector(20.0f,0.0f,-150.0f)
#define GRASS_POS_6 CVector(-230.0f,0.0f,-280.0f)

// �c�^�̍��W
#define VINE_POS_1	CVector(-15.0f,0.0f,170.0f)
#define VINE_POS_2	CVector(-135.0f,0.0f,295.0f)
// �c�^�̉�]
#define VINE_ROT_2	CVector(0.0f,70.0f,0.0f)
// �c�^�̉��̃I�t�Z�b�g���W
#define VINE_FIRE_OFFSET_POS CVector(0.0f,VINE_HEIGHT/2.0f,0.0f)
// �c�^�̉��̑傫��
#define VINE_FIRE_SCALE FIRE_SCALE * 15.0f

// �ŏ��ɔz�u����A�C�e���̍��W
#define ITEM_POS_1	CVector(0.0f,0.0f,50.0f)
#define ITEM_POS_2	CVector(0.0f,0.0f,270.0f)
#define ITEM_POS_3	CVector(-270.0f,0.0f,90.0f)

// �R���X�g���N�^
CMap3::CMap3()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("Map3");

	// �R���C�_�[�𐶐�
	CreateCol("Map3_Ground_Col", "Map3_Wall_Col", "");

	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CMap3::~CMap3()
{
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CMap3::CreateFieldObjects()
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

	// �����̍��W�̃��X�g
	CVector torchPosList[] = {
		TORCH_POS_1,
		TORCH_POS_2,
		TORCH_POS_3,
		TORCH_POS_4,
		TORCH_POS_5,
		TORCH_POS_6,
		TORCH_POS_7,
		TORCH_POS_8,
		TORCH_POS_9,
		TORCH_POS_10,
		TORCH_POS_11,
		TORCH_POS_12,
		TORCH_POS_13,
		TORCH_POS_14,
	};
	// �T�C�Y
	int size = ARRAY_SIZE(torchPosList);

	// ����
	CTorch* torch;
	for (int i = 0; i < size; i++)
	{
		// �����𐶐�
		torch = new CTorch();
		torch->Position(torchPosList[i]);
		// �ŏ��̈�����΂�����
		if (i == 0)
		{
			torch->SetFire(true);
		}
	}

	// ���̍��W�̃��X�g
	CVector grassPosList[] = {
		GRASS_POS_1,
		GRASS_POS_2,
		GRASS_POS_3,
		GRASS_POS_4,
		GRASS_POS_5,
		GRASS_POS_6,
	};
	// �T�C�Y
	size = ARRAY_SIZE(grassPosList);

	// ��
	CGrass* grass;
	for (int i = 0; i < size; i++)
	{
		// ���𐶐�
		grass = new CGrass();
		// �ʒu����
		grass->Position(grassPosList[i]);
		switch (i)
		{
		case 0:
			grass->SetDropItem(mergeArea->GetFragment(1));
			break;
		case 1:
			grass->SetDropItem(mergeArea->GetFragment(2));
			break;
		case 2:
			grass->SetDropItem(mergeArea->GetFragment(3));
			break;
		}
	}

	// �c�^�𐶐�
	grass = new CGrass(EGrassType::eVine, VINE_FIRE_OFFSET_POS, VINE_FIRE_SCALE);
	// �ʒu����
	grass->Position(VINE_POS_1);
	// �c�^�𐶐�
	grass = new CGrass(EGrassType::eVine, VINE_FIRE_OFFSET_POS, VINE_FIRE_SCALE);
	// �ʒu����
	grass->Position(VINE_POS_2);
	// ��]
	grass->Rotation(VINE_ROT_2);

	// �A�C�e���𐶐�
	CUpgradeItem* item;
	item = new CUpgradeItem();
	// ���W��ݒ�
	item->Position(ITEM_POS_1);
	item = new CUpgradeItem();
	// ���W��ݒ�
	item->Position(ITEM_POS_2);
	item = new CUpgradeItem();
	// ���W��ݒ�
	item->Position(ITEM_POS_2);
	item = new CUpgradeItem();
	// ���W��ݒ�
	item->Position(ITEM_POS_3);
	item = new CUpgradeItem();
	// ���W��ݒ�
	item->Position(ITEM_POS_3);
	item = new CUpgradeItem();
	// ���W��ݒ�
	item->Position(ITEM_POS_3);
}

// �o�H�T���p�̃m�[�h�𐶐�
void CMap3::CreateNavNodes()
{
}
