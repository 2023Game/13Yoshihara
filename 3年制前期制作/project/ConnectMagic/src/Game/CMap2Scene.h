#pragma once
#include "CSceneBase.h"

class CGameMenu;

// �`���[�g���A���V�[��
class CMap2Scene : public CSceneBase
{
public:
	// �R���X�g���N�^
	CMap2Scene();
	// �f�X�g���N�^
	~CMap2Scene();

	// �V�[���ǂݍ���
	void Load() override;
	// �V�[���̍X�V����
	void Update() override;

private:
	// �Q�[�����j���[
	CGameMenu* mpGameMenu;
};