#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CSound;

//�^�C�g���V�[��
class CDustGameScene :public CSceneBase
{
public:
	//�R���X�g���N�^
	CDustGameScene();
	//�f�X�g���N�^
	~CDustGameScene();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	CGameMenuBase* mpGameMenu;
};
