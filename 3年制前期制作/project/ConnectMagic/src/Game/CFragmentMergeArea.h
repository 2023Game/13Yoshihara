#pragma once
#include "CObjectBase.h"

class CPortalFragment;

// ���Ђ���������ꏊ
class CFragmentMergeArea : public CObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="num">���Ђ̐�</param>
	CFragmentMergeArea(int num);
	// �f�X�g���N�^
	~CFragmentMergeArea();

	/// <summary>
	/// ���Ђ����邩��ݒ�
	/// </summary>
	/// <param name="num">�ԍ�</param>
	/// <param name="enable">���邩</param>
	void SetIsFragment(int num, bool enable);
	// ���Ђ̃|�C���^���擾
	CPortalFragment* GetFragment(int num);

	// �X�V
	void Update() override;

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
	// ���Ђ���������ꏊ�̃R���C�_�[
	CCollider* mpMergeAreaCol;

	// ���ׂč폜
	void Delete() override;

	// ����
	void Merge();

	// ����
	std::vector<CPortalFragment*> mFragments;

	// ���Ђ������ꏊ�ɂ��邩
	std::vector<bool> mIsFragments;
};