#pragma once
#include "CFieldBase.h"
#include <array>

// �t�B�[���h�Ǘ��N���X
class CFieldManager
{
public:
	// �t�B�[���h�Ǘ��N���X�̃C���X�^���X���擾
	static CFieldManager* Instance();
	// �R���X�g���N�^
	CFieldManager();
	// �f�X�g���N�^
	~CFieldManager();

private:
	// �t�B�[���h�Ǘ��N���X�̃C���X�^���X
	static CFieldManager* spInstance;
	// �}�b�v�̔z��
	CFieldBase* mMapList[3][3];
};