#pragma once
#include "CTask.h"
#include "CImage.h"
#include "CText.h"
#include <vector>

// TODO:��ޕʂ̃Q�[�����j���[�ɑΉ�������
class CGameMenuBase : public CTask
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="menuItemPathList">���j���[�̃A�C�e���摜�̃p�X���X�g</param>
	/// <param name="menuSelectPath">���j���[�̃Z���N�g�摜�̃p�X</param>
	CGameMenuBase(std::vector<std::string> menuItemPathList, std::string menuSelectPath);
	// �f�X�g���N�^
	~CGameMenuBase();

	// ���j���[���J��
	void Open();
	// ���j���[�����
	void Close();
	// �J���Ă��邩�ǂ���
	bool IsOpened() const;

	// ���肵���{�^���̏���
	virtual void Decide(int select);

	// �X�V
	void Update() override;
	// �f�X�g���N�^
	void Render() override;

protected:
	CImage* mpBackground;				// �w�i�摜
	std::vector<CImage*> mMenuItems;	// ���j���[�̗v�f
	std::vector<CText*> mMenuTexts;		// ���j���[�̃e�L�X�g
	CImage* mpSelectFrame;				// �I�𒆂̉摜
	int mSelectIndex;					// �I�𒆂̔ԍ�
	bool mIsOpened;						// �J���Ă��邩
	CGameMenuBase* mpPrevMenu;			// ��O�̃��j���[�̊i�[�p
};