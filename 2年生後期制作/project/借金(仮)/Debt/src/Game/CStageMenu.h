#pragma once
#include "CGameMenuBase.h"

class CStageMenu : public CGameMenuBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="menuItemPathList">���j���[�̃A�C�e���摜�̃p�X���X�g</param>
	/// <param name="menuSelectPath">���j���[�̃Z���N�g�摜�̃p�X</param>
	CStageMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath);
	~CStageMenu();

	void Decide(int select) override;

	void Update();

private:
};