#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CSound;

//�^�C�g���V�[��
class CHomeScene :public CSceneBase
{
public:
	//�R���X�g���N�^
	CHomeScene();
	//�f�X�g���N�^
	~CHomeScene();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	CGameMenuBase* mpGameMenu;
};
