#include "CAirConnectObj.h"
#include "CColliderSphere.h"

// �ڑ��^�[�Q�b�g�̍��W
#define TARGET_POS CVector(2.5f,0.0f,0.0f)

// �󒆂̐ڑ��I�u�W�F�N�g�̏d��
#define WEIGHT 1.0f

// �R���C�_�[�̔��a
#define RADIUS 5.0f

// �R���X�g���N�^
CAirConnectObj::CAirConnectObj()
	: CConnectObject(WEIGHT)
{
	// �󒆂̐ڑ��I�u�W�F�N�g�̃^�O��ݒ�
	SetConnectObjTag(EConnectObjTag::eAir);

	mpModel = CResourceManager::Get<CModel>("AirConnectObj");

	// �R���C�_�[�𐶐�
	CreateCol();

	// �ڑ��^�[�Q�b�g�𐶐�
	CreateTarget(TARGET_POS);
}

// �f�X�g���N�^
CAirConnectObj::~CAirConnectObj()
{
}

// �R���C�_�[�𐶐�
void CAirConnectObj::CreateCol()
{
	mpCol = new CColliderSphere
	(
		this, ELayer::eObject,
		RADIUS, true
	);
	// �v���C���[�A�G�A�I�u�W�F�N�g�A�R�l�N�g�I�u�W�F�N�g�̒T�m�p�ƏՓ˔��������
	mpCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eObject,ELayer::eConnectSearch });
}
