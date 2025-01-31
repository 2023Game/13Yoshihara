#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;

class CResultUIBase : public CTask
{
public:
	// �R���X�g���N�^
	CResultUIBase(std::string backGroundPath);
	// �f�X�g���N�^
	~CResultUIBase();

	// ���ʉ�ʏI����
	bool IsEnd() const;
	// ���_�֖߂邩
	bool IsReturnHome() const;

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

	// [���_��]�N���b�N���̃R�[���o�b�N�֐�
	virtual void OnClickReturn();


	EState mState;		// ���݂̏��
	int mStateStep;		// ��ԓ��ł̃X�e�b�v�Ǘ��p
	float mElapsedTime;	// �o�ߎ��Ԍv���p
	bool mIsEnd;		// �^�C�g����ʏI���t���O
	int mSelectIndex;	// ���ݑI�����Ă��鍀��

	CFont* mpScoreFont;				// �X�R�A�̃t�H���g
	CText* mpScoreText;				// �X�R�A�Ƃ��������̃e�L�X�g
	CImage* mpResultBg;				// ���U���g�w�i�C���[�W
	CExpandButton* mpReturnButton;	// ���_�֖߂�{�^��
};