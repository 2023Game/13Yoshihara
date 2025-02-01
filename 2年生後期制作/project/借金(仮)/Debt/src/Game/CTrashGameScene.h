#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CVehicleManager;
class CResidentManager;
class CSound;
class CTimeUI;
class CTrashScoreUI;

//�^�C�g���V�[��
class CTrashGameScene :public CSceneBase
{
public:
	//�R���X�g���N�^
	CTrashGameScene();
	//�f�X�g���N�^
	~CTrashGameScene();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	// �Q�[�����j���[
	CGameMenuBase* mpGameMenu;
	// �ԗ��Ǘ��N���X
	CVehicleManager* mpVehicleManager;
	// �Z�l�Ǘ��N���X
	CResidentManager* mpResidentManager;
	// ���ԕ\��UI
	CTimeUI* mpTimeUI;
	// �X�R�A�\��UI
	CTrashScoreUI* mpTrashScoreUI;
	// �o�ߎ��Ԃ��v��
	float mElapsedTime;
	// �|�[�Y�������ǂ���
	bool mIsPause;
};
