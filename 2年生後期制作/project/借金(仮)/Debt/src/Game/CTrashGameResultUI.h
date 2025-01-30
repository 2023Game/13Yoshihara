#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;

class CTrashGameResultUI : public CTask
{
public:
	// �R���X�g���N�^
	CTrashGameResultUI();
	// �f�X�g���N�^
	~CTrashGameResultUI();

	// ���ʉ�ʏI����
	bool IsEnd() const;
	// ���g���C���邩
	bool IsStartGame() const;
	// ���_�֖߂邩
	bool IsReturnHome() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// �ҋ@���
	void UpdateIdle();
	// ���j���[���J��
	void UpdateOpen();
	// ���j���[�I��
	void UpdateSelect();
	// �t�F�[�h�A�E�g
	void UpdateFadeOut();

	// �^�C�g���̏��
	enum class EState
	{
		eIdle,		// �ҋ@���
		eOpen,		// ���j���[���J��
		eSelect,	// ���j���[�I��
		eFadeOut,	// �t�F�[�h�A�E�g
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);

	// [RETRY]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickOption();
	// [RETURN]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickReturn();


	EState mState;		// ���݂̏��
	int mStateStep;		// ��ԓ��ł̃X�e�b�v�Ǘ��p
	float mElapsedTime;	// �o�ߎ��Ԍv���p
	bool mIsEnd;		// �^�C�g����ʏI���t���O
	int mSelectIndex;	// ���ݑI�����Ă��鍀��

	CFont* mpFont;	// �t�H���g
	CText* mpScore;	// �X�R�A
	CImage* mpResultBg;	// ���ʔw�i�C���[�W
	std::vector<CExpandButton*> mButtons;
};