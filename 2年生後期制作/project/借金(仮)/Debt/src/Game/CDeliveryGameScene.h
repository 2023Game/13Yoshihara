#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CTimeUI;
class CTrashScoreUI;

//�^�C�g���V�[��
class CDeliveryGameScene :public CSceneBase
{
public:
	//�R���X�g���N�^
	CDeliveryGameScene();
	//�f�X�g���N�^
	~CDeliveryGameScene();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	// �Q�[�����j���[
	CGameMenuBase* mpGameMenu;

	// ���ԕ\��UI
	CTimeUI* mpTimeUI;
	// �X�R�A�\��UI
	CTrashScoreUI* mpTrashScoreUI;
	// �o�ߎ��Ԃ��v��
	float mElapsedTime;
};
