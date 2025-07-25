#include "CMap1.h"
#include "CWater.h"
#include "CBox.h"
#include "CUpgradeItem.h"
#include "Maths.h"
#include "CSwitch.h"

// ���ʂ̍��W
#define WATER_POS CVector(-250.0f,-10.0f,50.0f)
// ���ʂ̃X�P�[��
#define WATER_SCALE CVector(500.0f,1.0f,300.0f)

// �ŏ��ɔz�u����A�C�e���̍��W
#define ITEM_POS_1	CVector(0.0f,0.0f,50.0f)
#define ITEM_POS_2	CVector(0.0f,0.0f,270.0f)
#define ITEM_POS_3	CVector(-270.0f,0.0f,90.0f)

// ���̍��W
#define BOX_POS_1 CVector(-125.0f,100.0f,85.0f)
#define BOX_POS_2 CVector(61.5f,0.0f,413.0f)
#define BOX_POS_3 CVector(161.5f,0.0f,523.0f)
// �ł������̍��W
#define BIG_BOX_POS CVector(177.5,0.0f,128.0f)
// �f�J�����̃X�P�[���{��
#define BIG_BOX_SCALE 9.0f

// ������ǂ̍��W
#define DOWN_WALL_POS_1 CVector(116.5f,0.0f,128.0f)
#define DOWN_WALL_POS_2 CVector(-225.5f,0.0f,468.0f)
// �X�C�b�`�̍��W
#define SWITCH_POS CVector(-70.0f,0.0f,50.0f)
// 3�A�X�C�b�`�̍��W
#define SWITCHS_POS_1 CVector(-150.0f,0.0f,518.0f)
#define SWITCHS_POS_2 CVector(-150.0f,0.0f,468.0f)
#define SWITCHS_POS_3 CVector(-150.0f,0.0f,418.0f)

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
	// ���𐶐�
	CBox* box = new CBox(BOX_POS_1);
	// ���𐶐�
	box = new CBox(BOX_POS_2);
	// ���𐶐�
	box = new CBox(BOX_POS_3);

	// �f�J�����𐶐�
	CBox* bigBox = new CBox(BIG_BOX_POS, BIG_BOX_SCALE);

	//// �X�C�b�`�𐶐�
	//std::vector<CSwitch*> switchs = { new CSwitch(SWITCH_POS) };

	//// ������ǂ𐶐�
	//CDownWall* downWall = new CDownWall(DOWN_WALL_POS_1, switchs);

	//int size = switchs.size();
	//for (int i = 0; i < size; i++)
	//{
	//	// ��p����I�u�W�F�N�g�ɉ�����ǂ�ݒ�
	//	switchs[i]->SetActionObj(downWall);
	//}

	//// �X�C�b�`�𐶐�
	//switchs =
	//{ new CSwitch(SWITCHS_POS_1), new CSwitch(SWITCHS_POS_2), new CSwitch(SWITCHS_POS_3) };

	//// ������ǂ𐶐�
	//downWall = new CDownWall(DOWN_WALL_POS_2, switchs);

	//size = switchs.size();
	//for (int i = 0; i < size; i++)
	//{
	//	// ��p����I�u�W�F�N�g�ɉ�����ǂ�ݒ�
	//	switchs[i]->SetActionObj(downWall);
	//}
}

// �o�H�T���p�̃m�[�h�𐶐�
void CMap1::CreateNavNodes()
{
}
