#pragma once
#include "CSceneBase.h"

class CSelectMenu;

// �I����ʂ̃V�[��
class CSelectScene : public CSceneBase
{
public:
	// �R���X�g���N�^
	CSelectScene();
	// �f�X�g���N�^
	~CSelectScene();
	// �V�[���ǂݍ���
	void Load() override;
	// �V�[���̍X�V����
	void Update() override;

private:
	CSelectMenu* mpSelectUI;
};