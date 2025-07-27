#pragma once
#include "CSceneBase.h"

class CGameMenu;
class CRoomManager;

// �Q�[���V�[��
class CGameScene : public CSceneBase
{
public:
	// �R���X�g���N�^
	CGameScene();
	// �f�X�g���N�^
	~CGameScene();
	// �V�[���ǂݍ���
	void Load() override;
	// �V�[���̍X�V����
	void Update() override;

private:
	// �Q�[�����j���[
	CGameMenu* mpGameMenu;
	// �����̊Ǘ��N���X
	CRoomManager* mpRoomMgr;
};