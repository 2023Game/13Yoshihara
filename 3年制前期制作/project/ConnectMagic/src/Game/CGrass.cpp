#include "CGrass.h"
#include "CColliderSphere.h"
#include "CBillBoardImage.h"

// ���̏d��(�������Ȃ��j
#define WEIGHT 1.0f

// �摜�̃T�C�Y
#define SIZE 2.0f

// �ڑ��^�[�Q�b�g�̃I�t�Z�b�g���W
#define TARGET_OFFSET_POS CVector(0.0f, 2.0f, 0.0f)

// �R���X�g���N�^
CGrass::CGrass()
	: CConnectObject(WEIGHT)
{
	// �d�͖���
	mIsGravity = false;

	// ���̉摜�𐶐�
	mpGrassImage = new CBillBoardImage
	(
		"Field/Grass.png", ETag::eConnectObject,
		ETaskPauseType::eGame
	);
	// �e�ɐݒ�
	mpGrassImage->SetParent(this);
	// �T�C�Y���擾
	CVector2 size = mpGrassImage->GetSize();
	size *= SIZE;
	// �T�C�Y��ݒ�
	mpGrassImage->SetSize(size);
	// �T�C�Y�̔������ɏグ��
	mpGrassImage->SetOffsetPos(CVector(0.0f, size.Y() * 0.8f, 0.0f));

	// �ڑ��^�[�Q�b�g�𐶐�
	CreateTarget(Position() + TARGET_OFFSET_POS);

	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CGrass::~CGrass()
{
}

// �q�������Ƃ��̏���
void CGrass::Connect(CConnectObject* other)
{
	// �ڑ��I�u�W�F�N�g�̃^�O�����Ȃ�
	if (other->GetConnectObjTag() == EConnectObjTag::eFire)
	{
		// TODO�F���X�ɏ����Ă���
	}
}

// �R���C�_�[�𐶐�
void CGrass::CreateCol()
{
	mpCol = new CColliderSphere
	(
		this,ELayer::eObject,
		10.0f,true
	);
	// �R�l�N�g�I�u�W�F�N�g�̒T�m�p�Ƃ����Փ˔���
	mpCol->SetCollisionLayers({ ELayer::eConnectSearch });
}
