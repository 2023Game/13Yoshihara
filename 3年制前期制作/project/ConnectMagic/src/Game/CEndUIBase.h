#pragma once
#include "CTask.h"

class CImage;
class CExpandButton;
class CTextUI2D;

class CEndUIBase : public CTask
{
public:
	// �R���X�g���N�^
	CEndUIBase(std::string bgPath);
	// �f�X�g���N�^
	~CEndUIBase();

	// ��ʏI����
	bool IsEnd() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

protected:
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

	EState mState;		// ���݂̏��
	int mStateStep;		// ��ԓ��ł̃X�e�b�v�Ǘ��p
	float mElapsedTime;	// �o�ߎ��Ԍv���p
	bool mIsEnd;		// ��ʏI���t���O
	int mSelectIndex;	// ���ݑI�����Ă��鍀��

	CTextUI2D* mpText;	// �e�L�X�g
	CImage* mpBg;		// �w�i�C���[�W
	std::vector<CExpandButton*> mButtons;
};