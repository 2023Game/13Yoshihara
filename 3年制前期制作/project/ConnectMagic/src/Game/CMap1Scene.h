#pragma once
#include "CSceneBase.h"

class CGameMenu;

// �}�b�v�P
class CMap1Scene : public CSceneBase
{
public:
	// �R���X�g���N�^
	CMap1Scene();
	// �f�X�g���N�^
	~CMap1Scene();
	// �V�[���ǂݍ���
	void Load() override;
	// �V�[���̍X�V����
	void Update() override;

private:
	// �Q�[�����j���[
	CGameMenu* mpGameMenu;
};