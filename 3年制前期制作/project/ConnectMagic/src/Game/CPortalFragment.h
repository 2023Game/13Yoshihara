#pragma once
#include "CConnectObject.h"
#include "CFragmentType.h"
#include "CFragmentMergeArea.h"

class CImage3D;

// �|�[�^���̌��ЃN���X
class CPortalFragment : public CConnectObject
{
public:
	// �R���X�g���N�^
	CPortalFragment(EFragmentType fragmentType);
	// �f�X�g���N�^
	~CPortalFragment();

	// �X�V
	void Update() override;

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol();
	// ���Ђ̎��
	EFragmentType mFragmentType;

	// ���Ђ̉摜
	CImage3D* mpFragmentImg;

	// �����ꏊ�ɒ�������
	bool mIsMerge;

	// �����ꏊ�̍��W
	CVector mMergePos;

	// �����n
	CFragmentMergeArea* mpMergeArea;
};