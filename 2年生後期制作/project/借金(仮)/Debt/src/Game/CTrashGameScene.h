#pragma once
#include "CSceneBase.h"
class CGameMenuBase;
class CVehicleManager;
class CResidentManager;
class CTrashEnemyManager;
class CSound;
class CTimeUI;
class CTrashScoreUI;
class CImage;

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
	// �S�~�E���̓G�Ǘ��N���X
	CTrashEnemyManager* mpTrashEnemyManager;
	// ���ԕ\��UI
	CTimeUI* mpTimeUI;
	// �X�R�A�\��UI
	CTrashScoreUI* mpTrashScoreUI;
	// �o�ߎ��Ԃ��v��
	float mElapsedTime;
	// �|�[�Y�������ǂ���
	bool mIsPause;
};
