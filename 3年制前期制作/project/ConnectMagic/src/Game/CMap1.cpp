#include "CMap1.h"
#include "CBridge.h"

// �����̉��̃I�t�Z�b�g���W
#define TORCH_FIRE_OFFSET_POS CVector(0.0f,9.0f,0.0f)

// ���̃I�t�Z�b�g���W
#define BRIDGE_OFFSET_POS_1 CVector(-325.0f,0.0f,252.5f)
#define BRIDGE_OFFSET_POS_2 CVector(-325.0f,0.0f,127.5f)

// ���̃I�t�Z�b�g��]
#define BRIDGE_OFFSET_ROT_2 CVector(0.0f,180.0f,0.0f)

// ���̃f�t�H���g�p�x
#define BRIDGE_ANGLE_1 90.0f

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
	CBridge* bridge = new CBridge();
	// �ʒu�ݒ�
	bridge->Position(BRIDGE_OFFSET_POS_1);
	// �����p�x��ݒ�
	bridge->SetAngle(BRIDGE_ANGLE_1);

	// ���𐶐�
	bridge = new CBridge();
	// �ʒu�ݒ�
	bridge->Position(BRIDGE_OFFSET_POS_2);
	// ��]�ݒ�
	bridge->Rotate(BRIDGE_OFFSET_ROT_2);
}

// �o�H�T���p�̃m�[�h�𐶐�
void CMap1::CreateNavNodes()
{
}
