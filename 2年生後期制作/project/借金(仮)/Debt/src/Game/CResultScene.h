#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"
class CSound;
class CResultUIBase;

// ���U���g�V�[��
class CResultScene : public CSceneBase
{
public:
	//�R���X�g���N�^
	CResultScene();
	//�f�X�g���N�^
	~CResultScene();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	CResultUIBase* mpResultUI;
};