#include "CPortalFragment.h"
#include "CImage3D.h"
#include "CColliderCapsule.h"

// �摜�̍��W
#define IMG_OFFSET_POS CVector(0.0f,10.0f,0.0f)

// �R���C�_�[�̍���
#define HEIGHT 10.0f
// �R���C�_�[�̔��a
#define RADIUS 5.0f

// �����̈ړ����x
#define MERGE_SPEED 10.0f

// �R���X�g���N�^
CPortalFragment::CPortalFragment(EFragmentType fragmentType)
	: CConnectObject(0.1f, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mIsMerge(false)
	, mMergePos(CVector::zero)
	, mpMergeArea(nullptr)
{
	std::string path = "";
	// �摜�̃p�X��ݒ�
	switch (fragmentType)
	{
	case EFragmentType::eFragment1:
		path = "Field/PortalFragment/PortalFragment1.png";
		break;
	case EFragmentType::eFragment2:
		path = "Field/PortalFragment/PortalFragment2.png";
		break;
	case EFragmentType::eFragment3:
		path = "Field/PortalFragment/PortalFragment3.png";
		break;
	case EFragmentType::eFragment4:
		path = "Field/PortalFragment/PortalFragment4.png";
		break;
	}
	// �|�[�^���̌��Ђ̉摜�𐶐�
	mpFragmentImg = new CImage3D(
		path,
		ETag::eConnectObject,
		ETaskPriority::eDefault, 0,
		ETaskPauseType::eGame
	);
	// �e�ɐݒ�
	mpFragmentImg->SetParent(this);
	// ���ʕ`�悷��
	mpFragmentImg->SetBackFace(true);
	mpFragmentImg->Position(IMG_OFFSET_POS);

	// �R���C�_�[�𐶐�
	CreateCol();

	// �ڑ��^�[�Q�b�g�𐶐�
	CreateTarget(IMG_OFFSET_POS);
}

// �f�X�g���N�^
CPortalFragment::~CPortalFragment()
{
}

// �X�V
void CPortalFragment::Update()
{
	// ���N���X�̍X�V����
	CConnectObject::Update();

	// �����ꏊ�ɒ������Ȃ�
	if (mIsMerge)
	{
		// �����n�ւ̕���
		CVector dir = mMergePos - Position();
		// ����
		float distance = dir.Length();
		// ���K��
		dir.Normalize();
		// �ړ�����
		float moveDistance = MERGE_SPEED * Times::DeltaTime();
		// �ړ������̕����������
		if (distance < moveDistance)
		{
			// ���W��ݒ�
			Position(mMergePos);
			// ����
			SetEnable(false);
		}
		// �Z�����
		else
		{
			// �V�������W���v�Z
			CVector newPos = Position() + dir * moveDistance;
			Position(newPos);
		}
	}
}

// �Փˏ���
void CPortalFragment::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ���N���X�̏Փˏ���
	CConnectObject::Collision(self, other, hit);

	if (self == mpCol)
	{
		// ���Ђ���������ꏊ�̏ꍇ
		if (other->Layer() == ELayer::eFragmentMergeArea)
		{
			// �d�͖���
			SetGravity(false);
			// �R���C�_�𖳌�
			SetEnableCol(false);
			// �����ꏊ�̍��W
			mMergePos = other->Owner()->Position();
			// �����J�n
			mIsMerge = true;
		}
	}
}

// �R���C�_�[�𐶐�
void CPortalFragment::CreateCol()
{
	mpCol = new CColliderCapsule(
		this, ELayer::eObject,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, HEIGHT, 0.0f),
		RADIUS
	);
	// �t�B�[�h�ƃv���C���[��
	// �G�Ɛڑ��I�u�W�F�N�g�̒T�m�p�ƌ��Ђ���������ꏊ�Ƃ����Փ˔���
	mpCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,
		ELayer::ePlayer,ELayer::eEnemy,ELayer::eConnectSearch,
		ELayer::eFragmentMergeArea });
}
