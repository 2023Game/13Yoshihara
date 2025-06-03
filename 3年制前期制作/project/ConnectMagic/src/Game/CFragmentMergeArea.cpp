#include "CFragmentMergeArea.h"
#include "CColliderSphere.h"
#include "CPortal.h"
#include "CPortalFragment.h"

// ���a
#define RADIUS 10.0f

// �|�[�^���̃I�t�Z�b�g���W
#define PORTAL_OFFSET_POS CVector(0.0f,10.0f,0.0f)

// �R���X�g���N�^
CFragmentMergeArea::CFragmentMergeArea(int num)
	: CObjectBase(ETag::eFragmentMergeArea, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
{
	for (int i = 0; i < num; i++)
	{
		// ���Ђ𐶐�
		mFragments.push_back(new CPortalFragment(static_cast<EFragmentType>(i), this));
		// ���Ђ������ꏊ�ɂȂ�
		mIsFragments.push_back(false);
	}
	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CFragmentMergeArea::~CFragmentMergeArea()
{
	SAFE_DELETE(mpMergeAreaCol);
}

// ���Ђ����邩��ݒ�
void CFragmentMergeArea::SetIsFragment(int num, bool enable)
{
	// �T�C�Y�O�Ȃ珈�����Ȃ�
	if (mIsFragments.size() <= num) return;
	mIsFragments[num] = enable;
}

// ���Ђ̃|�C���^���擾
CPortalFragment* CFragmentMergeArea::GetFragment(int num)
{
	// �T�C�Y�O�Ȃ珈�����Ȃ�
	if (mFragments.size() <= num) return nullptr;
	return mFragments[num];
}

// �X�V
void CFragmentMergeArea::Update()
{
	// �S��true�Ȃ�
	for (bool isFragment : mIsFragments)
	{
		if (!isFragment) return;
	}

	// ����
	Merge();
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

// ���ׂč폜
void CFragmentMergeArea::Delete()
{
	for (int i = 0; i < mFragments.size(); i++)
	{
		// �폜
		mFragments[i]->Kill();
		mFragments[i]->Delete();
	}
	// �z�����ɂ���
	mFragments.clear();
}

// ����
void CFragmentMergeArea::Merge()
{
	// �|�[�^���𐶐�
	CPortal* portal = new CPortal();
	// ���W�𐶐�
	portal->Position(Position() + PORTAL_OFFSET_POS);

	// ���ׂč폜
	Delete();
	// ���g���폜
	Kill();
}
