#pragma once
#include "CTask.h"
#include "CImage.h"
#include "CTextUI2D.h"
#include <vector>
#include "CSound.h"
#include "CExpandButton.h"

// �Q�[�����j���[�̊��N���X
class CGameMenuBase : public CTask
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="menuItemPathList">���j���[�̃A�C�e���摜�̃p�X���X�g</param>
	CGameMenuBase(std::vector<std::string> menuItemPathList);
	// �f�X�g���N�^
	~CGameMenuBase();

	// ���j���[���J��
	void Open();
	// ���j���[�����
	void Close();
	// �J���Ă��邩�ǂ���
	bool IsOpened() const;

	// �X�V
	void Update() override;
	// �f�X�g���N�^
	void Render() override;

	// ���j���[�̗v�f�̗L��������ݒ�
	void SetMenuOnOff(int num, bool isOnOff);

	// ��O�̃��j���[��ݒ�
	void SetPreMenu(CGameMenuBase* preMenu);
	
	// �L�������̐؂�ւ�
	void SetOnOff(bool isOnOff);

protected:
	// �ҋ@
	void UpdateIdle();
	// ���j���[���J��
	virtual void UpdateOpen();
	// ���j���[�I��
	void UpdateSelect();

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
	EState mState;		// ���݂̏��
	int mStateStep;		// ��ԓ��ł̃X�e�b�v�Ǘ��p
	float mElapsedTime;	// �o�ߎ��Ԍv���p

	// [CLOSE]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickClose();

	CImage* mpBackground;					// �w�i�摜
	std::vector<CExpandButton*> mButtons;	// ���j���[�̗v�f
	std::vector<bool> mMenuOnOff;			// ���j���[�̗v�f���L������
	std::vector<CTextUI2D*> mMenuTexts;		// ���j���[�̐����p�e�L�X�g
	bool mIsOpened;						// �J���Ă��邩
	CGameMenuBase* mpPreMenu;			// ��O�̃��j���[�̊i�[�p
	bool mIsNextMenu;	// ���̃��j���[�����邩
	// ���ʉ�
	CSound* mpSelectSE;
	CSound* mpPushSE;
	CSound* mpBuzzerSE;
};