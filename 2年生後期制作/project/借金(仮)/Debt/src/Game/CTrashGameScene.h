#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CSound;

//�^�C�g���V�[��
class CTrashGameScene :public CSceneBase
{
public:
	//�R���X�g���N�^
	CTrashGameScene();
	//�f�X�g���N�^
	~CTrashGameScene();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	CGameMenuBase* mpGameMenu;
};
