#include "CBox.h"
#include "CColliderMesh.h"

#define TARGET_POS_1 CVector( 0.0f,10.0f, 0.0f)
#define TARGET_POS_2 CVector( 0.0f, 0.0f, 0.0f)
#define TARGET_POS_3 CVector( 5.0f, 5.0f, 0.0f)
#define TARGET_POS_4 CVector(-5.0f, 5.0f, 0.0f)
#define TARGET_POS_5 CVector( 0.0f, 5.0f, 5.0f)
#define TARGET_POS_6 CVector( 0.0f, 5.0f,-5.0f)

// �R���X�g���N�^
CBox::CBox()
	: CConnectObject()
{
	mpModel = CResourceManager::Get<CModel>("Box");

	Scale(Scale() * 2.0f);

	// �R���C�_�[�𐶐�
	CreateCol();

	// �ڑ��^�[�Q�b�g�𐶐�
	CreateTarget(TARGET_POS_1);
	CreateTarget(TARGET_POS_2);
	CreateTarget(TARGET_POS_3);
	CreateTarget(TARGET_POS_4);
	CreateTarget(TARGET_POS_5);
	CreateTarget(TARGET_POS_6);
}

// �f�X�g���N�^
CBox::~CBox()
{
}

// �X�V
void CBox::Update()
{
}

// ��������ꂽ���̏���
void CBox::Pull()
{
}

// �R���C�_�[�𐶐�
void CBox::CreateCol()
{
	mpCol = new CColliderMesh
	(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("Box_Col"),
		true
	);
	// �v���C���[�A�t�B�[���h�A�G�A�I�u�W�F�N�g�A�R�l�N�g�I�u�W�F�N�g�̒T�m�p�ƏՓ˔���
	mpCol->SetCollisionLayers({ ELayer::ePlayer,
		ELayer::eGround,ELayer::eWall,ELayer::eObject,ELayer::eConnectSearch });
}
