#include "CCampFire.h"
#include "CColliderMesh.h"

// ���̃I�t�Z�b�g���W
#define FIRE_OFFSET_POS CVector(0.0f,3.0f,0.0f)
// ���̑傫��
#define FIRE_SCALE 2.5f
// ���̈ړ����x
#define FIRE_SPEED 10.0f
// �ڑ��^�[�Q�b�g�̃I�t�Z�b�g���W
#define TARGET_OFFSET_POS CVector(0.0f,15.0f,0.0f)

// �R���X�g���N�^
CCampFire::CCampFire()
	: CFire(FIRE_OFFSET_POS, FIRE_SCALE, FIRE_SPEED, TARGET_OFFSET_POS)
	, CItemDrop()
{
	mpModel = CResourceManager::Get<CModel>("CampFire");

	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CCampFire::~CCampFire()
{
}

// �R���C�_�[�𐶐�
void CCampFire::CreateCol()
{
	mpCol = new CColliderMesh(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("CampFire_Col"),
		true
	);
	// �I�u�W�F�N�g�ƃv���C���[�ƓG��
	// �ڑ��I�u�W�F�N�g�̒T�m�p�Ƃ����Փ˔���
	mpCol->SetCollisionLayers({ ELayer::eObject,ELayer::ePlayer,
		ELayer::eEnemy,ELayer::eConnectSearch });
}

// �R�����Ƃ��̏���
void CCampFire::Burning()
{
	// ���Ƃ��A�C�e���̍��W��ݒ�
	SetDropItemPos(Position());
	// �A�C�e���𗎂Ƃ�
	Drop();
}
