#pragma once
#include "CSceneBase.h"
#include "CInput.h"

class CSound;
class CGameClearUI;

// �Q�[���N���A�V�[��
class CGameClearScene : public CSceneBase
{
public:
	// �R���X�g���N�^
	CGameClearScene();
	// �f�X�g���N�^
	~CGameClearScene();
	// �V�[���ǂݍ���
	void Load();
	// �V�[���̍X�V����
	void Update();

private:
	// �Q�[���N���AUI
	CGameClearUI* mpGameClearUI;
};