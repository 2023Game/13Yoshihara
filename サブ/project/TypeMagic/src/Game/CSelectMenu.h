#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;
class CScrollUIBase;

class CSelectMenu : public CTask
{
public:
	// �R���X�g���N�^
	CSelectMenu();
	// �f�X�g���N�^
	~CSelectMenu();

	// �Z���N�g��ʂ��I����
	bool IsEnd() const;
	// �ΐ���J�n���邩
	bool IsBattleStart() const;
	// ��O�ɖ߂邩
	bool IsReturn() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// ���j���[���J��
	void UpdateOpen();
	// ���j���[��I��
	void UpdateSelect();
	// �t�F�[�h�A�E�g
	void UpdateFadeOut();

	// ���
	enum class EState
	{
		eOpen,		// ���j���[���J��
		eSelect,	// ���j���[�I��
		eFadeOut,	// �t�F�[�h�A�E�g
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);

	// [BattleStart]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickBattleStart();
	// [Return]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickReturn();
	// [SelectElement]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickSelectElement();

	EState mState;		// ���݂̏��
	int mStateStep;		// ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime;	// �o�ߎ��Ԍv���p
	bool mIsEnd;		// ��ʏI���t���O
	int mSelectIndex;	// ���ݑI�����Ă��鍀��

	CFont* mpFont;	// �t�H���g
	CImage* mpBg;	// �w�i�C���[�W
	std::vector<CExpandButton*> mButtons;

	// �X�N���[���ł���UI
	CScrollUIBase* mpScrollUI;

	CImage* mpPlayerImg;	// �v���C���[�̃A�C�R���摜
};