#include "CFragmentMergeArea.h"
#include "CColliderSphere.h"

// ���a
#define RADIUS 10.0f

// �R���X�g���N�^
CFragmentMergeArea::CFragmentMergeArea()
	: CObjectBase(ETag::eFragmentMergeArea, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
{
	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CFragmentMergeArea::~CFragmentMergeArea()
{
}

// �R���C�_�[�𐶐�
void CFragmentMergeArea::CreateCol()
{
	mpMergeAreaCol = new CColliderSphere(
		this, ELayer::eFragmentMergeArea,
		RADIUS, true
	);
	// �I�u�W�F�N�g�Ƃ����Փ˔���
	mpMergeAreaCol->SetCollisionLayers({ ELayer::eObject });
}
