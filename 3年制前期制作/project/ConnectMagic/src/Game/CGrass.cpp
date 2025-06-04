#include "CGrass.h"
#include "CColliderRectangle.h"
#include "CBillBoardImage.h"
#include "CFlamethrower.h"
#include "CConnectPointManager.h"

// �摜�̃T�C�Y
#define GRASS_SIZE 2.0f
#define VINE_SIZE 12.0f

// �A���t�@�̌������x
#define ALPHA_DECREASE_SPEED 0.5f

// ���_
#define VERT_POS_1 CVector(-VINE_WIDTH/2.0f, VINE_HEIGHT, 0.0f)
#define VERT_POS_2 CVector(-VINE_WIDTH/2.0f,		0.0f, 0.0f)
#define VERT_POS_3 CVector( VINE_WIDTH/2.0f,		0.0f, 0.0f)
#define VERT_POS_4 CVector( VINE_WIDTH/2.0f, VINE_HEIGHT, 0.0f)

// �R���X�g���N�^
CGrass::CGrass(EGrassType type, CVector fireOffsetPos, float fireScale)
	: CFire(fireOffsetPos, fireScale)
	, CItemDrop()
	, mGrassType(type)
	, mpVineCol(nullptr)
{
	// �^�O�𑐂ɐݒ�
	SetConnectObjTag(EConnectObjTag::eGrass);

	// �摜�𐶐�
	CreateImg();

	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CGrass::~CGrass()
{
	SAFE_DELETE(mpVineCol);
}

// �X�V
void CGrass::Update()
{
	// ���N���X�̍X�V����
	CConnectObject::Update();

	// �������Ă���Ȃ�
	if (mIsFire)
	{
		// �A���t�@�����Z���Ă���
		float alpha = mpGrassImage->GetAlpha();
		alpha -= ALPHA_DECREASE_SPEED * Times::DeltaTime();
		// �A���t�@��0�ȉ��Ȃ�
		if (alpha <= 0.0f)
		{
			// �S�č폜
			Delete();
			return;
		}
		// �A���t�@��ݒ�
		mpGrassImage->SetAlpha(alpha);
	}
}

// �R���C�_�[�𐶐�
void CGrass::CreateCol()
{
	switch (mGrassType)
	{
		// �ʏ�̑��Ȃ�
	case EGrassType::eGrass:
		// ���N���X�̃R���C�_�[�𐶐�
		CFire::CreateCol();
		break;

		// �c�^�Ȃ�
	case EGrassType::eVine:
		// �l�p�`�R���C�_�[�𐶐�
		mpCol = new CColliderRectangle(
			this, ELayer::eObject,
			VERT_POS_1,
			VERT_POS_2,
			VERT_POS_3,
			VERT_POS_4,
			true
		);
		// �������炷
		mpCol->Position(VectorZ());
		// �I�u�W�F�N�g�ƃv���C���[�ƓG��
		// �ڑ��I�u�W�F�N�g�̒T�m�p�Ƃ����Փ˔���
		mpCol->SetCollisionLayers({ ELayer::eObject,
			ELayer::ePlayer,ELayer::eEnemy,ELayer::eConnectSearch });
		mpVineCol = new CColliderRectangle(
			this, ELayer::eObject,
			VERT_POS_4,
			VERT_POS_3,
			VERT_POS_2,
			VERT_POS_1,
			true
		);
		// �������炷
		mpVineCol->Position(-VectorZ());
		// �I�u�W�F�N�g�ƃv���C���[�ƓG��
		// �ڑ��I�u�W�F�N�g�̒T�m�p�Ƃ����Փ˔���
		mpVineCol->SetCollisionLayers({ ELayer::eObject,
			ELayer::ePlayer,ELayer::eEnemy,ELayer::eConnectSearch });
		break;
	}
}

// �R�����Ƃ��̏���
void CGrass::Burning()
{
	// ���Ƃ��A�C�e���̍��W��ݒ�
	SetDropItemPos(Position());
	// �A�C�e���𗎂Ƃ�
	Drop();
}

// �S�č폜
void CGrass::Delete()
{
	// ���������폜
	mpFlamethrower->Kill();
	mpFlamethrower = nullptr;
	// ���̉摜���폜
	mpGrassImage->Kill();
	mpGrassImage = nullptr;
	// �폜
	Kill();
}

// �摜�𐶐�
void CGrass::CreateImg()
{
	switch (mGrassType)
	{
	case EGrassType::eGrass:
	{
		// ���̉摜�𐶐�
		mpGrassImage = new CBillBoardImage
		(
			"Field/Grass/Grass.png", ETag::eConnectObject,
			ETaskPauseType::eGame
		);
		// �T�C�Y���擾
		CVector2 size = mpGrassImage->GetSize();
		size *= GRASS_SIZE;
		// �T�C�Y��ݒ�
		mpGrassImage->SetSize(size);
		// �T�C�Y�̔�������ɏグ��
		mpGrassImage->SetOffsetPos(CVector2(0.0f, size.Y() * 0.8f));
		break;
	}
	case EGrassType::eVine:
		// �c�^�̉摜�𐶐�
		mpGrassImage = new CImage3D(
			"Field/Grass/Vine.png", ETag::eConnectObject,
			ETaskPriority::eDefault, 0,
			ETaskPauseType::eGame
		);
		// ���ʕ`��
		mpGrassImage->SetBackFace(true);
		// �T�C�Y���擾
		CVector2 size = mpGrassImage->GetSize();
		size *= VINE_SIZE;
		// �T�C�Y��ݒ�
		mpGrassImage->SetSize(size);
		// �T�C�Y����ɏグ��
		mpGrassImage->SetOffsetPos(CVector2(0.0f, size.Y()));
		break;
	}
	// �e�ɐݒ�
	mpGrassImage->SetParent(this);
}
