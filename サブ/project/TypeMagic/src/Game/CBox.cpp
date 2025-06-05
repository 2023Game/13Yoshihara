#include "CBox.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"
#include "CConnectPointManager.h"

// �ڑ��^�[�Q�b�g�̍��W
#define TARGET_POS_1 CVector( 0.0f,10.0f, 0.0f)
#define TARGET_POS_2 CVector( 0.0f, 0.0f, 0.0f)
#define TARGET_POS_3 CVector( 5.0f, 5.0f, 0.0f)
#define TARGET_POS_4 CVector(-5.0f, 5.0f, 0.0f)
#define TARGET_POS_5 CVector( 0.0f, 5.0f, 5.0f)
#define TARGET_POS_6 CVector( 0.0f, 5.0f,-5.0f)

// ���̏d��
#define WEIGHT 0.1f

// �R���C�_�[�̔��a
#define RADIUS 2.4f

// �R���X�g���N�^
CBox::CBox()
	: CConnectObject(WEIGHT)
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
	SAFE_DELETE(mpCharaCol);
}

// �R���C�_�[�𐶐�
void CBox::CreateCol()
{
	// �t�B�[���h��I�u�W�F�N�g�ƏՓ˔��������R���C�_�[
	mpCol = new CColliderSphere
	(
		this, ELayer::eObject,
		RADIUS
	);
	// ���W�𒲐�
	mpCol->Position(Position() + CVector(0.0f, RADIUS / 2, 0.0f));
	// �t�B�[���h�A�I�u�W�F�N�g�A�R�l�N�g�I�u�W�F�N�g�̒T�m�p�A�X�C�b�`�ƏՓ˔���
	mpCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall,
		ELayer::eObject, ELayer::eConnectSearch, ELayer::eSwitch });

	// �L�����ƏՓ˔��������R���C�_�[
	mpCharaCol = new CColliderMesh
	(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("Box_Col")
	);
	// �v���C���[�A�G�A�I�u�W�F�N�g�ƏՓ˔���
	mpCharaCol->SetCollisionLayers({ ELayer::ePlayer, ELayer::eEnemy, ELayer::eObject });

	// �ڑ����̊Ǘ��N���X�̏Փ˔��肷��R���C�_�[�ɒǉ�
	CConnectPointManager::Instance()->AddCollider(mpCharaCol);

	// �J�����̏Փ˔��肷��R���C�_�[�ɒǉ�
	CCamera::CurrentCamera()->AddCollider(mpCharaCol);
}
