#pragma once
#include "CFieldBase.h"

class CMapBase;

// ��
class CTable : public CFieldBase
{
public:
	// �R���X�g���N�^
	CTable();
	// �f�X�g���N�^
	~CTable();

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects();

	// �e�[�u����̃}�b�v�𐶐�
	CMapBase* mpMap;
};