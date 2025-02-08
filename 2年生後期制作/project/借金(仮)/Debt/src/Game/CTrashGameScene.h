#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CVehicleManager;
class CResidentManager;
class CTrashEnemyManager;
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
	CVehicleManager* mpVehicleMgr;
	// �Z�l�Ǘ��N���X
	CResidentManager* mpResidentMgr;
	// �S�~�E���̓G�Ǘ��N���X
	CTrashEnemyManager* mpTrashEnemyMgr;
	// ���ԕ\��UI
	CTimeUI* mpTimeUI;
	// �X�R�A�\��UI
	CTrashScoreUI* mpTrashScoreUI;
	// �o�ߎ��Ԃ��v��
	float mElapsedTime;
};
