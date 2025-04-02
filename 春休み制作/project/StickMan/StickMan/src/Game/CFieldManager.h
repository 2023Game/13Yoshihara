#pragma once
#include "CMapBase.h"
#include <array>

// �}�b�v�̍s�Ɨ�̐�
#define MAP_ROW 3
#define MAP_COL 3

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

	// �n�ʂ̃R���C�_�\�擾
	CCollider* GetGroundCol();
	// �ǂ̃R���C�_�\�擾
	CCollider* GetWallCol();
	// �I�u�W�F�N�g�̃R���C�_�\�擾
	CCollider* GetObjCol();
private:
	// �t�B�[���h�Ǘ��N���X�̃C���X�^���X
	static CFieldManager* spInstance;
	// �}�b�v�̔z��
	CMapBase* mpMapList[MAP_ROW][MAP_COL];

	// �}�b�v�𐶐�
	void CreateMap();
};