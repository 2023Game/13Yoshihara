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
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	// �Q�[�����j���[
	CGameMenu* mpGameMenu;
};
