#pragma once
#include "CTask.h"
#include "CImage.h"
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
	~CGameMenuBase();

	void Open();
	void Close();
	bool IsOpened() const;

	// ���肵���{�^���̏���
	virtual void Decide(int select);

	void Update() override;
	void Render() override;

protected:
	CImage* mpBackground;
	std::vector<CImage*> mMenuItems;
	CImage* mpSelectFrame;
	int mSelectIndex;
	bool mIsOpened;
	CGameMenuBase* mPrevMenu;	// ��O�̃��j���[�̊i�[�p
};