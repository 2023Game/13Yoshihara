#pragma once
#include "CSceneBase.h"

class CGameMenu;

// �`���[�g���A���V�[��
class CTutorialScene : public CSceneBase
{
public:
	// �R���X�g���N�^
	CTutorialScene();
	// �f�X�g���N�^
	~CTutorialScene();

	// �V�[���ǂݍ���
	void Load() override;
	// �V�[���̍X�V����
	void Update() override;

private:
	// �Q�[�����j���[
	CGameMenu* mpGameMenu;
};