#pragma once
#include "CConnectObject.h"
#include "FragmentType.h"
#include "CDropItem.h"

class CImage3D;
class CFragmentMergeArea;

// �|�[�^���̌��ЃN���X
class CPortalFragment : public CConnectObject, public CDropItem
{
public:
	// �R���X�g���N�^
	CPortalFragment(EFragmentType fragmentType, CFragmentMergeArea* mergeArea);
	// �f�X�g���N�^
	~CPortalFragment();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// ���ׂč폜
	void Delete() override;

private:
	// ���
	enum class EState
	{
		eJump,	// �ŏ��̔�яオ��
		eIdle,	// �ҋ@
		eMerge,	// ����
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);
	EState mState;
	int mStateStep;		// ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime;	// �o�ߎ��Ԍv���p

	// ��яオ��
	void UpdateJump();	
	// �ҋ@
	void UpdateIdle();	
	// ����
	void UpdateMerge();

	// �R���C�_�[�𐶐�
	void CreateCol();
	// ���Ђ̎��
	EFragmentType mFragmentType;

	// ���Ђ̉摜
	CImage3D* mpFragmentImg;

	// �����n
	CFragmentMergeArea* mpMergeArea;
};