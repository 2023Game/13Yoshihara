#pragma once
#pragma once
#include "CSceneBase.h"

class CGameMenu;

class CFieldBase;

// �}�b�v3
class CMap3Scene : public CSceneBase
{
public:
	// �R���X�g���N�^
	CMap3Scene();
	// �f�X�g���N�^
	~CMap3Scene();
	// �V�[���ǂݍ���
	void Load() override;
	// �V�[���̍X�V����
	void Update() override;

private:
	// �Q�[�����j���[
	CGameMenu* mpGameMenu;

	CFieldBase* mpField;
};