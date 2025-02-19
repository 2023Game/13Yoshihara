#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;

class CGameOverUI : public CTask
{
public:
	// �R���X�g���N�^
	CGameOverUI();
	// �f�X�g���N�^
	~CGameOverUI();

	// �Q�[���I�[�o�[��ʏI����
	bool IsEnd() const;
	// �O���֖߂邩
	bool IsPreDay() const;
	// �Q�[�����I�����邩
	bool IsExitGame() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// ���j���[���J��
	void UpdateOpen();
	// ���j���[�I��
	void UpdateSelect();
	// �t�F�[�h�A�E�g
	void UpdateFadeOut();

	// �^�C�g���̏��
	enum class EState
	{
		eOpen,		// ���j���[���J��
		eSelect,	// ���j���[�I��
		eFadeOut,	// �t�F�[�h�A�E�g
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);

	// [�O����]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickPreDay();
	// [�I��]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickQuit();

	EState mState;		// ���݂̏��
	int mStateStep;		// ��ԓ��ł̃X�e�b�v�Ǘ��p
	float mElapsedTime;	// �o�ߎ��Ԍv���p
	bool mIsEnd;		// �^�C�g����ʏI���t���O
	int mSelectIndex;	// ���ݑI�����Ă��鍀��

	CFont* mpFont;	// �t�H���g
	CText* mpGameOverText;	// �Q�[���I�[�o�[�e�L�X
	CImage* mpGameOverBg;	// �Q�[���I�[�o�[�w�i�C���[�W
	std::vector<CExpandButton*> mButtons;
};