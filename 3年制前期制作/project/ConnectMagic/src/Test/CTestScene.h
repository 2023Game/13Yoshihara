#pragma once
#include "CSceneBase.h"

class CGameMenu;

// �e�X�g�V�[��
class CTestScene :public CSceneBase
{
public:
	//�R���X�g���N�^
	CTestScene();
	//�f�X�g���N�^
	~CTestScene();
	//�V�[���ǂݍ���
	void Load() override;
	//�V�[���̍X�V����
	void Update() override;

private:
	// �Q�[�����j���[
	CGameMenu* mpGameMenu;
};
