#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CTrashVehicleManager;
class CResidentManager;
class CTrashEnemyManager;
class CTimeUI;
class CTrashScoreUI;
class CManualMenu;
class CTrashWarningUI;

// �S�~�E���Q�[���V�[��
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
	CGameMenu* mpGameMenu;
	// �ԗ��Ǘ��N���X
	CTrashVehicleManager* mpVehicleMgr;
	// �Z�l�Ǘ��N���X
	CResidentManager* mpResidentMgr;
	// �S�~�E���̓G�Ǘ��N���X
	CTrashEnemyManager* mpTrashEnemyMgr;
	// ���ԕ\��UI
	CTimeUI* mpTimeUI;
	// �X�R�A�\��UI
	CTrashScoreUI* mpTrashScoreUI;
	// �x��UI
	CTrashWarningUI* mpWarningUI;
	// �o�ߎ��Ԃ��v��
	float mElapsedTime;

	// �������
	CManualMenu* mpManual;
};
