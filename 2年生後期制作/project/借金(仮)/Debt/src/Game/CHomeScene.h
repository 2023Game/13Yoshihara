#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CSound;
class CToDoUI;
class CSelectJobUI;
class CMoneyUI;

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
	// �Q�[�����j���[
	CGameMenu* mpGameMenu;
	// ��邱�ƕ\��UI
	CToDoUI* mpToDoUI;
	// �I�𒆂̎d�����\��UI
	CSelectJobUI* mpSelectJobUI;
	// �������̕\��UI
	CMoneyUI* mpMoneyUI;
};
