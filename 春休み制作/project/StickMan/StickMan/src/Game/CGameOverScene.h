#pragma once
#include "CSceneBase.h"
#include "CInput.h"

class CSound;
class CGameOverUI;

// �Q�[���I�[�o�[�V�[��
class CGameOverScene : public CSceneBase
{
public:
	// �R���X�g���N�^
	CGameOverScene();
	// �f�X�g���N�^
	~CGameOverScene();
	// �V�[���ǂݍ���
	void Load();
	// �V�[���̍X�V����
	void Update();

private:
	// �Q�[���I�[�o�[UI
	CGameOverUI* mpGameOverUI;
};