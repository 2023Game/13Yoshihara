#pragma once
#include "CObjectBase.h"

// ���Ђ���������ꏊ
class CFragmentMergeArea : public CObjectBase
{
public:
	// �R���X�g���N�^
	CFragmentMergeArea();
	// �f�X�g���N�^
	~CFragmentMergeArea();

private:
	// �R���C�_�[�𐶐�
	void CreateCol() override;
	// ���Ђ���������ꏊ�̃R���C�_�[
	CCollider* mpMergeAreaCol;
};