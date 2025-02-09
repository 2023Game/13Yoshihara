#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CDeliveryFieldManager;
class CTimeUI;
class CDeliveryScoreUI;

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

	// �t�B�[���h�Ǘ��N���X
	CDeliveryFieldManager* mpFieldMgr;
	// ���ԕ\��UI
	CTimeUI* mpTimeUI;
	// �X�R�A�\��UI
	CDeliveryScoreUI* mpDeliveryScoreUI;
	// �o�ߎ��Ԃ��v��
	float mElapsedTime;
};
