#pragma once
#include "CSceneBase.h"
class CMenu;
class CSound;
class CToDoUI;
class CSelectJobUI;
class CMoneyUI;

// ���_�V�[��
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
	CMenu* mpGameMenu;
	// ��邱�ƕ\��UI
	CToDoUI* mpToDoUI;
	// �I�𒆂̎d�����\��UI
	CSelectJobUI* mpSelectJobUI;
	// �������̕\��UI
	CMoneyUI* mpMoneyUI;
};
